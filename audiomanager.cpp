#include "audiomanager.h"

#include <QDebug>

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define FRAMES_PER_BUFFER 512
#define LATENCY_THRESHOLD_MS 50 // Maximum acceptable latency before warning
#define LATENCY_ADJUSTMENT_STEP 0.05 // Gradual buffer size correction
#define MAX_SAMPLE_VALUE 1.0f // Maximum amplitude for normalization
#define LATENCY_CORRECTION_FACTOR 0.75 // Reduce buffer size by 25% if latency is too high

AudioManager::AudioManager(QObject *parent)
    : QObject(parent), stream(nullptr)
{
    Pa_Initialize();
    sharedMixBuffer.resize(FRAMES_PER_BUFFER * CHANNELS, 0.0f);
    workerThread->start(); // Start audio processing thread
}

AudioManager::~AudioManager(){
    stop();
    workerThread->requestInterruption();
    workerThread->quit();
    workerThread->wait();

    // Clean up each media player’s resources.
    QMutexLocker locker(&mutex);
    for (auto &wrapper : mediaPlayers) {
        if (wrapper.audioSink) {
            wrapper.audioSink->stop();
            delete wrapper.audioSink;
        }
        if (wrapper.buffer) {
            wrapper.buffer->close();
            delete wrapper.buffer;
        }
    }
    mediaPlayers.clear();

    Pa_Terminate();
}

QStringList AudioManager::getInputDevices() {
    QStringList devices;
    int numDevices = Pa_GetDeviceCount();
    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo->maxInputChannels > 0) {
            devices.append(QString::fromUtf8(deviceInfo->name));
        }
    }
    return devices;
}

QStringList AudioManager::getOutputDevices() {
    QStringList devices;
    int numDevices = Pa_GetDeviceCount();
    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo->maxOutputChannels > 0) {
            devices.append(QString::fromUtf8(deviceInfo->name));
        }
    }
    return devices;
}

bool AudioManager::start(){
    PaStreamParameters inputParams, outputParams;

    inputParams.device = Pa_GetDefaultInputDevice();
    outputParams.device = Pa_GetDefaultOutputDevice();

    qDebug()<<"inp: "<<inputParams.device<<" "<<Pa_GetDeviceInfo(inputParams.device)->name;
    qDebug()<<"out: "<<outputParams.device<<" "<<Pa_GetDeviceInfo(outputParams.device)->name;

    if (inputParams.device == paNoDevice || outputParams.device == paNoDevice) {
        emit errorOccurred("No input or output device available.");
        return false;
    }

    inputParams.channelCount = CHANNELS;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency = Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = nullptr;

    outputParams.channelCount = CHANNELS;
    outputParams.sampleFormat = paFloat32;
    outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
    outputParams.hostApiSpecificStreamInfo = nullptr;

    PaError err = Pa_OpenStream(&stream, &inputParams, &outputParams, SAMPLE_RATE,
                                FRAMES_PER_BUFFER, paClipOff, audioCallback, this);

    if (err != paNoError) {
        emit errorOccurred(QString("Failed to open stream: %1").arg(Pa_GetErrorText(err)));
        return false;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        emit errorOccurred(QString("Failed to start stream: %1").arg(Pa_GetErrorText(err)));
        return false;
    }

    emit audioProcessingStarted();
    return true;
}

void AudioManager::stop(){
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        stream = nullptr;
        emit audioProcessingStopped();
    }
}

void AudioManager::addMediaPlayer(QMediaPlayer *player){
    QMutexLocker locker(&mutex);

    for (const MediaPlayerWrapper &wrapper : mediaPlayers) {
        if (wrapper.player == player) {
            return; //already added
        }
    }

    // Create the QAudioSink and QBuffer.
    QAudioDevice device = QAudioDevice::defaultOutputDevice();
    QAudioSink *audioSink = new QAudioSink(device, this);
    QBuffer *buffer = new QBuffer(this);
    buffer->open(QIODevice::ReadWrite);

    // Start the QAudioSink: it will return a QIODevice* which will write audio data into our buffer.
    audioSink->start(buffer);

    // Assign the QAudioSink to the QMediaPlayer.
    player->setAudioOutput(audioSink);

    MediaPlayerWrapper wrapper;
    wrapper.player = player;
    wrapper.audioSink = audioSink;
    wrapper.buffer = buffer;
    wrapper.playbackSpeed = 1.0f;
    mediaPlayers.append(wrapper);

}

void AudioManager::removeMediaPlayer(QMediaPlayer *player){
    QMutexLocker locker(&mutex);
    for (int i = 0; i < mediaPlayers.size(); ++i) {
        if (mediaPlayers[i].player == player) {
            if (mediaPlayers[i].audioSink) {
                mediaPlayers[i].audioSink->stop();
                delete mediaPlayers[i].audioSink;
            }
            if (mediaPlayers[i].buffer) {
                mediaPlayers[i].buffer->close();
                delete mediaPlayers[i].buffer;
            }
            mediaPlayers.removeAt(i);
            break;
        }
    }
}

int AudioManager::audioCallback(const void *input, void *output,
                                unsigned long frameCount,
                                const PaStreamCallbackTimeInfo *timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData)
{
    Q_UNUSED(timeInfo);
    Q_UNUSED(statusFlags);

    AudioManager *self = static_cast<AudioManager *>(userData);
    self->processAudio(static_cast<const float *>(input), static_cast<float *>(output), frameCount);
    return paContinue;
}

void AudioManager::processAudio(const float *input, float *output, unsigned long frameCount)
{

    // First, copy the passthrough (e.g. microphone) audio.
    for (unsigned long i = 0; i < frameCount * CHANNELS; i++) {
        output[i] = input[i];
    }
    // Then, add the media player audio stored in the shared mix buffer.
    if (mixBufferMutex.tryLock()) {  // tryLock to avoid blocking the real-time callback
        if (sharedMixBuffer.size() == frameCount * CHANNELS) {
            for (unsigned long i = 0; i < frameCount * CHANNELS; i++) {
                output[i] += sharedMixBuffer[i];
            }
        }
        mixBufferMutex.unlock();
    }

    // QMutexLocker locker(&mutex);

    // float maxSample = 0.0f;
    // QVector<float> mixBuffer(frameCount * CHANNELS, 0.0f);

    // for (MediaPlayerWrapper &wrapper : mediaPlayers) {
    //     if (wrapper.player->isPlaying()) {
    //         qint64 bufferSize = wrapper.buffer.size();
    //         int latencyMs = (bufferSize * 1000) / (SAMPLE_RATE * sizeof(float) * CHANNELS);

    //         if (latencyMs > LATENCY_THRESHOLD_MS) {
    //             qWarning() << "High latency detected for player" << wrapper.player
    //                        << "- Current latency:" << latencyMs << "ms";

    //             // Reduce buffer size dynamically to fix latency
    //             qint64 newSize = static_cast<qint64>(bufferSize * LATENCY_CORRECTION_FACTOR);
    //             wrapper.buffer.buffer().resize(newSize);

    //             // Adaptive latency correction
    //             wrapper.playbackSpeed -= LATENCY_ADJUSTMENT_STEP;
    //             wrapper.playbackSpeed = std::max(0.5f, wrapper.playbackSpeed); // Prevent too slow playback
    //             wrapper.player->setPlaybackRate(wrapper.playbackSpeed);
    //         }

    //         QByteArray audioData = wrapper.buffer.read(bufferSize);
    //         float *bufferData = reinterpret_cast<float *>(audioData.data());

    //         // Advanced mixing: Weighted summing with volume preservation
    //         for (unsigned long i = 0; i < frameCount * CHANNELS; i++) {
    //             mixBuffer[i] += bufferData[i] / mediaPlayers.size(); // Divide by number of active sources
    //             maxSample = std::max(maxSample, std::abs(mixBuffer[i]));
    //         }

    //         // Reset buffer to prevent excess growth
    //         wrapper.buffer.buffer().clear();
    //         wrapper.buffer.seek(0);
    //     }
    // }

    // // Normalize if necessary
    // if (maxSample > MAX_SAMPLE_VALUE) {
    //     float normalizationFactor = MAX_SAMPLE_VALUE / maxSample;
    //     for (unsigned long i = 0; i < frameCount * CHANNELS; i++) {
    //         mixBuffer[i] *= normalizationFactor;
    //     }
    // }

    // // Apply mixed buffer to output
    // for (unsigned long i = 0; i < frameCount * CHANNELS; i++) {
    //     output[i] += mixBuffer[i];
    // }
}

// Worker Thread Implementation
AudioWorker::AudioWorker(AudioManager *manager) : audioManager(manager) {}

void AudioWorker::run(){
    // Run until the thread is interrupted.
    while (!isInterruptionRequested()) {
        // Create a local mix buffer (one frame’s worth of samples)
        QVector<float> localMixBuffer(FRAMES_PER_BUFFER * CHANNELS, 0.0f);
        int activeSources = 0;

        {
            // Lock the mediaPlayers list for safe access.
            QMutexLocker locker(&audioManager->mutex);
            for (auto &wrapper : audioManager->mediaPlayers) {
                if (wrapper.player->isPlaying()) {
                    ++activeSources;
                    qint64 bufferSize = wrapper.buffer->size();
                    int latencyMs = int((bufferSize * 1000) / (SAMPLE_RATE * sizeof(float) * CHANNELS));
                    if (latencyMs > LATENCY_THRESHOLD_MS) {
                        qWarning() << "High latency detected for player" << wrapper.player
                                   << "- Current latency:" << latencyMs << "ms. Applying correction.";
                        wrapper.playbackSpeed -= LATENCY_ADJUSTMENT_STEP;
                        if (wrapper.playbackSpeed < 0.5f)
                            wrapper.playbackSpeed = 0.5f;
                        wrapper.player->setPlaybackRate(wrapper.playbackSpeed);
                    }
                    QByteArray audioData = wrapper.buffer->read(bufferSize);
                    // Ensure that we have enough data; otherwise, skip this source.
                    if (audioData.size() >= int(FRAMES_PER_BUFFER * CHANNELS * sizeof(float))) {
                        float *bufferData = reinterpret_cast<float *>(audioData.data());
                        for (unsigned long i = 0; i < FRAMES_PER_BUFFER * CHANNELS; i++) {
                            // Divide by the number of active sources so that no one source dominates.
                            localMixBuffer[i] += bufferData[i] / activeSources;
                        }
                    }
                    // Clear the internal QBuffer to prevent buildup and reduce latency.
                    wrapper.buffer->buffer().clear();
                    wrapper.buffer->seek(0);
                }
            }
        }
        // Normalize the mixed buffer if the signal is too high.
        float maxSample = 0.0f;
        for (unsigned long i = 0; i < FRAMES_PER_BUFFER * CHANNELS; i++) {
            maxSample = std::max(maxSample, std::abs(localMixBuffer[i]));
        }
        if (maxSample > MAX_SAMPLE_VALUE) {
            float normalizationFactor = MAX_SAMPLE_VALUE / maxSample;
            for (unsigned long i = 0; i < FRAMES_PER_BUFFER * CHANNELS; i++) {
                localMixBuffer[i] *= normalizationFactor;
            }
        }
        // Update the shared mix buffer (used by the audio callback)
        {
            QMutexLocker locker(&audioManager->mixBufferMutex);
            audioManager->sharedMixBuffer = localMixBuffer;
        }
        msleep(10); // Sleep a short time to reduce CPU load
    }
}

QString AudioManager::intToString(int n){
    switch (n){
    case 0:
        return "0";
    case 1:
        return "1";
    case 2:
        return "2";
    case 3:
        return "3";
    case 4:
        return "4";
    case 5:
        return "5";
    case 6:
        return "6";
    case 7:
        return "7";
    case 8:
        return "8";
    case 9:
        return "9";
    case 10:
        return "10";
    default:
        return "-1";


    }
}

// bool AudioManager::startLoopback(int inputDeviceIndex, int outputDeviceIndex) {
//     if (stream) {
//         emit errorOccurred("Audio loopback is already running.");
//         return false;
//     }

//     PaStreamParameters inputParams, outputParams;
//     inputParams.device = inputDeviceIndex;
//     outputParams.device = outputDeviceIndex;

//     //get device information so we can set the appropriate channel count.
//     const PaDeviceInfo *inputInfo = Pa_GetDeviceInfo(inputParams.device);
//     const PaDeviceInfo *outputInfo = Pa_GetDeviceInfo(outputParams.device);
//     channels = std::min(inputInfo->maxInputChannels, outputInfo->maxOutputChannels);
//     if (channels <= 0) {
//         emit errorOccurred("Invalid number of channels available for loopback.");
//         qDebug()<<"Channels: "<<channels;
//         return false;
//     }

//     inputParams.channelCount = channels;
//     inputParams.sampleFormat = paFloat32;
//     inputParams.suggestedLatency = inputInfo->defaultLowInputLatency;
//     inputParams.hostApiSpecificStreamInfo = nullptr;

//     outputParams.channelCount = channels;
//     outputParams.sampleFormat = paFloat32;
//     outputParams.suggestedLatency = outputInfo->defaultLowOutputLatency;
//     outputParams.hostApiSpecificStreamInfo = nullptr;

//     qDebug()<<"input index: "<<inputDeviceIndex<<"name: "<<inputInfo->name;
//     qDebug()<<"output index: "<<outputDeviceIndex<<"name: "<<outputInfo->name;

//     PaError openErrCode = Pa_OpenStream(&stream, &inputParams, &outputParams, SAMPLE_RATE,
//                                         FRAMES_PER_BUFFER, paClipOff, audioCallback, this);
//     if (openErrCode != paNoError) {
//         qDebug()<<"Err: "<<openErrCode<<"Text: "<<Pa_GetErrorText(openErrCode);
//         emit errorOccurred("Failed to open audio stream. Error: "+intToString(openErrCode));
//         return false;
//     }

//     PaError startErrCode = Pa_StartStream(stream);
//     if (startErrCode != paNoError) {
//         qDebug()<<"Err: "<<startErrCode<<"Text: "<<Pa_GetErrorText(startErrCode);
//         emit errorOccurred("Failed to start audio stream. Error: "+intToString(startErrCode));
//         return false;
//     }

//     emit audioLoopbackStarted();
//     return true;
// }

// void AudioManager::stopLoopback() {
//     if (stream) {
//         Pa_StopStream(stream);
//         Pa_CloseStream(stream);
//         stream = nullptr;
//         emit audioLoopbackStopped();
//     }
// }

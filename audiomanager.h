#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <portaudio.h>

#include <QAudioBufferOutput>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDevice>
#include <QAudioSink>
#include <QBuffer>
#include <QVector>
#include <QObject>
#include <QThread>
#include <QMutex>

class AudioWorker;

class AudioManager : public QObject
{
    Q_OBJECT
public:
    explicit AudioManager(QObject *parent = nullptr);
    ~AudioManager();

    QMutex mutex;
    QMutex mixBufferMutex;   // protects sharedMixBuffer
    QVector<float> sharedMixBuffer;

    QStringList getInputDevices();
    QStringList getOutputDevices();
    bool start();
    void stop();
    void addMediaPlayer(QMediaPlayer *player);
    void removeMediaPlayer(QMediaPlayer *player);

signals:
    void errorOccurred(const QString &errorMessage);
    void audioProcessingStarted();
    void audioProcessingStopped();

private:
    struct MediaPlayerWrapper {
        QMediaPlayer   *player;
        QAudioSink     *audioSink;
        QBuffer        *buffer;
        float           playbackSpeed = 1.0f;
    };

    static int audioCallback(const void *input, void *output,
                             unsigned long frameCount,
                             const PaStreamCallbackTimeInfo *timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData);

    void processAudio(const float *input, float *output, unsigned long frameCount);
    // void mixAudio(float *output, unsigned long frameCount);

    PaStream *stream;
    QVector<MediaPlayerWrapper> mediaPlayers;

    AudioWorker *workerThread = nullptr;

    QString intToString(int);

    friend class AudioWorker;
};

class AudioWorker : public QThread
{
    Q_OBJECT
public:
    explicit AudioWorker(AudioManager *manager);
    void run() override;

private:
    AudioManager *audioManager;
};


#endif // AUDIOMANAGER_H

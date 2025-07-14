#include "soundboard.h"

Soundboard::Soundboard(QWidget *parent)
    : QMainWindow{parent}
{
    //load initialization data from "init.json" if it exists
    loadInitData();

    //create a system tray icon
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/icon.ico"), this);

    //create the tray context menu
    trayMenu = new QMenu(this);
    QAction *restoreAction = new QAction("Show", this);
    QAction *quitAction = new QAction("Exit", this);

    //connect the tray context menu buttons to their respective actions
    connect(restoreAction, &QAction::triggered, this, &Soundboard::showFromTray);
    connect(quitAction, &QAction::triggered, this, &Soundboard::exitApp);

    //add the actions
    trayMenu->addAction(restoreAction);
    trayMenu->addAction(quitAction);

    //configure and show the tray icon
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    //connect tray icon clicked signal to showFromTray
    connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason r){
        if(r == QSystemTrayIcon::ActivationReason::MiddleClick){
            this->exitApp();
            return;
        }
        else if(r == QSystemTrayIcon::ActivationReason::Trigger || r == QSystemTrayIcon::ActivationReason::DoubleClick){
            this->showFromTray();
            return;
        }
    });

    //the format the serial data should be sent/recieved in
    serialData = "0|0|0|0|0|0|0|0|0|0\n";
    oldSerialData = "0|0|0|0|0|0|0|0|0|0\n";

    //initialize the serial port object
    serial = new QSerialPort(this);

    //connect incoming serial data to our recieveSerialData slot
    connect(serial, &QSerialPort::readyRead, this, &Soundboard::receiveSerialData);

    //catch any errors related to the serial port
    connect(serial, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        qDebug()<<"Serial Error: "<<error;
        serialError = error;
        if(error != QSerialPort::NoError && serial->isOpen()){
            qDebug()<<"Disconnecting. Error: "<<error;
            disconnectSerialPort(true, true);
        }
    });

    //initialize the main soundboard widget
    sbWidget = new SoundboardWidget(this);

    //initialize and define the mediaplayers for device 1
    for(int i=0; i<10; i++){
        device1MediaPlayers.push_back(new QMediaPlayer(this));
        connect(device1MediaPlayers[i], &QMediaPlayer::mediaStatusChanged, this, [this, i](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::EndOfMedia) {
                soundEnd(i);
            }
        });
    }

    //initialize and define the mediaplayers for device 2
    for(int i=0; i<10; i++){
        device2MediaPlayers.push_back(new QMediaPlayer(this));
        connect(device2MediaPlayers[i], &QMediaPlayer::mediaStatusChanged, this, [this, i](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::EndOfMedia) {
                soundEnd(i);
            }
        });
    }

    //the main vertical layout for the app
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QWidget *widget = new QWidget(this);
    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    //serial port ComboBox and refresh button layout
    QHBoxLayout *portLayout = new QHBoxLayout;
    portComboBox = new QComboBox(this);
    portComboBox->setMinimumWidth(180);
    refreshSerialPorts();
    portLayout->addWidget(portComboBox);

    //set initial connection status
    connectionStatusIcon_NONE = new QIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
    connectionStatusIcon_ERR_ = new QIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
    connectionStatusIcon_TRUE = new QIcon (":/icons/connected.png");
    connectionStatusIconWrapper = new QLabel();
    connectionStatusIconWrapper->setPixmap(connectionStatusIcon_NONE->pixmap(16,16));
    connectionStatusIconWrapper->setToolTip("Not Connected.");
    portLayout->addWidget(connectionStatusIconWrapper);

    //define + initialize + connect the refresh/connect/disconnect buttons
    QPushButton *refreshButton = new QPushButton(tr("Refresh"), this);
    portLayout->addWidget(refreshButton);
    connect(refreshButton, &QPushButton::clicked, this, &Soundboard::refreshSerialPorts);
    QPushButton *connectButton = new QPushButton(tr("Connect"), this);
    portLayout->addWidget(connectButton);
    connect(connectButton, &QPushButton::clicked, this, [this](){
        connectToSerialPort(true);
    });
    QPushButton *disconnectButton = new QPushButton(tr("Disconnect"), this);
    portLayout->addWidget(disconnectButton);
    connect(disconnectButton, &QPushButton::clicked, this, [this](){
        disconnectSerialPort(false, true);
    });
    mainLayout->addLayout(portLayout);

    //add the soundboard ui widget
    mainLayout->addWidget(sbWidget);

    //connections for the soundboard ui widget
    connect(sbWidget, &SoundboardWidget::fileDropped,        this, &Soundboard::fileDropped);
    connect(sbWidget, &SoundboardWidget::buttonRightClicked, this, &Soundboard::selectSound);
    connect(sbWidget, &SoundboardWidget::buttonPressed,      this, &Soundboard::playSound);

    //device selection and volume sliders
    //output device 1 selection
    output1ComboBox = new QComboBox(this);
    QLabel *l1 = new QLabel(tr("Select Output One"), this);
    l1->setToolTip("Sound effects will be sent to this device");
    mainLayout->addWidget(l1);
    mainLayout->addWidget(output1ComboBox);

    //output device 2 selection
    output2ComboBox = new QComboBox(this);
    QLabel *l2 = new QLabel(tr("Select Output Two"), this);
    l2->setToolTip("Sound effects will be sent to this device");
    mainLayout->addWidget(l2);
    mainLayout->addWidget(output2ComboBox);

    populateAudioDevices();

    connect(output1ComboBox, &QComboBox::currentIndexChanged, this, &Soundboard::combo1Changed);
    connect(output2ComboBox, &QComboBox::currentIndexChanged, this, &Soundboard::combo2Changed);

    //add Output 1 Volume Slider
    QHBoxLayout *output1VolumeLayout = new QHBoxLayout();
    QLabel *output1VolumeLabel = new QLabel(tr("Output 1 Volume "), this);
    output1VolumeSlider = new QSlider(Qt::Horizontal, this);
    output1VolumeSlider->setMinimum(0);
    output1VolumeSlider->setMaximum(100);
    output1VolumeValueLabel = new QLabel(QString("%1 %").arg(output1Volume), this);
    QPushButton *resetOutput1VolumeButton = new QPushButton(tr("Reset"), this);
    output1VolumeLayout->addWidget(output1VolumeLabel);
    output1VolumeLayout->addWidget(output1VolumeSlider);
    output1VolumeLayout->addWidget(output1VolumeValueLabel);
    output1VolumeLayout->addWidget(resetOutput1VolumeButton);
    mainLayout->addLayout(output1VolumeLayout);

    //add Output 2 Volume Slider
    QHBoxLayout *output2VolumeLayout = new QHBoxLayout();
    QLabel *output2VolumeLabel = new QLabel(tr("Output 2 Volume"), this);
    output2VolumeSlider = new QSlider(Qt::Horizontal, this);
    output2VolumeSlider->setMinimum(0);
    output2VolumeSlider->setMaximum(100);
    output2VolumeValueLabel = new QLabel(QString("%1 %").arg(output2Volume), this);
    QPushButton *resetOutput2VolumeButton = new QPushButton(tr("Reset"), this);
    output2VolumeLayout->addWidget(output2VolumeLabel);
    output2VolumeLayout->addWidget(output2VolumeSlider);
    output2VolumeLayout->addWidget(output2VolumeValueLabel);
    output2VolumeLayout->addWidget(resetOutput2VolumeButton);
    mainLayout->addLayout(output2VolumeLayout);

    //connect slider signals to their respective volume update slots
    connect(output1VolumeSlider, &QSlider::valueChanged, this, &Soundboard::updateOutput1VolumeLabel);
    connect(output2VolumeSlider, &QSlider::valueChanged, this, &Soundboard::updateOutput2VolumeLabel);

    //connect reset buttons to their respective reset functions
    connect(resetOutput1VolumeButton, &QPushButton::clicked, this, &Soundboard::resetOutput1Volume);
    connect(resetOutput2VolumeButton, &QPushButton::clicked, this, &Soundboard::resetOutput2Volume);

    //setup the menu bar
    QMenu *userConfigMenu = menuBar()->addMenu(tr("User Config"));
    QAction *loadConfigAction = new QAction(tr("Load Config"), this);
    QAction *saveConfigAction = new QAction(tr("Save Config"), this);
    userConfigMenu->addAction(loadConfigAction);
    userConfigMenu->addAction(saveConfigAction);

    QMenu *initConfigMenu = menuBar()->addMenu(tr("Startup"));
    QAction *toggleQuitSaveConfigAction = new QAction(tr("Save Config on Program Exit"), this);
    toggleQuitSaveConfigAction->setCheckable(true);
    initConfigMenu->addAction(toggleQuitSaveConfigAction);
    QAction *toggleStartMinimizedAction = new QAction(tr("Start Progam Minimized"), this);
    toggleStartMinimizedAction->setCheckable(true);
    initConfigMenu->addAction(toggleStartMinimizedAction);
    startupConfigMenu = initConfigMenu->addMenu(tr("Set Startup Config"));
    QAction *helpStartupAction = new QAction(tr("Start Program on Login"), this);
    helpStartupAction->setIcon(QIcon(":/icons/info.ico"));
    helpStartupAction->setToolTip("Opens instructions on how to add this program to your startup folder.");
    initConfigMenu->addAction(helpStartupAction);
    initConfigMenu->setToolTipsVisible(true);

    //add the option to not load any config at startup
    QAction *noStartupConfigAction = new QAction(tr("None"), this);
    startupConfigMenu->setToolTipsVisible(true);
    noStartupConfigAction->setCheckable(true);
    noStartupConfigAction->setChecked(cfgToLoadAtStartup == "NONE");
    noStartupConfigAction->setToolTip(tr("Manually load a configuration file first to see it in this list!"));
    startupConfigMenu->addAction(noStartupConfigAction);
    connect(noStartupConfigAction, &QAction::triggered, this, [this](){
        startupConfigSelected("NONE");
    });

    //update the known configs
    updateKnownConfigsMenu();

    connect(loadConfigAction, &QAction::triggered, this, [this](){
        loadConfig(false);
    });
    connect(saveConfigAction, &QAction::triggered, this, &Soundboard::saveConfig);
    connect(helpStartupAction, &QAction::triggered, this, &Soundboard::openStartupHelp);

    //check if the program is currently set to start at boot and update the action
    toggleQuitSaveConfigAction->setChecked(saveCfgAtShutdown);
    toggleStartMinimizedAction->setChecked(startMinimized);

    connect(toggleQuitSaveConfigAction, &QAction::triggered, this, [this](bool checked){
        saveCfgAtShutdown = checked;
    });
    connect(toggleStartMinimizedAction, &QAction::triggered, this, [this](bool checked){
        startMinimized = checked;
    });

    //self-explanatory
    if (loadCfgAtStartup)
        loadConfig(true);

    device1AudioOutput = new QAudioOutput(outputDevice1, this);
    device2AudioOutput = new QAudioOutput(outputDevice1, this);

    output1VolumeSlider->setValue(output1Volume);
    output2VolumeSlider->setValue(output2Volume);
}

Soundboard::~Soundboard() {
}

//detect window minimize event
void Soundboard::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
            saveInitData();
            hide();  //hide window if minimized
        }
    }
    QMainWindow::changeEvent(event); //call the super method
}

//show the window from tray
void Soundboard::showFromTray() {
    showNormal();
    activateWindow();
}

//public exit point (calls exitApp)
void Soundboard::publicAppExitPoint(){
    this->exitApp();
}

//exit the application
void Soundboard::exitApp() {
    saveInitData();
    trayIcon->hide();//hide the tray icon
    if(saveCfgAtShutdown){
        if(configChanged()){
            QMessageBox::StandardButton choice = QMessageBox::information(this, tr("Question"), tr("Overwrite config?"), QMessageBox::Yes | QMessageBox::No);
            if(choice == QMessageBox::Yes){
                saveConfig(true);
            }
        }
    }
    QApplication::quit();//call the super method
}

//handle the close event
void Soundboard::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    exitApp();
}

//updates the serial port combo box with current serial ports
void Soundboard::refreshSerialPorts() {
    //clear any existing entries
    portComboBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        portComboBox->addItem(info.portName() + " (" + info.manufacturer() + ")", info.systemLocation());
    }
    if(portComboBox->count() == 0)
        portComboBox->setToolTip("Connect a serial device, then hit refresh.");
    else portComboBox->setToolTip("");
}

//connect to the serial port selected in the combo box
void Soundboard::connectToSerialPort(bool popup) {
    //close the serial port if it is already open
    if (serial->isOpen()) {
        serial->close();
    }

    //grab the selected serial port
    QString selectedPort = portComboBox->currentData().toString();

    //check if a serial port is connected
    if (!selectedPort.isEmpty()) {
        //set information about the port
        serial->setPortName(selectedPort);
        serial->setBaudRate(currentBaudRate);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        if(serial->isOpen())serial->setDataTerminalReady(true); //prevent arduino resets
        if(serial->isOpen())serial->setRequestToSend(true);

        //attempt to open the serial port
        if (serial->open(QIODevice::ReadWrite)){
            if(serial->isOpen())serial->setDataTerminalReady(true); //prevent arduino resets
            if(serial->isOpen())serial->setRequestToSend(true);
            //if no errors occured, then connection was successful. update the status icon and tooltip
            if(serialError == QSerialPort::SerialPortError::NoError){
                connectionStatusIconWrapper->setPixmap(connectionStatusIcon_TRUE->pixmap(16,16));
                connectionStatusIconWrapper->setToolTip(tr("Connected to %1 at baud rate %2.").arg(selectedPort).arg(currentBaudRate));
                if(popup)QMessageBox::information(this, tr("Connected"), tr("Successfully connected to %1 at baud rate %2").arg(selectedPort).arg(currentBaudRate));
                return;
            }
            else{
                connectionStatusIconWrapper->setPixmap(connectionStatusIcon_ERR_->pixmap(16,16));
                connectionStatusIconWrapper->setToolTip(tr("Disconnected. Error: %1").arg(toString(serialError)));
                QMessageBox::critical(this, tr("Error"), tr("Unable to connect to serial port.\nError: %1").arg(toString(serialError)));
            }
        }
        else{
            //the serial port wasn't able to be opened
            connectionStatusIconWrapper->setPixmap(connectionStatusIcon_ERR_->pixmap(16,16));
            connectionStatusIconWrapper->setToolTip(tr("Failed to open serial port %1 with baud rate %2.").arg(selectedPort).arg(currentBaudRate));
            QMessageBox::critical(this, tr("Error"), tr("Failed to open serial port %1 with baud rate %2.").arg(selectedPort).arg(currentBaudRate));
            return;
        }
    }
    else {
        //no port was selected in the combo box
        connectionStatusIconWrapper->setPixmap(connectionStatusIcon_NONE->pixmap(16,16));
        connectionStatusIconWrapper->setToolTip(tr("Not Connected."));
        QMessageBox::critical(this, tr("Error"), tr("Please select a port."));
    }

}

//disconnect from the current serial port, if there is a connection
void Soundboard::disconnectSerialPort(bool error, bool popup){
    //if the serial port is already closed, punch the user in the face
    if(!serial->isOpen()){
        connectionStatusIconWrapper->setPixmap(connectionStatusIcon_NONE->pixmap(16,16));
        connectionStatusIconWrapper->setToolTip(tr("Not connected."));
        QMessageBox::critical(this, tr("Error"), tr("Not connected to a serial port."));
        return;
    }
    if(error && popup){
        if(serial->isOpen())serial->close();
        connectionStatusIconWrapper->setPixmap(connectionStatusIcon_ERR_->pixmap(16,16));
        connectionStatusIconWrapper->setToolTip(tr("Disconnected from serial port."));
        QMessageBox::critical(this, tr("Error"), tr("Disconnected from serial port.\nError: %1").arg(toString(serialError)));
    }
    else if(!error && popup){
        if(serial->isOpen())serial->close();
        connectionStatusIconWrapper->setPixmap(connectionStatusIcon_NONE->pixmap(16,16));
        connectionStatusIconWrapper->setToolTip(tr("Not connected."));
        QMessageBox::information(this, tr("Notice"), tr("Disconnected from serial port."));
    }
    else {
        if(serial->isOpen())serial->close();
        connectionStatusIconWrapper->setPixmap(connectionStatusIcon_NONE->pixmap(16,16));
        connectionStatusIconWrapper->setToolTip(tr("Not connected."));
    }
}

//prompt the user for a sound file
void Soundboard::selectSound(int index) {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Audio File"), "", tr("Audio Files (*.wav *.mp3)"));
    if (QFile(fileName).exists()) {
        soundFiles[index] = fileName;
        sbWidget->setTableElement(index, fileName);
    }
    else{
        QMessageBox::critical(this, tr("Error: BadSoundError"), tr("File \"%1\" does not exist.").arg(fileName));
        soundFiles[index] = "";
        sbWidget->setTableElement(index, "");
    }
}

//a file was dropped on a button
void Soundboard::fileDropped(int index, const QString& fileName){
    if (QFile(fileName).exists()) {
        soundFiles[index] = fileName;
        sbWidget->setTableElement(index, fileName);
    }
    else{
        QMessageBox::critical(this, tr("Error: BadSoundError"), tr("File \"%1\" does not exist.").arg(fileName));
        soundFiles[index] = "";
        sbWidget->setTableElement(index, "");
    }
}

//serial data was recieved (if a serial port is open)
void Soundboard::receiveSerialData() {
    while (serial->canReadLine()) {
        //grab the serial data
        QString data = QString(serial->readLine().trimmed());
        for(int i=0;i<data.length(); i++){
            if(data[i] == '1'){//if 1, the button is pressed down
                if(oldSerialData[i] == '1' && s1[i] == '1' && s2[i] == '0'){//if the previous data for this index was 0, then this button was *JUST* pressed
                    // qDebug()<<oldSerialData<<" old:new "<<data<<" Playing sound!";
                    playSound((i+1)/2);//play the sound
                }
            }
        }
        //update the old serial data
        s2 = s1;
        s1 = oldSerialData;
        oldSerialData = data;
        // qDebug()<<"Recieved: "+data.toStdString();
    }
}

//send serial data to the device (if a serial port is open)
void Soundboard::sendSerialData(const QString &data) {
    if (serial->isOpen() && serial->isWritable()) {
        serial->write(data.toUtf8());
        // qDebug()<<"Sent: "+data.toUtf8();
    }
}

//play a sound
void Soundboard::playSound(int index) {
    //check if a sound is loaded
    if (!soundFiles[index].isEmpty()) {
        device1AudioOutput = new QAudioOutput(outputDevice1, this);
        device1AudioOutput->setVolume(scale(output1Volume));
        device2AudioOutput = new QAudioOutput(outputDevice2, this);
        device2AudioOutput->setVolume(scale(output2Volume));

        // qDebug()<<"outdev1: "<<outputDevice1.description();
        // qDebug()<<"outdev2: "<<outputDevice2.description();

        // qDebug()<<"outvol1: "<<scale(output1Volume);
        // qDebug()<<"outvol2: "<<scale(output2Volume);

        //play the sound and send the action to the device
        //map the sound to the correct led (button → LED):
        //1 → 10   3 → 9    5 → 8    7 → 7    9  → 6
        //2 → 1    4 → 2    6 → 3    8 → 4    10 → 5
        //then map the led to the correct string index for
        //the serial communication protocol.
        switch(index){
        case 0:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[18] = '1';
            sendSerialData(serialData);
            break;
        case 1:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[0] = '1';
            sendSerialData(serialData);
            break;
        case 2:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[16] = '1';
            sendSerialData(serialData);
            break;
        case 3:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[2] = '1';
            sendSerialData(serialData);
            break;
        case 4:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[14] = '1';
            sendSerialData(serialData);
            break;
        case 5:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[4] = '1';
            sendSerialData(serialData);
            break;
        case 6:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[12] = '1';
            sendSerialData(serialData);
            break;
        case 7:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[6] = '1';
            sendSerialData(serialData);
            break;
        case 8:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[10] = '1';
            sendSerialData(serialData);
            break;
        case 9:
            //play into device 1
            device1MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device1MediaPlayers[index]->setAudioOutput(device1AudioOutput);
            device1MediaPlayers[index]->play();

            //play into device 2
            device2MediaPlayers[index]->setSource(QUrl::fromLocalFile(soundFiles[index]));
            device2MediaPlayers[index]->setAudioOutput(device2AudioOutput);
            device2MediaPlayers[index]->play();

            //send serial data
            serialData[8] = '1';
            sendSerialData(serialData);
            break;
        default:
            //an invalid index was provided
            QMessageBox::critical(this, tr("Error"), tr("Invalid index supplied: %1").arg(index));
            return;
        }
    }
    //if there is no sound selected for this index
    else{
        //if the error is already open, bring it to the front and flash it
        if (errorBox != nullptr && errorBox->isVisible()) {
            errorBox->raise();
            errorBox->activateWindow();
            //play system alert sound
            QApplication::beep();

            //flash the window
            #ifdef Q_OS_WIN
                FLASHWINFO fi;
                fi.cbSize = sizeof(FLASHWINFO);
                fi.hwnd = (HWND)errorBox->winId();
                fi.dwFlags = FLASHW_ALL;  //flash both caption and taskbar
                fi.uCount = 6;            //number of flashes
                fi.dwTimeout = 75;        //speed of flashes
                FlashWindowEx(&fi);
            #endif //Q_OS_WIN

            return;
        }

        //otherwise, create a new error message box
        errorBox = new QMessageBox(QMessageBox::Critical, tr("Error"), tr("Select a sound before playing it! (right click/drag+drop)"), QMessageBox::Ok, this);

        //delete it when closed
        connect(errorBox, &QMessageBox::finished, this, [this]() { errorBox = nullptr; });

        //show the error box
        errorBox->show();
        errorBox->raise();
        errorBox->activateWindow();
    }
}

//triggered when a sound stops playing
void Soundboard::soundEnd(int index) {
    //send the action to the device after mapping
    //the sound to the correct led (button → LED):
    //1 → 10   3 → 9    5 → 8    7 → 7    9  → 6
    //2 → 1    4 → 2    6 → 3    8 → 4    10 → 5
    //then, map the led to the correct string index
    //for the serial communication protocol.
    switch(index){
    case 0:
        serialData[18] = '0';
        sendSerialData(serialData);
        break;
    case 1:
        serialData[0] = '0';
        sendSerialData(serialData);
        break;
    case 2:
        serialData[16] = '0';
        sendSerialData(serialData);
        break;
    case 3:
        serialData[2] = '0';
        sendSerialData(serialData);
        break;
    case 4:
        serialData[14] = '0';
        sendSerialData(serialData);
        break;
    case 5:
        serialData[4] = '0';
        sendSerialData(serialData);
        break;
    case 6:
        serialData[12] = '0';
        sendSerialData(serialData);
        break;
    case 7:
        serialData[6] = '0';
        sendSerialData(serialData);
        break;
    case 8:
        serialData[10] = '0';
        sendSerialData(serialData);
        break;
    case 9:
        serialData[8] = '0';
        sendSerialData(serialData);
        break;
    default:
        //an invalid index was provided
        QMessageBox::critical(this, tr("Error"), tr("Invalid index supplied: %1").arg(index));
        return;
    }
}

//save a configuration file
void Soundboard::saveConfig(bool exit) {
    //ask the user where to save the file
    QString fileName = loadedConfig;
    if((!exit || loadedConfig == "") || (exit && loadedConfig == "")) fileName = QFileDialog::getSaveFileName(this, tr("Save Config File"), "", tr("Config Files (*.json)"));
    if (!fileName.isEmpty()) {
        QJsonObject config;
        QJsonArray soundArray;

        //append all of the sounds
        for (const QString &file : std::as_const(soundFiles)) {
            soundArray.append(file);
        }

        config["GLOBAL_PROGRAM_VERSION"] = GLOBAL_PROGRAM_VERSION;

        config["sounds"] = soundArray;

        //add the serial port
        config["serialPort"] = portComboBox->currentData().toString();

        //add the device id's
        config["outputDevice1Id"] = QString::fromUtf8(output1ComboBox->currentData().value<QAudioDevice>().id());
        config["outputDevice2Id"] = QString::fromUtf8(output2ComboBox->currentData().value<QAudioDevice>().id());

        //add the input and output volumes
        config["output1Volume"] = output1Volume;
        config["output2Volume"] = output2Volume;

        //save the file
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(QJsonDocument(config).toJson());
            file.close();
            if(!exit) QMessageBox::information(this, tr("Success"), tr("Configuration saved successfully"));
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Failed to save configuration file"));
        }
    }
}

//load a configuration file
void Soundboard::loadConfig(bool initial) {
    //ask the user which file to load
    QString fileName;
    if(!initial)fileName = QFileDialog::getOpenFileName(this, tr("Open Config File"), "", tr("Config Files (*.json)"));
    else fileName = cfgToLoadAtStartup;
    if (!fileName.isEmpty()) {
        //create the file object
        QFile file(fileName);
        //attempt to read the file
        if (file.open(QIODevice::ReadOnly)) {
            //read the file as a json then close it
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();

            //check if the file is formatted properly then load all of its data
            if (doc.isObject()) {
                QJsonObject config = doc.object();
                //check the program version
                if(config.contains("GLOBAL_PROGRAM_VERSION")){
                    if(config["GLOBAL_PROGRAM_VERSION"] != GLOBAL_PROGRAM_VERSION){
                        //outdated user config
                        QMessageBox::critical(this, "Error: OutdatedConfigError", "The selected configuration file was created by a different version of this program and cannot be loaded.");
                        return;
                    }
                }
                else {
                    if(config.contains("outputDevice1Index") && config.contains("outputDevice2Index")){//refactor the configuration file
                        //add GLOBAL_PROGRAM_VERSION
                        config["GLOBAL_PROGRAM_VERSION"] = GLOBAL_PROGRAM_VERSION;

                        //change the device indices to id's
                        output1ComboBox->setCurrentIndex(config["outputDevice1Index"].toInt());
                        config["outputDevice1Id"] = QString::fromUtf8(output1ComboBox->currentData().value<QAudioDevice>().id());
                        config.remove("outputDevice1Index");

                        output2ComboBox->setCurrentIndex(config["outputDevice2Index"].toInt());
                        config["outputDevice2Id"] = QString::fromUtf8(output2ComboBox->currentData().value<QAudioDevice>().id());
                        config.remove("outputDevice2Index");

                        //write the file
                        file.open(QIODevice::WriteOnly);
                        file.write(QJsonDocument(config).toJson());
                        file.close();
                    }
                    else {
                        QMessageBox::critical(this, "Error: Missing GLOBAL_PROGRAM_VERSION", "The selected configuration file is missing GLOBAL_PROGRAM_VERSION.");
                        return;
                    }
                }

                //load the sounds
                if (config.contains("sounds") && config["sounds"].isArray()) {
                    QJsonArray soundArray = config["sounds"].toArray();
                    for (int i = 0; i < soundArray.size() && i < 10; ++i) {
                        //verify that the file exists
                        if(!QFile(soundArray[i].toString()).exists()){
                            QMessageBox::critical(this, tr("Error: BadSoundError"), tr("File \"%1\" does not exist.").arg(soundArray[i].toString()));
                            soundFiles[i] = "";
                            sbWidget->setTableElement(i, "");
                        }
                        else{
                            soundFiles[i] = soundArray[i].toString();
                            sbWidget->setTableElement(i, soundArray[i].toString());
                        }
                    }
                }
                else {
                    //alert user of incorrectly formatted configuration
                    QMessageBox::critical(this, tr("Error: NoSoundsError"), tr("Failed to parse configuration file %1").arg(fileName));
                    return;
                }

                //load the serial port
                if (config.contains("serialPort")) {

                    //refresh the serial ports
                    refreshSerialPorts();

                    QString savedPort = config["serialPort"].toString();
                    if (portComboBox->findData(savedPort) != -1) {
                        portComboBox->setCurrentIndex(portComboBox->findData(savedPort));
                    }
                    else qDebug()<<"Error loading serial port";
                }
                else {
                    //alert user of incorrectly formatted configuration
                    QMessageBox::critical(this, tr("Error: NoSerialError"), tr("Failed to parse configuration file %1").arg(fileName));
                    return;
                }

                //load input and output device id
                if(config.contains("outputDevice1Id") && config.contains("outputDevice2Id")){
                    QByteArray outputDevice1Id = config["outputDevice1Id"].toString().toUtf8();
                    output1Index = index(outputDevice1Id);
                    output1ComboBox->setCurrentIndex(output1Index);

                    QByteArray outputDevice2Id = config["outputDevice2Id"].toString().toUtf8();
                    output2Index = index(outputDevice2Id);
                    output2ComboBox->setCurrentIndex(output2Index);
                }
                else {
                    //alert user of incorrectly formatted configuration
                    QMessageBox::critical(this, tr("Error: NoDevIdError"), tr("Failed to parse configuration file %1").arg(fileName));
                    return;
                }

                //load the input and output volumes
                if(config.contains("output1Volume") && config.contains("output2Volume")){
                    output1Volume = config["output1Volume"].toInt();
                    output2Volume = config["output2Volume"].toInt();
                    if(!initial)output1VolumeSlider->setValue(output1Volume);
                    if(!initial)output2VolumeSlider->setValue(output2Volume);
                }
                else {
                    //alert user of incorrectly formatted configuration
                    QMessageBox::critical(this, tr("Error: NoVolInfoError"), tr("Failed to parse configuration file %1").arg(fileName));
                    return;
                }

                //notify the user if they were the one to load the config manually
                if(!initial) QMessageBox::information(this, tr("Success"), tr("Configuration loaded successfully"));

                loadedConfig = fileName;

                //connect to the serial port automatically if there is a com port
                if(portComboBox->count() > 0)connectToSerialPort(false);

                //update the list of known confiurations
                if(contains(fileName, knownConfigurations)){
                    //move the currently loaded one to the top
                    knownConfigurations.remove(index(fileName, knownConfigurations));
                }
                knownConfigurations.push_front(fileName);

                updateKnownConfigsMenu();
            }
            else {
                //alert user of incorrectly formatted configuration
                QMessageBox::critical(this, tr("Error: NoDocError"), tr("Failed to parse configuration file %1").arg(fileName));
                return;
            }
        }
        else {
            //alert user of error
            QMessageBox::critical(this, tr("Error"), tr("Failed to open configuration file %1").arg(fileName));
            return;
        }
    }
}

//save initialization data
void Soundboard::saveInitData() {
    //initialize the file object
    QString fileName = QCoreApplication::applicationDirPath()+"/init.json";
    QJsonObject config;
    QJsonArray cfgArray;

    //append the known configurations
    for (const QString &cfg : std::as_const(knownConfigurations)) {
        cfgArray.append(cfg);
    }

    //save program version
    config["GLOBAL_PROGRAM_VERSION"] = GLOBAL_PROGRAM_VERSION;

    //save various info about known configurations
    config["knownConfigs"] = cfgArray;
    config["loadCfgAtStartup"] = loadCfgAtStartup;
    config["cfgToLoadAtStartup"] = cfgToLoadAtStartup;
    config["saveCfgAtShutdown"] = saveCfgAtShutdown;
    config["startMinimized"] = startMinimized;

    //write the file
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(config).toJson());
        file.close();
        // qDebug()<<"init data saved!";
    }
    else {
        //alert the user of errors
        QMessageBox::critical(this, tr("Error"), tr("Failed to save initialization configuration file."));
    }

}

//load initialization data
void Soundboard::loadInitData(){
    //initialize file object
    QString initFilePath = "init.json";
    QFile initFile(initFilePath);
    bool err = false;
    //check if init.json exists
    if(initFile.exists()){
        //read the file
        initFile.open(QIODevice::ReadOnly);
        QJsonDocument doc = QJsonDocument::fromJson(initFile.readAll());
        initFile.close();
        QJsonObject initConfig = doc.object();

        //grab the known configurations
        if (initConfig.contains("knownConfigs") && initConfig["knownConfigs"].isArray()) {
            for (QJsonValueRef i:initConfig["knownConfigs"].toArray()) {
                knownConfigurations.push_back(i.toString());
            }
        }
        else {
            //alert user of incorrectly formatted configuration
            QMessageBox::critical(this, tr("Error: NoKnownConfigsError"), tr("Failed to parse configuration file \'init.config\'"));
            err = true;
        }

        //grab startup config state
        if (initConfig.contains("loadCfgAtStartup"))
            loadCfgAtStartup = initConfig["loadCfgAtStartup"].toBool();
        else {
            //alert user of incorrectly formatted configuration
            QMessageBox::critical(this, tr("Error: NoBoolStartupConfigError"), tr("Failed to parse configuration file \'init.config\'"));
            err = true;
        }

        //grab startup config file
        if(initConfig.contains("cfgToLoadAtStartup"))
            cfgToLoadAtStartup = initConfig["cfgToLoadAtStartup"].toString();
        else {
            //alert user of incorrectly formatted configuration
            QMessageBox::critical(this, tr("Error: NoStartupConfigError"), tr("Failed to parse configuration file \'init.config\'"));
            err = true;
        }

        //grab shutdown save config state
        if(initConfig.contains("saveCfgAtShutdown"))
            saveCfgAtShutdown = initConfig["saveCfgAtShutdown"].toBool();
        else {
            //alert user of incorrectly formatted configuration
            QMessageBox::critical(this, tr("Error: NoBoolShutdownSaveError"), tr("Failed to parse configuration file \'init.config\'"));
            err = true;
        }


        //grab start minimized state
        if(initConfig.contains("startMinimized"))
            startMinimized = initConfig["startMinimized"].toBool();
        else {
            //alert user of incorrectly formatted configuration
            QMessageBox::critical(this, tr("Error: NoStartupMinimizedError"), tr("Failed to parse configuration file \'init.config\'"));
            err = true;
        }

        //check the program version
        if(initConfig.contains("GLOBAL_PROGRAM_VERSION") && initConfig["GLOBAL_PROGRAM_VERSION"] != GLOBAL_PROGRAM_VERSION){
            //update the program version
            initConfig["GLOBAL_PROGRAM_VERSION"] = GLOBAL_PROGRAM_VERSION;

            //write the file
            initFile.open(QIODevice::WriteOnly);
            initFile.write(QJsonDocument(initConfig).toJson());
            initFile.close();

            //welcome the user to the new program version
            QMessageBox::information(this, tr("Post-Update Welcome"), tr("Welcome to version %1 of the USB Soundboard User App!\nThis update fixes the bug where your audio output devices would randomly switch around, and introduces small optimizations.\nEnjoy!").arg(GLOBAL_PROGRAM_VERSION));
        }
        else if(!initConfig.contains("GLOBAL_PROGRAM_VERSION")){
            if(!err) {
                //update the program version
                initConfig["GLOBAL_PROGRAM_VERSION"] = GLOBAL_PROGRAM_VERSION;

                //write the file
                initFile.open(QIODevice::WriteOnly);
                initFile.write(QJsonDocument(initConfig).toJson());
                initFile.close();

                //welcome the user to the new program version
                QMessageBox::information(this, tr("Post-Update Welcome"), tr("Welcome to version %1 of the USB Soundboard User App!\nThis update fixes the bug where your audio output devices would randomly switch around, and introduces small optimizations.\nEnjoy!").arg(GLOBAL_PROGRAM_VERSION));
            }
            else{
                QMessageBox::warning(this, "Warn: NoPgmVersion", "Initialization Configuration is missing GLOBAL_PROGRAM_VERSION!");
                err = true;
            }

        }
    }
    else{
        //if the initialization configuration doesn't exist, set user config loading info to defaults
        loadCfgAtStartup = false;
        cfgToLoadAtStartup = "NONE";
        saveCfgAtShutdown = false;
        startMinimized = false;
    }
    if(err)
        QMessageBox::critical(this, tr("Error: Initialization Configuration File Corrupted"), tr("Try deleting the file \'init.config\' and restarting the program. \nYour Initialization settings will be cleared."));
}

//stringify serial port errors. used for debugging purposes
QString Soundboard::toString(QSerialPort::SerialPortError i){
    switch(i){
    case 0:
        return "NoError";
    case 1:
        return "DeviceNotFoundError";
    case 2:
        return "PermissionError";
    case 3:
        return "OpenError";
    case 4:
        return "WriteError";
    case 5:
        return "ReadError";
    case 6:
        return "ResourceError";
    case 7:
        return "UnsupportedOperationError";
    case 8:
        return "UnknownError";
    case 9:
        return "TimeoutError";
    case 10:
        return "NotOpenError";
    default:
        qDebug()<<i;
        return "INVALID ENUM ERR";
    }
}

//extracts the file name from the path provided
QString Soundboard::extractFileName(const QString& path){
    for(int i=path.length()-1;i>=0;i--)
        if(path[i]=='/' && i!=path.length()-1)
            return QString::fromStdString(path.toStdString().substr(i+1));
    return "";
}

//checks if the provided list contains the provided string
bool Soundboard::contains(const QString& s, QStringList list){
    for(QString& i:list){
        if(i == s) return true;
    }
    return false;
}

//checks if the provided array of qactions contains an action named the provided string
bool Soundboard::contains(const QString& s, QList<QAction *> list){
    for(QAction *a:list){
        if(a->text() == s) return true;
    }
    return false;
}

//finds the provided string in the provied list. returns -1 if not found.
qsizetype Soundboard::index(const QString& s, QStringList list){
    for(int i = 0; i< list.size(); i++){
        if(list[i] == s) return i;
    }
    return -1;
}

//returns the action in the provided list named the provided string, or nullptr if not found
QAction* Soundboard::index(const QString& s, QList<QAction*> list){
    for(int i = 0; i< list.size(); i++){
        if(list[i]->text() == s) return list[i];
    }
    return nullptr;
}

//scales an integer (0-100) to a 0-1 scale
float Soundboard::scale(int v){
    return v/100.0f;
}

//the slot for selecting a configuration from the menu bar
void Soundboard::startupConfigSelected(const QString& cfg){
    if(cfg == "NONE"){
        loadCfgAtStartup = false;
        cfgToLoadAtStartup = "NONE";
        //update the list of known configurations in the menu bar
        updateKnownConfigsMenu();
        return;
    }
    else {
        loadCfgAtStartup = true;
        cfgToLoadAtStartup = cfg;
        //update the list of known configurations in the menu bar
        updateKnownConfigsMenu();
    }

    //prompt the user
    QMessageBox::StandardButton choice = QMessageBox::question(this, tr("Question"), tr("Do you want to load this configuration right now?"));

    if(choice == QMessageBox::Yes){
        loadConfig(true);
        output1VolumeSlider->setValue(output1Volume);
        output2VolumeSlider->setValue(output2Volume);
    }
}

//checks if the loaded configuration matches the selected settings
bool Soundboard::configChanged(){
    if(loadedConfig == "") return true;

    //ask the user which file to load
    QString fileName = loadedConfig;

    //create the file object
    QFile file(fileName);
    //attempt to read the file
    if (file.open(QIODevice::ReadOnly)) {
        //read the file as a json then close it
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        //check if the file is formatted properly then load all of its data
        if (doc.isObject()) {
            QJsonObject config = doc.object();
            //check the sounds
            if (config.contains("sounds") && config["sounds"].isArray()) {
                QJsonArray soundArray = config["sounds"].toArray();
                for (int i = 0; i < soundArray.size() && i < 10; ++i) {
                    //verify that the file exists
                    if(!QFile(soundArray[i].toString()).exists())
                        return true; //the config file is broken and needs to be overwritten
                    if(soundFiles[i] != soundArray[i].toString())
                        return true; //mismatch in sounds
                }
            }
            else return true; //the config file is broken and needs to be overwritten

            //check the serial port
            if (config.contains("serialPort")) {
                QString savedPort = config["serialPort"].toString();
                if(portComboBox->findData(savedPort) != portComboBox->currentIndex())
                    return true; //mismatch in port
            }
            else return true; //the config file is broken and needs to be overwritten

            //check input and output devices
            if(config.contains("outputDevice1Index") && config.contains("outputDevice2Index")){
                int outputDevice1Index = config["outputDevice1Index"].toInt();
                if((output1ComboBox->currentIndex() != outputDevice1Index))
                    return true; //mismatch in dev 1
                int outputDevice2Index = config["outputDevice2Index"].toInt();
                if((output2ComboBox->currentIndex() != outputDevice2Index))
                    return true; //mismatch in dev 2
            }
            else return true; //the config file is broken and needs to be overwritten

            //check the input and output volumes
            if(config.contains("output1Volume") && config.contains("output2Volume")){
                if(config["output1Volume"].toInt() != output1Volume)
                    return true; //mismatch in out1 vol
                if(config["output2Volume"].toInt() != output2Volume)
                    return true; //mismatch in out2 vol
            }
            else return true; //the config file is broken and needs to be overwritten
        }
        else return true; //the config file is broken and needs to be overwritten
    }
    else return true; //the config file is broken and needs to be overwritten
    //loaded configuration matches current settings
    return false;
}

//updates the known configurations in the menu bar
void Soundboard::updateKnownConfigsMenu(){
    //clear the menu
    startupConfigMenu->clear();
    startupConfigMenu->setToolTipsVisible(true);

    //re-add the option to not load any config at startup
    QAction *noStartupConfigAction = new QAction(tr("None"), this);
    noStartupConfigAction->setCheckable(true);
    noStartupConfigAction->setChecked(cfgToLoadAtStartup == "NONE");
    noStartupConfigAction->setToolTip(tr("Manually load a configuration file first to see it in this list!"));
    startupConfigMenu->addAction(noStartupConfigAction);
    connect(noStartupConfigAction, &QAction::triggered, this, [this](){
        startupConfigSelected("NONE");
    });

    //add the rest of the items of interest
    for(const QString &s:std::as_const(knownConfigurations)){
        QAction *action = new QAction(extractFileName(s), this);
        action->setCheckable(true);
        action->setChecked(cfgToLoadAtStartup == s);
        action->setToolTip(s);
        connect(action, &QAction::triggered, this, [this, s]() {
            startupConfigSelected(s);
        });
        startupConfigMenu->addAction(action);
    }
}

void Soundboard::populateAudioDevices() {
    //clear current contents of combo boxes
    output1ComboBox->clear();
    output2ComboBox->clear();

    //list available output devices
    outputDevices = QMediaDevices::audioOutputs();
    for (const auto &device : outputDevices) {
        QVariant variant = QVariant::fromValue(device);
        output1ComboBox->addItem(device.description(), variant);
        output2ComboBox->addItem(device.description(), variant);
    }

    //set the selected devices as the current combo box indices
    output1ComboBox->setCurrentIndex(output1Index);
    output2ComboBox->setCurrentIndex(output2Index);

}

//triggered whenever the volume slider is changed
void Soundboard::updateOutput1VolumeLabel(int value) {
    //update the label
    output1VolumeValueLabel->setText(QString("%1 %").arg(value));

    //update the parent class' volume value
    output1Volume = value;

    //update the parent class' audio output (actual) volume
    device1AudioOutput->setVolume(value);
}

//triggered whenever the volume slider is changed
void Soundboard::updateOutput2VolumeLabel(int value) {
    //update the label
    output2VolumeValueLabel->setText(QString("%1 %").arg(value));

    //update the parent class' volume value
    output2Volume = value;

    //update the parent class' audio output (actual) volume
    device2AudioOutput->setVolume(value);
}

//reset input volume to 0%
void Soundboard::resetOutput1Volume() {
    output1VolumeSlider->setValue(0);
    updateOutput1VolumeLabel(0);
}

//reset output volume to 0%
void Soundboard::resetOutput2Volume() {
    output2VolumeSlider->setValue(0);
    updateOutput2VolumeLabel(0);
}

//triggered when the user changes the selected audio device #1
void Soundboard::combo1Changed(int newIndex) {
    //check if the user tried to select the same device for both
    if(this->output2ComboBox->currentIndex() == newIndex){
        QMessageBox::critical(this, tr("Error"), tr("Please select two different audio devices"));
        if(this->output2ComboBox->currentIndex() != 0)
            this->output1ComboBox->setCurrentIndex(0);
        else this->output1ComboBox->setCurrentIndex(1);
    }
    //update the output device
    outputDevice1 = QMediaDevices::audioOutputs().at(output1ComboBox->currentIndex());
    //update the device index
    output1Index = output1ComboBox->currentIndex();
}

//triggered when the user changes the selected audio device #2
void Soundboard::combo2Changed(int newIndex) {
    //check if the user tried to select the same device for both
    if(this->output1ComboBox->currentIndex() == newIndex){
        QMessageBox::critical(this, tr("Error"), tr("Please select two different audio devices"));
        if(this->output1ComboBox->currentIndex() != 0)
            this->output2ComboBox->setCurrentIndex(0);
        else this->output2ComboBox->setCurrentIndex(1);
    }
    //update the output device
    outputDevice2 = QMediaDevices::audioOutputs().at(output2ComboBox->currentIndex());
    //update the device index
    output2Index = output2ComboBox->currentIndex();
}

//tells the user how to add this program to their computer's startup folder
void Soundboard::openStartupHelp(){
    startupHelpBox = new StartupHelp(this);
    startupHelpBox->exec();
}

int Soundboard::index(QByteArray deviceId){
    for (int i = 0; i < outputDevices.count(); ++i) {
        QAudioDevice device = outputDevices[i];
        if (device.id() == deviceId) {
            return i;
            break;
        }
    }
    return 0;
}

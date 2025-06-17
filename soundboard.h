#ifndef SOUNDBOARD_H
#define SOUNDBOARD_H

#include "soundboardwidget.h"
#include "startuphelp.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QCoreApplication>
#include <QSystemTrayIcon>
#include <QJsonDocument>
#include <QMediaDevices>
#include <QMediaPlayer>
#include <QApplication>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QComboBox>
#include <QMenuBar>
#include <QPointer>
#include <QThread>
#include <QObject>
#include <QFile>
#include <QMenu>

#ifdef Q_OS_WIN
#include <QSettings>
#include <windows.h>
#endif

#ifdef Q_OS_LINUX
#include <QStandardPaths>
#endif

class Soundboard : public QMainWindow
{
    Q_OBJECT
public:
    explicit Soundboard(QWidget *parent = nullptr);
    ~Soundboard();
    int output1Volume, output2Volume, output1Index = 0, output2Index = 1;
    bool startMinimized;
    QAudioDevice outputDevice1 = QMediaDevices::defaultAudioOutput(),
                 outputDevice2 = QMediaDevices::audioOutputs().at(1);
    QAudioOutput *device1AudioOutput, *device2AudioOutput;
    void publicAppExitPoint();

protected:
    void closeEvent(QCloseEvent *event) override; //triggers when the app is closed
    void changeEvent(QEvent *event) override; //triggers when the app is minimized

private slots:
    void refreshSerialPorts();
    void connectToSerialPort(bool);
    void disconnectSerialPort(bool, bool);
    void selectSound(int index);
    void fileDropped(int, const QString&);
    void receiveSerialData();
    void sendSerialData(const QString &);
    void playSound(int index);
    void soundEnd(int index);
    void saveConfig(bool);
    void loadConfig(bool);
    void saveInitData();
    void loadInitData();
    void showFromTray();
    void exitApp();
    void startupConfigSelected(const QString&);
    void populateAudioDevices();
    void updateOutput1VolumeLabel(int);
    void updateOutput2VolumeLabel(int);
    void resetOutput1Volume();
    void resetOutput2Volume();
    void combo1Changed(int);
    void combo2Changed(int);
    void openStartupHelp();
private:
    SoundboardWidget *sbWidget;
    StartupHelp *startupHelpBox;
    // const QList<qint32> baudRates = {300, 600, 750, 1200, 2400, 4800, 9600, 19200, 31250, 38400, 57600, 74880, 115200, 230400, 250000, 460800, 500000, 921600, 1000000, 2000000};//common baud rates to attempt for auto-discovery
    QStringList soundFiles = QStringList(10);
    QStringList knownConfigurations = QStringList();
    QString serialData, oldSerialData, s1, s2;
    QString cfgToLoadAtStartup, loadedConfig;
    QList<QMediaPlayer*> device1MediaPlayers, device2MediaPlayers;
    QSerialPort::SerialPortError serialError = QSerialPort::SerialPortError::NoError;
    QSerialPort *serial;
    QComboBox *portComboBox;
    QPointer<QMessageBox> errorBox = nullptr;
    QIcon *connectionStatusIcon_NONE, *connectionStatusIcon_TRUE, *connectionStatusIcon_ERR_;
    QLabel *connectionStatusIconWrapper;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu, *startupConfigMenu;
    QComboBox *output1ComboBox, *output2ComboBox;
    QSlider *output1VolumeSlider, *output2VolumeSlider;
    QLabel *output1VolumeValueLabel, *output2VolumeValueLabel;
    const int currentBaudRate = 115200;
    bool loadCfgAtStartup;
    bool saveCfgAtShutdown;

    QString toString(QSerialPort::SerialPortError);
    QString extractFileName(const QString&);
    bool contains(const QString&, QStringList);
    bool contains(const QString&, QList<QAction *>);
    bool configChanged();
    qsizetype index(const QString&, QStringList);
    QAction* index(const QString& , QList<QAction*>);
    void updateKnownConfigsMenu();
    float scale(int);
signals:
    void sendSerial(QString);
};

#endif // SOUNDBOARD_H

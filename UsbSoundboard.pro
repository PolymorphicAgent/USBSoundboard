QT += core gui
QT += multimedia
QT += serialport

win32: LIBS += -luser32


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = icon.ico

SOURCES += \
    droppablebutton.cpp \
    main.cpp \
    soundboard.cpp \
    startuphelp.cpp

HEADERS += \
    droppablebutton.h \
    soundboard.h \
    soundboardwidget.h \
    startuphelp.h

# INCLUDEPATH += $$PWD/libs/portaudio/include
# LIBS += -L$$PWD/libs/portaudio/lib -lportaudio_x64

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

FORMS += \
    Soundboard.ui

DISTFILES += \
    info.txt

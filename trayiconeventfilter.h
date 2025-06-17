#ifndef TRAYICONEVENTFILTER_H
#define TRAYICONEVENTFILTER_H

#include "soundboard.h"

#include <QAbstractNativeEventFilter>
#include <QApplication>
#ifdef Q_OS_WIN
    #include <windows.h>
#endif

class TrayIconEventFilter : public QAbstractNativeEventFilter
{
public:
    TrayIconEventFilter(Soundboard* sb);
    bool nativeEventFilter(const QByteArray&, void *, long long *) override;

private:
    Soundboard* soundboard;
};

#endif // TRAYICONEVENTFILTER_H

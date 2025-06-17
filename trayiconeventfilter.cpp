#include "trayiconeventfilter.h"

TrayIconEventFilter::TrayIconEventFilter(Soundboard* sb) {
    this->soundboard = sb;
}

bool TrayIconEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long long *result) {
#ifdef Q_OS_WIN
    Q_UNUSED(result);
    Q_UNUSED(eventType);
    MSG *msg = static_cast<MSG*>(message);
    if (msg->message == WM_MBUTTONDOWN) {
        // Here, you can invoke your exit logic
        // Note: You might need to ensure this event really targets your tray icon.
        // For example, you could emit a signal or call a global function.
        qDebug()<<"called";
    }
#endif
    return false;
}

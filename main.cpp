#include "soundboard.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_LOGGING_RULES", "qt.multimedia.ffmpeg=false");

    QApplication a(argc, argv);
    Soundboard soundboard;
    if(!soundboard.startMinimized)soundboard.show();
    return a.exec();
}


//TODO: style GUI (make it look pretty)
//TODO: prettify code

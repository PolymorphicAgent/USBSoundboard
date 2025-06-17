#include "DroppableButton.h"

DroppableButton::DroppableButton(QWidget *parent)
    : QPushButton(parent) {
    setAcceptDrops(true); //enable drag-and-drop
}

void DroppableButton::dragEnterEvent(QDragEnterEvent *event) {
    //accept drag if the payload contains URLs (files)
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void DroppableButton::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        const QList<QUrl> urls = mimeData->urls();
        if (!urls.isEmpty()) {
            QString filePath = urls.first().toLocalFile();

            // Validate file extension
            QFileInfo fileInfo(filePath);
            QString extension = fileInfo.suffix().toLower(); //get file extension

            //allow only specific audio file types
            if (extension == "mp3" || extension == "wav" || extension == "ogg") {
                emit fileDropped(filePath);
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Invalid file type. (.mp3, .wav, .ogg, ONLY)"));
            }
        }
    }
}

void DroppableButton::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::RightButton)
        emit rightClicked();
    QPushButton::mousePressEvent(e);
}

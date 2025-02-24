#ifndef DROPPABLEBUTTON_H
#define DROPPABLEBUTTON_H

#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QMimeData>
#include <QFileInfo>
#include <QString>

class DroppableButton : public QPushButton {
    Q_OBJECT

public:
    explicit DroppableButton(QWidget *parent = nullptr);

private slots:
    void mousePressEvent(QMouseEvent *e) override;

signals:
    void fileDropped(const QString &filePath);
    void rightClicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // DROPPABLEBUTTON_H

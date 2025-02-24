#ifndef SOUNDBOARDWIDGET_H
#define SOUNDBOARDWIDGET_H

#include <QObject>
#include <QWidget>

#include "ui_Soundboard.h"

class SoundboardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundboardWidget(QWidget *parent = nullptr) : QWidget(parent), ui(new Ui::SoundboardForm) {
        ui->setupUi(this);
        //connect button presses
        connect(ui->one,   &QPushButton::pressed,          this, [this](){emit buttonPressed(0);});
        connect(ui->two,   &QPushButton::pressed,          this, [this](){emit buttonPressed(1);});
        connect(ui->three, &QPushButton::pressed,          this, [this](){emit buttonPressed(2);});
        connect(ui->four,  &QPushButton::pressed,          this, [this](){emit buttonPressed(3);});
        connect(ui->five,  &QPushButton::pressed,          this, [this](){emit buttonPressed(4);});
        connect(ui->six,   &QPushButton::pressed,          this, [this](){emit buttonPressed(5);});
        connect(ui->seven, &QPushButton::pressed,          this, [this](){emit buttonPressed(6);});
        connect(ui->eight, &QPushButton::pressed,          this, [this](){emit buttonPressed(7);});
        connect(ui->nine,  &QPushButton::pressed,          this, [this](){emit buttonPressed(8);});
        connect(ui->ten,   &QPushButton::pressed,          this, [this](){emit buttonPressed(9);});
        //connect button right clicks
        connect(ui->one,   &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(0);});
        connect(ui->two,   &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(1);});
        connect(ui->three, &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(2);});
        connect(ui->four,  &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(3);});
        connect(ui->five,  &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(4);});
        connect(ui->six,   &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(5);});
        connect(ui->seven, &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(6);});
        connect(ui->eight, &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(7);});
        connect(ui->nine,  &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(8);});
        connect(ui->ten,   &DroppableButton::rightClicked, this, [this](){emit buttonRightClicked(9);});
        //connect file drops
        connect(ui->one,   &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(0, filePath);});
        connect(ui->two,   &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(1, filePath);});
        connect(ui->three, &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(2, filePath);});
        connect(ui->four,  &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(3, filePath);});
        connect(ui->five,  &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(4, filePath);});
        connect(ui->six,   &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(5, filePath);});
        connect(ui->seven, &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(6, filePath);});
        connect(ui->eight, &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(7, filePath);});
        connect(ui->nine,  &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(8, filePath);});
        connect(ui->ten,   &DroppableButton::fileDropped,  this, [this](const QString &filePath) {emit fileDropped(9, filePath);});

        //make the table read-only
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    ~SoundboardWidget() { delete ui; }

    void setTableElement(int index, QString text){
        ui->tableWidget->setItem(index, 0, new QTableWidgetItem(text));
        ui->tableWidget->item(index, 0)->setToolTip(text);
    }

    QString getTableElement(int index){
        return ui->tableWidget->item(index, 0)->text();
    }

private:
    Ui::SoundboardForm *ui;
signals:
    void buttonPressed(int);
    void buttonRightClicked(int);
    void fileDropped(int, const QString&);
};

#endif // SOUNDBOARDWIDGET_H

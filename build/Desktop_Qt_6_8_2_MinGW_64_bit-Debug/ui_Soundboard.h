/********************************************************************************
** Form generated from reading UI file 'Soundboard.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOUNDBOARD_H
#define UI_SOUNDBOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include <droppablebutton.h>

QT_BEGIN_NAMESPACE

class Ui_SoundboardForm
{
public:
    DroppableButton *one;
    DroppableButton *two;
    DroppableButton *three;
    DroppableButton *four;
    DroppableButton *five;
    DroppableButton *six;
    DroppableButton *seven;
    DroppableButton *eight;
    DroppableButton *nine;
    DroppableButton *ten;
    QFrame *border;
    QTableWidget *tableWidget;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *SoundboardForm)
    {
        if (SoundboardForm->objectName().isEmpty())
            SoundboardForm->setObjectName("SoundboardForm");
        SoundboardForm->resize(471, 459);
        SoundboardForm->setMinimumSize(QSize(471, 459));
        one = new DroppableButton(SoundboardForm);
        one->setObjectName("one");
        one->setGeometry(QRect(50, 50, 51, 51));
        one->setMinimumSize(QSize(51, 51));
        one->setMaximumSize(QSize(51, 51));
        QFont font;
        font.setFamilies({QString::fromUtf8("AcadEref")});
        font.setPointSize(20);
        one->setFont(font);
        one->setAcceptDrops(true);
        one->setAutoFillBackground(false);
        one->setAutoDefault(false);
        one->setFlat(false);
        two = new DroppableButton(SoundboardForm);
        two->setObjectName("two");
        two->setGeometry(QRect(50, 120, 51, 51));
        two->setMinimumSize(QSize(51, 51));
        two->setMaximumSize(QSize(51, 51));
        two->setFont(font);
        two->setAcceptDrops(true);
        two->setFlat(false);
        three = new DroppableButton(SoundboardForm);
        three->setObjectName("three");
        three->setGeometry(QRect(130, 50, 51, 51));
        three->setMinimumSize(QSize(51, 51));
        three->setMaximumSize(QSize(51, 51));
        three->setFont(font);
        three->setAcceptDrops(true);
        three->setFlat(false);
        four = new DroppableButton(SoundboardForm);
        four->setObjectName("four");
        four->setGeometry(QRect(130, 120, 51, 51));
        four->setMinimumSize(QSize(51, 51));
        four->setMaximumSize(QSize(51, 51));
        four->setFont(font);
        four->setAcceptDrops(true);
        four->setFlat(false);
        five = new DroppableButton(SoundboardForm);
        five->setObjectName("five");
        five->setGeometry(QRect(210, 50, 51, 51));
        five->setMinimumSize(QSize(51, 51));
        five->setMaximumSize(QSize(51, 51));
        five->setFont(font);
        five->setAcceptDrops(true);
        five->setFlat(false);
        six = new DroppableButton(SoundboardForm);
        six->setObjectName("six");
        six->setGeometry(QRect(210, 120, 51, 51));
        six->setMinimumSize(QSize(51, 51));
        six->setMaximumSize(QSize(51, 51));
        six->setFont(font);
        six->setAcceptDrops(true);
        six->setFlat(false);
        seven = new DroppableButton(SoundboardForm);
        seven->setObjectName("seven");
        seven->setGeometry(QRect(290, 50, 51, 51));
        seven->setMinimumSize(QSize(51, 51));
        seven->setMaximumSize(QSize(51, 51));
        seven->setFont(font);
        seven->setAcceptDrops(true);
        seven->setFlat(false);
        eight = new DroppableButton(SoundboardForm);
        eight->setObjectName("eight");
        eight->setGeometry(QRect(290, 120, 51, 51));
        eight->setMinimumSize(QSize(51, 51));
        eight->setMaximumSize(QSize(51, 51));
        eight->setFont(font);
        eight->setAcceptDrops(true);
        eight->setFlat(false);
        nine = new DroppableButton(SoundboardForm);
        nine->setObjectName("nine");
        nine->setGeometry(QRect(370, 50, 51, 51));
        nine->setMinimumSize(QSize(51, 51));
        nine->setMaximumSize(QSize(51, 51));
        nine->setFont(font);
        nine->setAcceptDrops(true);
        nine->setFlat(false);
        ten = new DroppableButton(SoundboardForm);
        ten->setObjectName("ten");
        ten->setGeometry(QRect(370, 120, 51, 51));
        ten->setMinimumSize(QSize(51, 51));
        ten->setMaximumSize(QSize(51, 51));
        ten->setFont(font);
        ten->setAcceptDrops(true);
        ten->setFlat(false);
        border = new QFrame(SoundboardForm);
        border->setObjectName("border");
        border->setGeometry(QRect(30, 10, 411, 181));
        border->setFrameShape(QFrame::Shape::WinPanel);
        border->setFrameShadow(QFrame::Shadow::Sunken);
        border->setLineWidth(5);
        border->setMidLineWidth(5);
        tableWidget = new QTableWidget(SoundboardForm);
        if (tableWidget->columnCount() < 1)
            tableWidget->setColumnCount(1);
        if (tableWidget->rowCount() < 10)
            tableWidget->setRowCount(10);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(10, 230, 451, 224));
        tableWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        tableWidget->setFrameShape(QFrame::Shape::WinPanel);
        tableWidget->setLineWidth(5);
        tableWidget->setMidLineWidth(5);
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        tableWidget->setAutoScroll(false);
        tableWidget->setAutoScrollMargin(0);
        tableWidget->setTabKeyNavigation(false);
        tableWidget->setProperty("showDropIndicator", QVariant(false));
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
        tableWidget->setShowGrid(true);
        tableWidget->setGridStyle(Qt::PenStyle::SolidLine);
        tableWidget->setWordWrap(false);
        tableWidget->setRowCount(10);
        tableWidget->setColumnCount(1);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->horizontalHeader()->setMinimumSectionSize(430);
        tableWidget->horizontalHeader()->setDefaultSectionSize(430);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setMinimumSectionSize(22);
        tableWidget->verticalHeader()->setDefaultSectionSize(22);
        tableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->verticalHeader()->setStretchLastSection(false);
        label = new QLabel(SoundboardForm);
        label->setObjectName("label");
        label->setGeometry(QRect(180, 200, 101, 20));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Artifakt Element")});
        font1.setPointSize(13);
        font1.setBold(true);
        font1.setUnderline(true);
        font1.setStrikeOut(false);
        font1.setKerning(true);
        label->setFont(font1);
        label->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label_2 = new QLabel(SoundboardForm);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(200, 10, 71, 31));
        label_2->setFont(font1);
        label_2->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        border->raise();
        tableWidget->raise();
        label->raise();
        label_2->raise();
        four->raise();
        five->raise();
        eight->raise();
        nine->raise();
        two->raise();
        one->raise();
        seven->raise();
        six->raise();
        three->raise();
        ten->raise();

        retranslateUi(SoundboardForm);

        one->setDefault(false);


        QMetaObject::connectSlotsByName(SoundboardForm);
    } // setupUi

    void retranslateUi(QWidget *SoundboardForm)
    {
        SoundboardForm->setWindowTitle(QCoreApplication::translate("SoundboardForm", "Form", nullptr));
#if QT_CONFIG(tooltip)
        one->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        one->setText(QCoreApplication::translate("SoundboardForm", "1", nullptr));
#if QT_CONFIG(tooltip)
        two->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        two->setText(QCoreApplication::translate("SoundboardForm", "2", nullptr));
#if QT_CONFIG(tooltip)
        three->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        three->setText(QCoreApplication::translate("SoundboardForm", "3", nullptr));
#if QT_CONFIG(tooltip)
        four->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        four->setText(QCoreApplication::translate("SoundboardForm", "4", nullptr));
#if QT_CONFIG(tooltip)
        five->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        five->setText(QCoreApplication::translate("SoundboardForm", "5", nullptr));
#if QT_CONFIG(tooltip)
        six->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        six->setText(QCoreApplication::translate("SoundboardForm", "6", nullptr));
#if QT_CONFIG(tooltip)
        seven->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        seven->setText(QCoreApplication::translate("SoundboardForm", "7", nullptr));
#if QT_CONFIG(tooltip)
        eight->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        eight->setText(QCoreApplication::translate("SoundboardForm", "8", nullptr));
#if QT_CONFIG(tooltip)
        nine->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        nine->setText(QCoreApplication::translate("SoundboardForm", "9", nullptr));
#if QT_CONFIG(tooltip)
        ten->setToolTip(QCoreApplication::translate("SoundboardForm", "Left click to play the sound, Right click to select a sound file, or drag and drop.", nullptr));
#endif // QT_CONFIG(tooltip)
        ten->setText(QCoreApplication::translate("SoundboardForm", "10", nullptr));
        label->setText(QCoreApplication::translate("SoundboardForm", "Sound Files", nullptr));
        label_2->setText(QCoreApplication::translate("SoundboardForm", "Buttons", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SoundboardForm: public Ui_SoundboardForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOUNDBOARD_H

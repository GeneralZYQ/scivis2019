/********************************************************************************
** Form generated from reading UI file 'QtGuiApplication1WTF.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGUIAPPLICATION1WTF_H
#define UI_QTGUIAPPLICATION1WTF_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGuiApplication1WTFClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLabel *label;
    QOpenGLWidget *openGLWidget;
    QWidget *barwidget;
    QRadioButton *radioButton;
    QRadioButton *grayscaleRadio;
    QRadioButton *blueToYellowRadio;
    QLabel *label_2;
    QRadioButton *heatMapRadio;
    QRadioButton *radioButton_256;
    QRadioButton *radioButton_128;
    QRadioButton *radioButton_64;
    QRadioButton *radioButton_32;
    QSlider *horizontalSlider_H;
    QLabel *label_3;
    QLabel *label_4;
    QSlider *horizontalSlider_S;
    QLabel *label_5;
    QSlider *horizontalSlider_V;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup_2;

    void setupUi(QMainWindow *QtGuiApplication1WTFClass)
    {
        if (QtGuiApplication1WTFClass->objectName().isEmpty())
            QtGuiApplication1WTFClass->setObjectName(QString::fromUtf8("QtGuiApplication1WTFClass"));
        QtGuiApplication1WTFClass->resize(751, 622);
        centralWidget = new QWidget(QtGuiApplication1WTFClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(640, 520, 75, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(640, 450, 81, 51));
        openGLWidget = new QOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(10, 10, 531, 511));
        barwidget = new QWidget(centralWidget);
        barwidget->setObjectName(QString::fromUtf8("barwidget"));
        barwidget->setGeometry(QRect(550, 10, 31, 471));
        radioButton = new QRadioButton(centralWidget);
        buttonGroup = new QButtonGroup(QtGuiApplication1WTFClass);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(radioButton);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(650, 40, 82, 17));
        grayscaleRadio = new QRadioButton(centralWidget);
        buttonGroup->addButton(grayscaleRadio);
        grayscaleRadio->setObjectName(QString::fromUtf8("grayscaleRadio"));
        grayscaleRadio->setGeometry(QRect(650, 60, 82, 17));
        blueToYellowRadio = new QRadioButton(centralWidget);
        buttonGroup->addButton(blueToYellowRadio);
        blueToYellowRadio->setObjectName(QString::fromUtf8("blueToYellowRadio"));
        blueToYellowRadio->setGeometry(QRect(650, 80, 91, 17));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(596, 10, 131, 21));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);
        heatMapRadio = new QRadioButton(centralWidget);
        buttonGroup->addButton(heatMapRadio);
        heatMapRadio->setObjectName(QString::fromUtf8("heatMapRadio"));
        heatMapRadio->setGeometry(QRect(650, 100, 82, 17));
        radioButton_256 = new QRadioButton(centralWidget);
        buttonGroup_2 = new QButtonGroup(QtGuiApplication1WTFClass);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(radioButton_256);
        radioButton_256->setObjectName(QString::fromUtf8("radioButton_256"));
        radioButton_256->setGeometry(QRect(590, 40, 82, 17));
        radioButton_128 = new QRadioButton(centralWidget);
        buttonGroup_2->addButton(radioButton_128);
        radioButton_128->setObjectName(QString::fromUtf8("radioButton_128"));
        radioButton_128->setGeometry(QRect(590, 60, 82, 17));
        radioButton_64 = new QRadioButton(centralWidget);
        buttonGroup_2->addButton(radioButton_64);
        radioButton_64->setObjectName(QString::fromUtf8("radioButton_64"));
        radioButton_64->setGeometry(QRect(590, 80, 82, 17));
        radioButton_32 = new QRadioButton(centralWidget);
        buttonGroup_2->addButton(radioButton_32);
        radioButton_32->setObjectName(QString::fromUtf8("radioButton_32"));
        radioButton_32->setGeometry(QRect(590, 100, 82, 17));
        horizontalSlider_H = new QSlider(centralWidget);
        horizontalSlider_H->setObjectName(QString::fromUtf8("horizontalSlider_H"));
        horizontalSlider_H->setGeometry(QRect(590, 140, 160, 19));
        horizontalSlider_H->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(600, 120, 131, 21));
        label_3->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(600, 170, 131, 21));
        label_4->setAlignment(Qt::AlignCenter);
        horizontalSlider_S = new QSlider(centralWidget);
        horizontalSlider_S->setObjectName(QString::fromUtf8("horizontalSlider_S"));
        horizontalSlider_S->setGeometry(QRect(590, 190, 160, 19));
        horizontalSlider_S->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(600, 220, 131, 21));
        label_5->setAlignment(Qt::AlignCenter);
        horizontalSlider_V = new QSlider(centralWidget);
        horizontalSlider_V->setObjectName(QString::fromUtf8("horizontalSlider_V"));
        horizontalSlider_V->setGeometry(QRect(590, 240, 160, 19));
        horizontalSlider_V->setOrientation(Qt::Horizontal);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(590, 160, 47, 13));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(720, 160, 47, 13));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(590, 210, 47, 13));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(720, 210, 47, 13));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(590, 260, 47, 13));
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(720, 260, 47, 13));
        QtGuiApplication1WTFClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtGuiApplication1WTFClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 751, 21));
        QtGuiApplication1WTFClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtGuiApplication1WTFClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtGuiApplication1WTFClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtGuiApplication1WTFClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtGuiApplication1WTFClass->setStatusBar(statusBar);

        retranslateUi(QtGuiApplication1WTFClass);

        QMetaObject::connectSlotsByName(QtGuiApplication1WTFClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtGuiApplication1WTFClass)
    {
        QtGuiApplication1WTFClass->setWindowTitle(QApplication::translate("QtGuiApplication1WTFClass", "QtGuiApplication1WTF", nullptr));
        pushButton->setText(QApplication::translate("QtGuiApplication1WTFClass", "PushButton", nullptr));
        label->setText(QApplication::translate("QtGuiApplication1WTFClass", "TextLabel", nullptr));
        radioButton->setText(QApplication::translate("QtGuiApplication1WTFClass", "rainbow", nullptr));
        grayscaleRadio->setText(QApplication::translate("QtGuiApplication1WTFClass", "grayscale", nullptr));
        blueToYellowRadio->setText(QApplication::translate("QtGuiApplication1WTFClass", "blue to yellow", nullptr));
        label_2->setText(QApplication::translate("QtGuiApplication1WTFClass", "Color Mapping", nullptr));
        heatMapRadio->setText(QApplication::translate("QtGuiApplication1WTFClass", "heat map", nullptr));
        radioButton_256->setText(QApplication::translate("QtGuiApplication1WTFClass", "256", nullptr));
        radioButton_128->setText(QApplication::translate("QtGuiApplication1WTFClass", "128", nullptr));
        radioButton_64->setText(QApplication::translate("QtGuiApplication1WTFClass", "64", nullptr));
        radioButton_32->setText(QApplication::translate("QtGuiApplication1WTFClass", "32", nullptr));
        label_3->setText(QApplication::translate("QtGuiApplication1WTFClass", "Hue", nullptr));
        label_4->setText(QApplication::translate("QtGuiApplication1WTFClass", "Saturation", nullptr));
        label_5->setText(QApplication::translate("QtGuiApplication1WTFClass", "V", nullptr));
        label_6->setText(QApplication::translate("QtGuiApplication1WTFClass", "- 0.5", nullptr));
        label_7->setText(QApplication::translate("QtGuiApplication1WTFClass", "+0.5", nullptr));
        label_8->setText(QApplication::translate("QtGuiApplication1WTFClass", " -0.5", nullptr));
        label_9->setText(QApplication::translate("QtGuiApplication1WTFClass", "+0.5", nullptr));
        label_10->setText(QApplication::translate("QtGuiApplication1WTFClass", " - 0.5", nullptr));
        label_11->setText(QApplication::translate("QtGuiApplication1WTFClass", "+0.5", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtGuiApplication1WTFClass: public Ui_QtGuiApplication1WTFClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGUIAPPLICATION1WTF_H

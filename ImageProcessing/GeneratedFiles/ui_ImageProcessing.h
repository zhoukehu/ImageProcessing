/********************************************************************************
** Form generated from reading UI file 'ImageProcessing.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEPROCESSING_H
#define UI_IMAGEPROCESSING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageProcessingClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageProcessingClass)
    {
        if (ImageProcessingClass->objectName().isEmpty())
            ImageProcessingClass->setObjectName(QStringLiteral("ImageProcessingClass"));
        ImageProcessingClass->resize(595, 400);
        centralWidget = new QWidget(ImageProcessingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ImageProcessingClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ImageProcessingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 595, 23));
        ImageProcessingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImageProcessingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ImageProcessingClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ImageProcessingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ImageProcessingClass->setStatusBar(statusBar);

        retranslateUi(ImageProcessingClass);

        QMetaObject::connectSlotsByName(ImageProcessingClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImageProcessingClass)
    {
        ImageProcessingClass->setWindowTitle(QApplication::translate("ImageProcessingClass", "ImageProcessing", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageProcessingClass: public Ui_ImageProcessingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEPROCESSING_H

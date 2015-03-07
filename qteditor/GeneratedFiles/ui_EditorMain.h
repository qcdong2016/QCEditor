/********************************************************************************
** Form generated from reading UI file 'EditorMain.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORMAIN_H
#define UI_EDITORMAIN_H

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

class Ui_EditorMainClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EditorMainClass)
    {
        if (EditorMainClass->objectName().isEmpty())
            EditorMainClass->setObjectName(QStringLiteral("EditorMainClass"));
        EditorMainClass->resize(600, 400);
        menuBar = new QMenuBar(EditorMainClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        EditorMainClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EditorMainClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        EditorMainClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(EditorMainClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        EditorMainClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(EditorMainClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EditorMainClass->setStatusBar(statusBar);

        retranslateUi(EditorMainClass);

        QMetaObject::connectSlotsByName(EditorMainClass);
    } // setupUi

    void retranslateUi(QMainWindow *EditorMainClass)
    {
        EditorMainClass->setWindowTitle(QApplication::translate("EditorMainClass", "EditorMain", 0));
    } // retranslateUi

};

namespace Ui {
    class EditorMainClass: public Ui_EditorMainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORMAIN_H

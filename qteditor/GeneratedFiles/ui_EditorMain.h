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
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorMainClass
{
public:
    QAction *action_Quit;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *widgetDock;
    QWidget *dockWidgetContents_2;
    QDockWidget *propDock;
    QWidget *dockWidgetContents_3;

    void setupUi(QMainWindow *EditorMainClass)
    {
        if (EditorMainClass->objectName().isEmpty())
            EditorMainClass->setObjectName(QStringLiteral("EditorMainClass"));
        EditorMainClass->resize(533, 404);
        action_Quit = new QAction(EditorMainClass);
        action_Quit->setObjectName(QStringLiteral("action_Quit"));
        centralWidget = new QWidget(EditorMainClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        EditorMainClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EditorMainClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 533, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        EditorMainClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EditorMainClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        EditorMainClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(EditorMainClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EditorMainClass->setStatusBar(statusBar);
        widgetDock = new QDockWidget(EditorMainClass);
        widgetDock->setObjectName(QStringLiteral("widgetDock"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        widgetDock->setWidget(dockWidgetContents_2);
        EditorMainClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), widgetDock);
        propDock = new QDockWidget(EditorMainClass);
        propDock->setObjectName(QStringLiteral("propDock"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        propDock->setWidget(dockWidgetContents_3);
        EditorMainClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), propDock);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(action_Quit);

        retranslateUi(EditorMainClass);

        QMetaObject::connectSlotsByName(EditorMainClass);
    } // setupUi

    void retranslateUi(QMainWindow *EditorMainClass)
    {
        EditorMainClass->setWindowTitle(QApplication::translate("EditorMainClass", "EditorMain", 0));
        action_Quit->setText(QApplication::translate("EditorMainClass", "&Quit", 0));
        menuFile->setTitle(QApplication::translate("EditorMainClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class EditorMainClass: public Ui_EditorMainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORMAIN_H

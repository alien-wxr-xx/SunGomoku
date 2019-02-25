/********************************************************************************
** Form generated from reading UI file 'xrroom.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XRROOM_H
#define UI_XRROOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_xrRoom
{
public:
    QAction *actionRestart;
    QAction *actionQuit;
    QAction *actionUndo;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QMenuBar *menubar;
    QMenu *menuOption;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *xrRoom)
    {
        if (xrRoom->objectName().isEmpty())
            xrRoom->setObjectName(QString::fromUtf8("xrRoom"));
        xrRoom->resize(640, 762);
        xrRoom->setMinimumSize(QSize(640, 762));
        actionRestart = new QAction(xrRoom);
        actionRestart->setObjectName(QString::fromUtf8("actionRestart"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/myimages/restartV2.icns"), QSize(), QIcon::Normal, QIcon::Off);
        actionRestart->setIcon(icon);
        actionRestart->setVisible(true);
        actionRestart->setIconVisibleInMenu(false);
        actionQuit = new QAction(xrRoom);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionUndo = new QAction(xrRoom);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/myimages/undoV2.icns"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon1);
        centralwidget = new QWidget(xrRoom);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMinimumSize(QSize(640, 640));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        xrRoom->setCentralWidget(centralwidget);
        menubar = new QMenuBar(xrRoom);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 640, 21));
        menuOption = new QMenu(menubar);
        menuOption->setObjectName(QString::fromUtf8("menuOption"));
        xrRoom->setMenuBar(menubar);
        statusbar = new QStatusBar(xrRoom);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        xrRoom->setStatusBar(statusbar);
        toolBar = new QToolBar(xrRoom);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setIconSize(QSize(64, 64));
        xrRoom->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuOption->menuAction());
        menuOption->addAction(actionUndo);
        menuOption->addAction(actionRestart);
        menuOption->addSeparator();
        menuOption->addAction(actionQuit);
        toolBar->addAction(actionUndo);
        toolBar->addAction(actionRestart);

        retranslateUi(xrRoom);

        QMetaObject::connectSlotsByName(xrRoom);
    } // setupUi

    void retranslateUi(QMainWindow *xrRoom)
    {
        xrRoom->setWindowTitle(QApplication::translate("xrRoom", "SunGomoku", nullptr));
        actionRestart->setText(QApplication::translate("xrRoom", "Restart", nullptr));
#ifndef QT_NO_STATUSTIP
        actionRestart->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_SHORTCUT
        actionRestart->setShortcut(QApplication::translate("xrRoom", "Ctrl+R", nullptr));
#endif // QT_NO_SHORTCUT
        actionQuit->setText(QApplication::translate("xrRoom", "Quit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("xrRoom", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionUndo->setText(QApplication::translate("xrRoom", "Undo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionUndo->setShortcut(QApplication::translate("xrRoom", "Ctrl+U", nullptr));
#endif // QT_NO_SHORTCUT
        menuOption->setTitle(QApplication::translate("xrRoom", "Option", nullptr));
        toolBar->setWindowTitle(QApplication::translate("xrRoom", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class xrRoom: public Ui_xrRoom {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XRROOM_H

#include "ui.h"
#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *File_Option;
    QAction *actionQuit;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuAction;
    QMenu *menuExit;
    QMenu *menut_h_n;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(587, 346);
        File_Option = new QAction(MainWindow);
        File_Option->setObjectName(QString::fromUtf8("File_Option"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/wallpaper.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        File_Option->setIcon(icon);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 587, 21));
        menuAction = new QMenu(menubar);
        menuAction->setObjectName(QString::fromUtf8("menuAction"));
        menuExit = new QMenu(menubar);
        menuExit->setObjectName(QString::fromUtf8("menuExit"));
        menut_h_n = new QMenu(menubar);
        menut_h_n->setObjectName(QString::fromUtf8("menut_h_n"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuAction->menuAction());
        menubar->addAction(menuExit->menuAction());
        menubar->addAction(menut_h_n->menuAction());
        menuAction->addAction(File_Option);
        menuExit->addAction(actionQuit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        File_Option->setText(QApplication::translate("MainWindow", "Add file...", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
        menuAction->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuExit->setTitle(QApplication::translate("MainWindow", "Exit", nullptr));
        menut_h_n->setTitle(QApplication::translate("MainWindow", "More", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H


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
    QAction *actionPause;
    QAction *actionContinue;
    QAction *actionHistogram;
    QAction *actionTime;
    QAction *actionChooseRoad;
    QAction *actionSave;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuAction;
    QMenu *menuExit;
    QMenu *pausemenu;
    QMenu *continuemenu;
    QMenu *histogram_menu;
    QMenu *time_menu;
    QMenu *road_menu;
    QMenu *save_menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(587, 346);

        // Set up actions

        File_Option = new QAction(MainWindow);
        File_Option->setObjectName(QString::fromUtf8("File_Option"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/wallpaper.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        File_Option->setIcon(icon);

        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionQuit->setShortcut(QKeySequence("Ctrl+W"));

        actionPause = new QAction(MainWindow);
        actionPause->setObjectName(QString::fromUtf8("actionPause"));

        actionContinue = new QAction(MainWindow);
        actionContinue->setObjectName(QString::fromUtf8("actionContinue"));

        actionHistogram = new QAction(MainWindow);
        actionHistogram->setObjectName(QString::fromUtf8("actionHistogram"));

        actionTime = new QAction(MainWindow);
        actionTime->setObjectName(QString::fromUtf8("actionTime"));

        actionChooseRoad = new QAction(MainWindow);
        actionChooseRoad->setObjectName(QString::fromUtf8("actionChooseRoad"));

        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));


        // Add menubar to the top of the window

        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 587, 21));

        // Add menus to the menubar

        menuAction = new QMenu(menubar);
        menuAction->setObjectName(QString::fromUtf8("menuAction"));
        menuExit = new QMenu(menubar);
        menuExit->setObjectName(QString::fromUtf8("menuExit"));
        pausemenu = new QMenu(menubar);
        pausemenu->setObjectName(QString::fromUtf8("pausemenu"));
        continuemenu = new QMenu(menubar);
        continuemenu->setObjectName(QString::fromUtf8("continuemenu"));
        histogram_menu = new QMenu(menubar);
        histogram_menu->setObjectName(QString::fromUtf8("histogram_menu"));
        time_menu = new QMenu(menubar);
        time_menu->setObjectName(QString::fromUtf8("time_menu"));
        road_menu = new QMenu(menubar);
        road_menu->setObjectName(QString::fromUtf8("road_menu"));
        save_menu = new QMenu(menubar);
        save_menu->setObjectName(QString::fromUtf8("save_menu"));

        // Set menubar visible
        MainWindow->setMenuBar(menubar);
        MainWindow->setStatusBar(statusbar);

        // Actions to menubar
        menubar->addAction(menuAction->menuAction());
        menubar->addAction(menuExit->menuAction());
        menubar->addAction(pausemenu->menuAction());
        menubar->addAction(continuemenu->menuAction());
        menubar->addAction(histogram_menu->menuAction());
        menubar->addAction(time_menu->menuAction());
        menubar->addAction(road_menu->menuAction());
        menubar->addAction(save_menu->menuAction());


        // Actions for submenus
        menuAction->addAction(File_Option);
        menuAction->addAction(actionPause);
        menuAction->addAction(actionContinue);
        menuAction->addAction(actionHistogram);
        menuAction->addAction(actionTime);
        menuAction->addAction(actionChooseRoad);
        menuAction->addAction(actionSave);
        menuExit->addAction(actionQuit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        File_Option->setText(QApplication::translate("MainWindow", "Change file...", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
        actionPause->setText(QApplication::translate("MainWindow", "Pause", nullptr));
        actionContinue->setText(QApplication::translate("MainWindow", "Continue", nullptr));
        actionHistogram->setText(QApplication::translate("MainWindow", "Histogram", nullptr));
        actionTime->setText(QApplication::translate("MainWindow", "Set time", nullptr));
        actionChooseRoad->setText(QApplication::translate("MainWindow", "Choose Road", nullptr));
        actionSave->setText(QApplication::translate("MainWindow", "Save as png..", nullptr));
        menuAction->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuExit->setTitle(QApplication::translate("MainWindow", "Exit", nullptr));


    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include "city.hpp"
#include "ui.h"
#include <QLabel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void createNodes();
    void createRoads();
    void createBuildings();
    void kellotus();
    void closeEvent(QCloseEvent *event);
    void histogram();
    void ask_save();
    void save_screen();
    void draw(std::vector<int>,int);

private slots:
    void on_actionQuit_triggered();
    void on_actionPause_triggered();
    void on_actionContinue_triggered();
    void on_File_Option_triggered();
    void on_actionHistogram_triggered();
    void on_actionTime_triggered();
    void on_actionChooseRoad_triggered();

private:
    int width_ = 1200;
    int height_ = 700;
    City* city_;
    QTimer t_;
    QLabel* dig_clock_;
    int flag_ = 1;
    std::string currentFile_ = "";

    // Histogrammia varten: ajajien määrä vektorissa
    std::vector<int> drivers_;
    QLabel* driver_count_;

    // Aikayksikkö 10 s ja lähdetään kello viidestä eli 5*6*60 = 1800
    // Lopetetaan simulaatio aamuyhteentoista
    int time_ = 1800;
};
#endif // MAINWINDOW_H

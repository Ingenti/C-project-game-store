#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:

    void on_actionQuit_triggered();

    void on_File_option_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
};
#endif // MAINWINDOW_H

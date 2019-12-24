#include "mainwindow.h"
#include "dialog.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <QFileDialog>
#include <QLineF>
#include <QPainter>
#include <QDebug>
#include <QDesktopWidget>
#include <QLabel>
#include <QColor>
#include <QTimer>
#include <QVector>
#include <QMenuBar>
#include <QStatusBar>
#include <QInputDialog>
#include <QDockWidget>
#include <QDateTime>
#include <QMessageBox>
#include <QPixmap>
#include <QScreen>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Ui_MainWindow is a helper class to build UI features such as menubar
    Ui_MainWindow().setupUi(this);


    // basic setup for window size etc.
    this->resize(width_, height_);
    this->setWindowTitle("Google Maps");

    dig_clock_ = new QLabel(this);
    driver_count_ = new QLabel(this);

    try{
    QString s = QInputDialog().getText(nullptr, "File", "Which .osm file do you want to open?");
    std::string filename = s.toLocal8Bit().constData();      // Tyyppimuunnos QStringistä std::stringiin
    currentFile_ = filename;
    city_ = new City();
    city_->Load_map(currentFile_.c_str());}
    catch(std::runtime_error)
    {
        QMessageBox mb(this);
        mb.critical(this, "Error", "No such file exists.");
        exit(EXIT_FAILURE);
    }
}


MainWindow::~MainWindow()
{
    delete this;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    driver_count_->setGeometry(this->size().width() -150, 30, 150, 100);
    dig_clock_->setGeometry(this->size().width()-150,10,100,100);
    if (flag_ == 1) {t_.singleShot(500, this, SLOT(update()));}
    //createNodes();
    createRoads();
    createBuildings();

    int total_drivers = city_->Loop(time_);
    if (total_drivers == -1) {qDebug() << "Simulation not working as intended."; exit(EXIT_FAILURE);}
    drivers_.push_back(total_drivers);
    driver_count_->setText(QString("Total drivers: ") + QString::number(total_drivers,10));

    time_ = time_ + 1;
    if(time_ >= 8640) {time_ = 0;}
    kellotus();
}


void MainWindow::createNodes()
{
   QPainter painter(this);
   for(auto node : city_->Nodes())
   {
        float lat = node.second->Location().first;
        float lon = node.second->Location().second;
        float relative_lat = (city_->Dim()[2] - lat ) / (city_->Dim()[2] - city_->Dim()[0] );
        float relative_lon = 1 - (city_->Dim()[3] - lon ) / (city_->Dim()[3] - city_->Dim()[1] );
        int x =  static_cast<int>(relative_lat) * this->size().width();
        int y =  static_cast<int>(relative_lon) * this->size().height();
        painter.setPen(QPen(Qt::blue, 10));
        painter.drawPoint(x,y);
        painter.setPen(QPen(Qt::green, 2));
    }
}


void MainWindow::createRoads()
{
    QPainter painter(this);
    for (auto road :  city_->Roads())
    {
        double usage = static_cast<double>(road->Use_percent());
        if(usage >= 0.95){painter.setPen(QPen(Qt::darkRed,6));}
        if(0.75 <= usage && usage < 1){painter.setPen(QPen(QColor(0+255,255-255,0),6));}
        if(0.5 <= usage && usage < 0.75){painter.setPen(QPen(QColor(0+255*2/3,255-255*2/3,0),6));}
        if(0.25 <= usage && usage < 0.5){painter.setPen(QPen(QColor(0+255*1/3,255-255*1/3,0),6));}
        if(0 <= usage && usage < 0.25){painter.setPen(QPen(QColor(0,255,0),6));}
        if (road == city_->road_){painter.setPen(QPen(Qt::cyan, 10));}  //highlighted road

        float source_lat = road->Source()->Location().first;
        float source_lon = road->Source()->Location().second;
        float dest_lat = road->Destination()->Location().first;
        float dest_lon = road->Destination()->Location().second;
        float rl_source_lat = (city_->Dim()[2] - source_lat ) / (city_->Dim()[2] - city_->Dim()[0] );
        float rl_source_lon = ( (city_->Dim()[3] - source_lon ) / (city_->Dim()[3] - city_->Dim()[1] ) );
        float rl_dest_lat = (city_->Dim()[2] - dest_lat ) / (city_->Dim()[2] - city_->Dim()[0] );
        float rl_dest_lon = ( (city_->Dim()[3] - dest_lon ) / (city_->Dim()[3] - city_->Dim()[1] ) );
        int s_y =  static_cast<int>((rl_source_lat) * this->size().height());
        int s_x =  static_cast<int>(this->size().width() - ((rl_source_lon) * this->size().width()));
        int d_y =  static_cast<int>((rl_dest_lat) * this->size().height());
        int d_x =  static_cast<int>(this->size().width() - ((rl_dest_lon) * this->size().width()));
        if(road->Side()) { painter.drawLine((QPoint(s_x, s_y)), QPoint(d_x, d_y)); }
        else { painter.drawLine(QPoint(s_x + 15, s_y + 15), QPoint(d_x + 15, d_y + 15)); }
    }
}

void MainWindow::createBuildings()
{
    QPainter painter(this);
    painter.setBrush((Qt::SolidPattern));
    for(auto b : city_->Buildings())
    {
        QVector<QPointF> points;
        for(auto c : b.second->Corners())
        {
            float lat = city_->Nodes()[c]->Location().first;
            float lon = city_->Nodes()[c]->Location().second;
            float relative_lat = (city_->Dim()[2] - lat ) / (city_->Dim()[2] - city_->Dim()[0] );
            float relative_lon = 1 - (city_->Dim()[3] - lon ) / (city_->Dim()[3] - city_->Dim()[1] );
            float y =  ((relative_lat) * this->size().height());
            float x =  ((relative_lon) * this->size().width());
            points.push_front(QPointF(static_cast<double>(x),static_cast<double>(y)));
        }
    if(b.second->Type() == 0) {painter.setBrush(Qt::yellow); } if(b.second->Type() == 1) { painter.setBrush(Qt::magenta); } if(b.second->Type() == 2) { painter.setBrush(Qt::gray); }
        painter.drawPolygon(points, Qt::WindingFill);
    }
}


void MainWindow::kellotus()
{
    int h = time_*10/3600;              //tunnit
    int m = (time_*10/60) - h*60;       //minuutit
    QString text = QString::number(h) + ":" + QString::number(m);
    if (m < 10){text = QString::number(h) + ":0" + QString::number(m);}
    dig_clock_->setText(text);
}


// Overloaded window closing
void MainWindow::closeEvent(QCloseEvent *)
{
    ask_save();
    exit(EXIT_SUCCESS);
}

// Menubar actions

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_File_Option_triggered()
{
    QString qs = QInputDialog().getText(this, "New file", "Enter filename:");
    std::string fn = qs.toLocal8Bit().constData();      // Tyyppimuunnos QStringistä std::stringiin
    std::ifstream file(fn);
        if (file)
        {
            city_ = new City();
            city_->Load_map(fn.c_str());
            currentFile_ = fn;
        }
        else
        {
            QMessageBox mb(this);
            mb.critical(nullptr, "Error","File not found.");
            file.close();
        }
}

void MainWindow::on_actionPause_triggered()
{
    if (flag_ == 1)
        flag_ = 0;
}

void MainWindow::on_actionContinue_triggered()
{
    if (flag_ == 0)
        flag_ = 1;
}

void MainWindow::on_actionTime_triggered()
{
    QString s = QInputDialog().getText(this, "Set time", "Set time (as decaseconds from midnight):");
    std::string new_time = s.toLocal8Bit().constData();      // Tyyppimuunnos QStringistä std::stringiin
    try {
        time_ = stoi(new_time);
    } catch (std::invalid_argument) {
        QMessageBox mb(this);
        mb.critical(this, "Bad input", "Non-numeric or too large input.");
    }
}

// Raportoi kaikkien kaupungissa liikkeellä olleiden autojen lukumäärän keskiarvon jokaiselta kuluneelta tunnilta
// ja piirtää kuvaajan, jonka voi tallentaa.
void MainWindow::on_actionHistogram_triggered()
{
    std::vector<int> mean_drivers;
    double sum = 0;
    int k = 0;
    for (auto driver : drivers_)
    {
        k++;
        sum = sum + driver;
        if (k >= 360)           // 10s aikavälein; 360 näytettä tunnissa
        {
            //reset loop, divide by amount of roads in the map
            mean_drivers.push_back(static_cast<int>(sum/city_->Visible().size()));
            sum = 0;
            k = 0;
        }
    }
    int h = time_*10/3600;
    draw(mean_drivers, h);
}

void MainWindow::ask_save()
{
    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(nullptr, "Save", "Do you want to save this map?");
    if(answer == QMessageBox::Yes) {save_screen();}
}

void MainWindow::save_screen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pm = screen->grabWindow(this->winId());
    bool b = pm.save("window.png");
    QMessageBox mb(this);
    if (b) {mb.setText("Screen successfully saved."); mb.exec();}
    else {mb.critical(nullptr, "Error", "Screen was not saved.");}
}

void MainWindow::on_actionChooseRoad_triggered()
{
    std::set<Road*> roads = city_->Roads();
    QString s = QInputDialog().getText(this, "Specify a road with an index", "Which road do you want to analyze? There are " + QString::number(city_->Visible().size()) + " roads total.");
    std::string road = s.toLocal8Bit().constData();
    try {
        int road_index = stoi(road) - 1;
        Road* r = city_->Visible()[static_cast<size_t>(road_index)];
        city_->road_ = r;
        QMessageBox mb(this);
        mb.setText("Current traffic for this road: "+ QString::number(r->Total_drivers(), 10));
        mb.exec();
        std::vector<int> drivers = r->Data();
        std::vector<int> mean_drivers;
        int sum = 0;
        int k = 0;
        for (auto driver : drivers)
        {
            k++;
            sum = sum + driver;
            if (k >= 360)           // 10s aikavälein; 360 näytettä tunnissa
            {
                //reset loop, divide by amount of (unique_drivers_ in road.cpp) samples taken. 360 in an hour because each time paintEvent ticks the time increases by 1
                mean_drivers.push_back(sum/360);
                sum = 0;
                k = 0;
            }
        }
        int h = time_*10/3600;
        draw(mean_drivers, h);

    } catch (std::invalid_argument) {
        QMessageBox mb(this);
        mb.critical(nullptr, "Bad input", "Non-numeric or too large input.");
    }
}

void MainWindow::draw(std::vector<int> drivers, int h)
{
    Dialog dialogNew(drivers,h);
    dialogNew.setModal(true);
    dialogNew.exec();
}

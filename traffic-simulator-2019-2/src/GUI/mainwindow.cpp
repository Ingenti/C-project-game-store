#include "mainwindow.h"
#include "ui_mainwindow.h"
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


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // basic setup for window size etc.
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    createNodes();
    createRoads();

    QMainWindow::paintEvent(event);
}


void MainWindow::createNodes()
{
    QPainter painter(this);

    /*
    std::map<long, Node*> kaikki_solmut = city->Nodes()
    for(auto map_itr = kaikki_solmut.begin(); map_itr != kaikki_solmut.end(); map_itr++)
    {
        std::pair<float,float> koordinaatit = map_itr.second->Coordinates()
        city_layout.push_back(koordinaatit);
    }
    */
    // NOTE: Skaalaa ikkunan koon mukaan!

    //testi
    std::vector<std::pair<float,float>> city_layout;
    std::pair<float, float> ff = std::make_pair(100.3*4, 30*10.8);
    city_layout.push_back(ff);
    city_layout.push_back(std::make_pair(89.3, 75.4));
    city_layout.push_back(std::make_pair(200, 600));
    city_layout.push_back(std::make_pair(600, 200));
    city_layout.push_back(std::make_pair(400, 600));
    city_layout.push_back(std::make_pair(100, 10));

    for(auto node = city_layout.begin(); node != city_layout.end(); node++)
    {
        painter.setPen(QPen(Qt::blue, 10));
        painter.drawPoint((*node).second, (*node).first);
        painter.setPen(QPen(Qt::green, 2));
    }
}


void MainWindow::createRoads()
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::green, 2));

    //testi
    std::vector<std::pair<float, float>> roads;
    roads.push_back(std::make_pair(89.3,75.4));
    roads.push_back(std::make_pair(400,600));
    roads.push_back(std::make_pair(100, 10));
    roads.push_back(std::make_pair(600, 200));

    //for(auto j : city->Roads())
    for (auto j = roads.begin(); j != roads.end();j++)
    {
             //Node* lähtöpaikka = j->Source()     Haetaan tien lähtö ja loppusolmut
             //Node* määränpää = j->Destination()
             //lpk = lähtöpaikka->Coordinates()   Haetaan lähtö ja loppusolmujen koordinaatit
             //mpk = määränpää->Coordinates()      eli siis tie on näiden kahden pair<float,float> välillä
            if (((j+1)) != roads.end())
            {
            painter.drawLine(QPoint((*j).second, (*j).first), QPoint((*(j+1)).second, (*(j+1)).first));
            // toinen tie, modifioidaan koordinaatteja vakiolla (esim. 5)
            painter.drawLine(QPoint((*j).second-5, (*j).first-5), QPoint((*(j+1)).second-5, (*(j+1)).first-5));
            }
    }
}

void MainWindow::createBuildings()
{

}


// Menubar actions

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_File_option_triggered()
{

}

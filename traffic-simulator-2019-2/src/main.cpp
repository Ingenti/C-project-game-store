#include "mainwindow.h"
#include "../lib/rapidxml-1.13/rapidxml_utils.hpp"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = simulation

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

HEADERS = \
   $$PWD/building.hpp \
   $$PWD/car.hpp \
   $$PWD/city.hpp \
   $$PWD/mainwindow.h \
   $$PWD/node.hpp \
   $$PWD/road.hpp \
   $$PWD/dialog.h \
   ../lib/rapidxml-1.13/rapidxml.hpp \
   ../lib/rapidxml-1.13/rapidxml_iterators.hpp \
   ../lib/rapidxml-1.13/rapidxml_print.hpp \
   ../lib/rapidxml-1.13/rapidxml_utils.hpp \
   ui.h \
   ui_dialog.h

SOURCES = \
   $$PWD/car.cpp \
   $$PWD/city.cpp \
   $$PWD/main.cpp \
   $$PWD/mainwindow.cpp \
   $$PWD/road.cpp \
   $$PWD/dialog.cpp \
   

INCLUDEPATH = \
    $$PWD/.

#DEFINES = 

DISTFILES += \
    ../lib/rapidxml-1.13/license.txt \
    ../lib/rapidxml-1.13/manual.html
RESOURCES += \
    map7.osm

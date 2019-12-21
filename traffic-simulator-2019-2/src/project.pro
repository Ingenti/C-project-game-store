CONFIG += c++11
HEADERS += mainwindow.h
SOURCES += mainwindow.cpp
SOURCES += main.cpp
SOURCES += city.cpp
HEADERS += city.hpp
SOURCES += car.cpp
HEADERS += car.hpp
SOURCES += road.cpp
HEADERS += road.hpp
HEADERS += node.hpp
HEADERS += building.hpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

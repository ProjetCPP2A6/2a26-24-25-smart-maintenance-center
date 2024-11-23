# Project name and Qt modules
QT += widgets
QT += sql
QT += core gui widgets printsupport
QT += network
CONFIG += c++17
CONFIG += c++11
TEMPLATE = app

# Project sources
SOURCES += main.cpp \
           connection.cpp \
           maintenance.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h \
    connection.h \
    maintenance.h

FORMS += mainwindow.ui

# Resource files
RESOURCES += ressouces.qrc



# Deployment paths
DESTDIR = $$OUT_PWD
INSTALLS += target

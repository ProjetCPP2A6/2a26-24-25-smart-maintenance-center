# Project name and Qt modules
QT += widgets
QT += sql
QT += core gui widgets printsupport
QT += network
QT += serialport
CONFIG += c++17
CONFIG += c++11
TEMPLATE = app

# Project sources
SOURCES += main.cpp \
           assiduite.cpp \
           authentification.cpp \
           connection.cpp \
           maintenance.cpp \
           mainwindow.cpp \
           perso.cpp

HEADERS += mainwindow.h \
    assiduite.h \
    authentification.h \
    connection.h \
    maintenance.h \
    perso.h

FORMS += mainwindow.ui

# Resource files
RESOURCES += ressouces.qrc



# Deployment paths
DESTDIR = $$OUT_PWD
INSTALLS += target

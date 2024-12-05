# Project name and Qt modules
QT += widgets sql
QT += sql
QT += core gui serialport  # Add serialport module


CONFIG += c++17
TARGET = smart_maintenance_center
TEMPLATE = app


# Project sources
SOURCES += main.cpp \
           assiduite.cpp \
           authentification.cpp \
           connection.cpp \
           mainwindow.cpp \
           perso.cpp
HEADERS += mainwindow.h \
    assiduite.h \
    authentification.h \
    connection.h \
    perso.h


# Resource files
RESOURCES += ressouces.qrc
QT += core sql
FORMS += mainwindow.ui
# Deployment paths
DESTDIR = $$OUT_PWD
target.path = /path/to/install/directory
INSTALLS += target
# Ajoutez ces lignes à votre fichier .pro
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


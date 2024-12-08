# Project name and Qt modules
QT += charts

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
           dialog_statistiques.cpp \
           equipement.cpp \
           maintenance.cpp \
           mainwindow.cpp \
           perso.cpp \
           smtp.cpp

HEADERS += mainwindow.h \
    assiduite.h \
    authentification.h \
    connection.h \
    dialog_statistiques.h \
    equipement.h \
    maintenance.h \
    perso.h \
    smtp.h

FORMS += mainwindow.ui \
    dialog_statistiques.ui

# Resource files
RESOURCES += ressouces.qrc



# Deployment paths
DESTDIR = $$OUT_PWD
INSTALLS += target

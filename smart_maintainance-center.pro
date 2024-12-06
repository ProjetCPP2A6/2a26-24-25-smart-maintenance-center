QT += sql
QT += charts
QT += widgets
QT += core gui serialport
CONFIG += c++17
CONFIG+= console
TEMPLATE = app

# Project sources
SOURCES += main.cpp \
           connection.cpp \
           mainwindow.cpp \
           ressources.cpp

HEADERS += mainwindow.h \
    connection.h \
    ressources.h

FORMS += mainwindow.ui

# Resource files
RESOURCES += ressouces.qrc



# Deployment paths
DESTDIR = $$OUT_PWD

target.path = $$DESTDIR
INSTALLS += target

# Project name and Qt modules
QT += widgets sql
QT += sql

CONFIG += c++17
TEMPLATE = app


# Project sources
SOURCES += main.cpp \
           aasiduite.cpp \
           connection.cpp \
           mainwindow.cpp \
           perso.cpp

HEADERS += mainwindow.h \
    aasiduite.h \
    connection.h \
    perso.h

FORMS += mainwindow.ui

# Resource files
RESOURCES += ressouces.qrc



# Deployment paths
DESTDIR = $$OUT_PWD
target.path = /path/to/install/directory
INSTALLS += target
# Ajoutez ces lignes à votre fichier .pro
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


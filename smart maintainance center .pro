# Project name and Qt modules
QT += widgets
QT += printsupport
TEMPLATE = app
QT += sql
QT += charts
QT += core gui charts
QT += serialport
# Project sources
SOURCES += main.cpp \
           analysefacture.cpp \
           connection.cpp \
           factures.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h \
    analysefacture.h \
    connection.h \
    factures.h

FORMS += mainwindow.ui

# Resource files
RESOURCES += ressouces.qrc



# Deployment paths
DESTDIR = $$OUT_PWD
INSTALLS += target

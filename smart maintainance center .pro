# Project name and Qt modules
QT += widgets
CONFIG += c++17
TEMPLATE = app

# Project sources
SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

# Resource files
RESOURCES += ressouces.qrc



# Deployment paths
DESTDIR = $$OUT_PWD
INSTALLS += target

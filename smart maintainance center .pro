# Project name and Qt modules
QT += widgets sql
CONFIG += c++17
TEMPLATE = app

# Project sources
SOURCES += main.cpp \
           connection.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h \
    connection.h

FORMS += mainwindow.ui

# Resource files
RESOURCES += ressouces.qrc



# Deployment paths
DESTDIR = $$OUT_PWD
INSTALLS += target

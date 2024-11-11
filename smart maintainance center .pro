# Project name and Qt modules
QT       += core gui sql printsupport multimedia charts multimediawidgets printsupport widgets
CONFIG += c++17
TEMPLATE = app

# Project sources
SOURCES += main.cpp \
           connection.cpp \
           dialog_statistiques.cpp \
           equipement.cpp \
           mainwindow.cpp \
           smtp.cpp

HEADERS += mainwindow.h \
    connection.h \
    dialog_statistiques.h \
    equipement.h \
    smtp.h

FORMS += mainwindow.ui \
    dialog_statistiques.ui

# Resource files
RESOURCES += ressouces.qrc



# Deployment paths
DESTDIR = $$OUT_PWD
INSTALLS += target

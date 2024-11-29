#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPalette palette = app.palette();
        palette.setColor(QPalette::Text, Qt::blue);
        app.setPalette(palette);

    MainWindow w;
    w.show();
    return app.exec();
}

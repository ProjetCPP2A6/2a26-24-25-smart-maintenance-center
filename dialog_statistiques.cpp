#include "dialog_statistiques.h"
#include "ui_dialog_statistiques.h"
#include "equipement.h"
#include "mainwindow.h"

Dialog_Statistiques::Dialog_Statistiques(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Statistiques)
{
    ui->setupUi(this);
}

Dialog_Statistiques::~Dialog_Statistiques()
{
    delete ui;
}


void Dialog_Statistiques::choix_pie()
{
    QSqlQuery q1,q2,q3,q4;
    qreal tot=0,c1=0,c2=0,c3=0;
    q1.prepare("SELECT * FROM EQUIPEMENTS");
    q1.exec();
    q2.prepare("SELECT * FROM EQUIPEMENTS WHERE STATUS='Actif';");
    q2.exec();
    q3.prepare("SELECT * FROM EQUIPEMENTS WHERE STATUS='En maintenance';");
    q3.exec();
    q4.prepare("SELECT * FROM EQUIPEMENTS WHERE STATUS='Hors service'");
    q4.exec();
    while (q1.next()){tot++;}
    while (q2.next()){c1++;}
    while (q3.next()){c2++;}
    while (q4.next()){c3++;}
    c1=c1/tot;
    c2=c2/tot;
    c3=c3/tot;
    QPieSeries *series = new QPieSeries();
    series->append("Actif",c1);
    series->append("En maintenance",c2);
    series->append("Hors service",c3);
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->show();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeBlueIcy);
    chartView = new QChartView(chart,ui->label_stats);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(500,500);
    chartView->show();
}

void Dialog_Statistiques::on_pushButton_Fermer_clicked()
{
    close();
}

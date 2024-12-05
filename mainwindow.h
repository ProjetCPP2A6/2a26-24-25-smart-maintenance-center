#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include "perso.h"
#include "assiduite.h"
#include "authentification.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();
    void on_close_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_17_clicked();
    void on_pushButton_18_clicked();
    void on_home_clicked();
    void on_close_2_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_21_clicked();
    void on_pb_Ajouter_clicked();
    void on_pb_modifier_clicked();
    void on_Supprimer_clicked();
    void afficherPersonnel();
    void on_pushButton_25_clicked();
    void on_p_trier_clicked();
    void on_trier_age_clicked();
    void on_rechercher_clicked();
    void on_consulter_clicked();
    void on_afficherabsc_clicked();
    void on_enregistrer_clicked();
    void on_stat_2_clicked();
    void readFromArduino();


void closeEvent(QCloseEvent *event);
    void on_readbutton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    QSqlDatabase db;
    Perso *persoManager;
    Assiduite *assiduiteManager;
    bool cond = false;
    //void connectDB();
    void insertIntoDatabase(const QString &numCarte, const QString &cin);
};

#endif // MAINWINDOW_H

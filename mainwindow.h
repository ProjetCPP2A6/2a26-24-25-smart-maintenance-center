#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QDebug>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_home_clicked();
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
    void on_close_2_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_21_clicked();

    void afficherPersonnel(); // Ensure this is declared


    void on_Supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_pb_Ajouter_clicked();

private:
    Ui::MainWindow *ui;
    bool cond; // Déplacer ici
    QStandardItemModel *PersonnelModel; // Déplacer ici

};

#endif // MAINWINDOW_H

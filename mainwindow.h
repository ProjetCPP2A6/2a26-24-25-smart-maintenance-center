#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QDebug>
#include <QStandardItemModel>
<<<<<<< HEAD
#include "perso.h"  // Inclure le fichier Perso
#include "assiduite.h"
=======

>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770
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

<<<<<<< HEAD
    void afficherPersonnel(); // Assurez-vous que cette méthode est bien déclarée
=======
    void afficherPersonnel(); // Ensure this is declared

>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770

    void on_Supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_pb_Ajouter_clicked();

<<<<<<< HEAD
    void on_pushButton_25_clicked();

    void on_p_trier_clicked();

    void on_trier_age_clicked();

    void on_rechercher_clicked();

    void on_consulter_clicked();


    void on_afficherabsc_clicked();

    void on_enregistrer_clicked();

private:
    Ui::MainWindow *ui;
    bool cond; // Initialiser cond si nécessaire
    Perso* persoManager;  // Déclarer persoManager comme un pointeur vers la classe Perso
    Assiduite assiduite;  // Déclarer assiduite comme membre de MainWindow si nécessaire
        Assiduite *assiduiteManager;  // Déclaration du gestionnaire des absences
=======
private:
    Ui::MainWindow *ui;
    bool cond; // Déplacer ici
    QStandardItemModel *PersonnelModel; // Déplacer ici
>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770

};

#endif // MAINWINDOW_H

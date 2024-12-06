#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "maintenance.h"
#include "perso.h"
#include "assiduite.h"
#include "authentification.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTextBrowser>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QLabel>



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

    void on_addmaintain_clicked();
    void on_pushbutton55_clicked();
    void on_modifier_clicked();

    void on_reaficher_clicked();

    void on_reaficher_2_clicked();

    void on_reaficher_3_clicked();



    void on_recherche_clicked();



    void on_pushButton_4_clicked();
    void updateCalendarHighlights();

    void on_pushButton_27_clicked();
    void sendEmail(const QString &recipient, const QString &subject, const QString &body);


    void generateMaintenanceReport(int id);
    void on_generatereport_clicked();


    void on_reset_4_clicked();
    QString searchDatabase(const QString &queryText);
    void sendMessageToOasisAI(const QString &userMessage, QTextBrowser *textBrowser);
    void rescheduleMaintenanceTasks();
    void on_pushButton_28_clicked();
    void readSerialData();


    void on_btnHighestGaz_clicked();

    void on_btnLowestGaz_clicked();

    void on_pushButton_29_clicked();
    void afficherPersonnel(); // Assurez-vous que cette méthode est bien déclarée

    void on_Supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_pb_Ajouter_clicked();

    void on_pushButton_25_clicked();

    void on_p_trier_clicked();

    void on_trier_age_clicked();

    void on_rechercher_clicked();

    void on_consulter_clicked();


    void on_afficherabsc_clicked();

    void on_enregistrer_clicked();

    void on_stat_2_clicked();
    void readFromArduino();


    void on_readbutton_clicked();

private:
    Ui::MainWindow *ui;
    bool cond;
    Maintenance maintenanceManager;
    QSortFilterProxyModel *maintenanceProxyModel;
    QStandardItemModel *maintenanceModel;
    QSerialPort *serial;
    QSerialPort *arduino;        // Serial port object
    QLabel *statusLabel;    // Label to display gas detection status
    QList<int> gasValues;  // List to store gas values
    QLabel *highestGazLabel; // Label to display highest gaz value
    QLabel *lowestGazLabel;  // Label to display lowest gaz value
    int highest=0;
    int currentSensorValue;
    int lowest=500;
    void setupSerialPort();    // Function to initialize the serial port
    void setupModels();
    void loadMaintenanceData();
    Perso* persoManager;  // Déclarer persoManager comme un pointeur vers la classe Perso
    Assiduite assiduite;  // Déclarer assiduite comme membre de MainWindow si nécessaire
    Assiduite *assiduiteManager;  // Déclaration du gestionnaire des absences
    void insertIntoDatabase(const QString &numCarte, const QString &cin);

};
#endif // MAINWINDOW_H

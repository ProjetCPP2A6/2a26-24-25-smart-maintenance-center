#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStandardItemModel>
#include "equipement.h"
#include "dialog_statistiques.h"
#include "smtp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(QString, QString, QMainWindow *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBox_IDs_currentIndexChanged(int index);
    void on_line_Recherche_textChanged(const QString &arg1);
    void on_bt_Tri_Nom_clicked();
    void on_bt_Tri_Marque_clicked();
    void on_bt_Tri_ID_clicked();
    void on_bt_Tri_Statut_clicked();
    void on_bt_Statistique_clicked();
    void on_bt_ExportPDF_clicked();
    void on_push_equi_Ajouter_clicked();
    void on_push_equi_Modifier_clicked();
    void on_push_equi_Supp_clicked();
    void on_envoyer_dialog_2_clicked();
    void Function_Mailing();
    void on_ClearningAlerts_clicked();
    void on_calendarWidget_selectionChanged();
    void highlightDates();
    void sendSerialCommand(const QString &command);  // Function to send serial command to Arduino

private:
    Ui::MainWindow *ui;
    Equipement E;
    Dialog_Statistiques *DS;
    QStandardItemModel *equipmentModel;
    QSerialPort *serialPort;  // Serial port for Arduino communication
};

#endif // MAINWINDOW_H

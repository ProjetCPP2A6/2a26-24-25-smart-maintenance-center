#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ressources.h"
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

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

    void readFromArduino();

    void insertIntoDatabase(const QString &temp, const QString &hum) ;

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


    void on_ajouter_clicked();

    void on_suppr_clicked();

    void on_Mod_3_clicked();

    void on_rechercher_clicked();


    void on_pdf_clicked();

    void on_ajouter_2_clicked();

    void on_Mod_5_clicked();

    void on_check_clicked();

    void animateProgressBar(int targetValue, const QString &style);

    void updateSortOrder();

    void on_ex_clicked();

    void exportToPDF(QSqlQueryModel *model, const QString &situationText);





private:
    Ui::MainWindow *ui;
    bool cond;
    ressources res;
    QSerialPort *arduino; // Serial port object
    QString arduinoPortName; // Name of the Arduino port
};
#endif // MAINWINDOW_H

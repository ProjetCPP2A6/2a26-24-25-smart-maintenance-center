#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "maintenance.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTextBrowser>

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
    void on_pushButton_28_clicked();
private:
    Ui::MainWindow *ui;
    bool cond;
    Maintenance maintenanceManager;
    QSortFilterProxyModel *maintenanceProxyModel;
    QStandardItemModel *maintenanceModel;
    void setupModels();
    void loadMaintenanceData();
};
#endif // MAINWINDOW_H

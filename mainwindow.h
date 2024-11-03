#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

    // Slots for managing factures
    void on_pushButtonAddFacture_clicked();
    void on_pushButtonSupprimerFacture_clicked();
    void on_pushButtonModifierFacture_clicked();

    // Slots for managing equipment
    void on_pushButtonAddEquipment_clicked();
    void on_pushButtonModifyEquipment_clicked();
    void on_pushButtonImportPDF_clicked();

private:

    QString equipmentId;  // ID for equipment entries
    Ui::MainWindow *ui;
    bool cond;  // State variable for conditional navigation
    QStandardItemModel *factureModel;    // Model for facture data
    QStandardItemModel *equipmentModel;  // Model for equipment data
};

#endif // MAINWINDOW_H

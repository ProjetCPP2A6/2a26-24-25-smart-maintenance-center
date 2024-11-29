#ifndef mainwindow_h
#define mainwindow_h

#include <QTableView>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QDebug>
#include "ExportPDF.h"
#include "AnalyseFacture.h"
#include <QLabel>          // Add this line
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QTabWidget>
#include <QCheckBox>
#include <QSerialPort>

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
    void applyBlueTextToLineEdits(); // Declare the function here


private slots:
    void mettreAJourMontantTotal();
    void on_pushButtonAddSalarie_clicked();
    void exportTableFournisseurToPdf(const QString &filePath);
    void on_exportPdfButtonFournisseur_clicked();
    void on_triantPrix_clicked();
    void on_triantQuantite_clicked();
    void on_exportPDFButton_clicked();
    void on_UploadImage_Clicked();
    void on_AddToTab_Clicked();
    void exportTableToPdf(const QString &filePath);
    void on_AnalyzeMonth_Clicked();
    void on_AnalyzePaymentStatus_Clicked();
    void on_exportPdfButton_clicked();
    void rechercherFacture();
    void on_pushButtonSearchFournisseur_clicked();
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
    void on_pushButtonAddFacture_clicked(); // Slot pour ajouter une facture

    void on_pushButtonSupprimerFacture_clicked(); // Pour supprimer une facture
    void on_pushButtonModifierFacture_clicked(); // Déclaration de la méthode
    void on_rechercherButton_clicked();

private:
    QLineEdit *lineEditIdFacture;
    QPushButton *btnAfficherFacture; // Déclaration ici
    QSerialPort *arduino;
    Ui::MainWindow *ui;
    bool cond;
    QStandardItemModel *factureModel; // Modèle pour le QTableView
    QLabel *imageLabel;       // For displaying the selected image
    QLineEdit *descriptionEdit; // For entering a description
    QTabWidget *tabWidget;    // For managing images and descriptions in tabs
          // For marking an entry as authentic
    QStandardItemModel *supplierModel;
    QString currentImagePath;
    void setupModelAndView();
    QStandardItemModel *tableModel;
    QStandardItemModel *salarieModel;
};
#endif // MAINWINDOW_H

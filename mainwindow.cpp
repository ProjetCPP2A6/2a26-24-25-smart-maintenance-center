#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    cond(false),
    factureModel(new QStandardItemModel(this)),
    equipmentModel(new QStandardItemModel(this)) // Initializing a model for equipment
{
    ui->setupUi(this);
    ui->home->setEnabled(false);

    // Set up facture model headers
    factureModel->setHorizontalHeaderLabels({"ID Facture", "Date", "Fournisseur", "Montant Total"});
    ui->tableViewFactures->setModel(factureModel);

    // Set up equipment model headers
    equipmentModel->setHorizontalHeaderLabels({"Nom Machine", "Marque", "Date Arrivée"});
    ui->tableViewEquipment->setModel(equipmentModel); // Assuming there’s a QTableView named tableViewEquipment
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Navigation Functions
void MainWindow::on_pushButton_2_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_pushButton_3_clicked() { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::on_pushButton_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_pushButton_12_clicked() { ui->stackedWidget->setCurrentIndex(1); }
void MainWindow::on_pushButton_13_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_close_clicked() { this->close(); }
void MainWindow::on_pushButton_11_clicked() { ui->stackedWidget->setCurrentIndex(6); ui->home->setEnabled(false); }
void MainWindow::on_pushButton_14_clicked() { ui->stackedWidget_3->setCurrentIndex(1); }
void MainWindow::on_pushButton_9_clicked() { ui->stackedWidget_3->setCurrentIndex(2); }
void MainWindow::on_pushButton_15_clicked() { ui->home->setEnabled(true); ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::on_pushButton_16_clicked() { cond=true; ui->home->setEnabled(true); ui->stackedWidget->setCurrentIndex(7); }
void MainWindow::on_pushButton_17_clicked() { ui->stackedWidget_3->setCurrentIndex(0); }
void MainWindow::on_pushButton_18_clicked() { ui->stackedWidget_3->setCurrentIndex(0); }
void MainWindow::on_home_clicked() { ui->stackedWidget->setCurrentIndex(cond ? 7 : 0); }
void MainWindow::on_close_2_clicked() { this->close(); }
void MainWindow::on_pushButton_20_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_pushButton_19_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_pushButton_23_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_pushButton_22_clicked() { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::on_pushButton_21_clicked() { ui->stackedWidget->setCurrentIndex(6); ui->home->setEnabled(false); }

// Facture Functions
void MainWindow::on_pushButtonAddFacture_clicked() {
    QString idFacture = ui->lineEditID->text();
    QString date = ui->lineEditDate->text();
    QString fournisseur = ui->lineEditFournisseur->text();
    QString montantTotal = ui->lineEditMontant->text();

    QList<QStandardItem *> row;
    row << new QStandardItem(idFacture);
    row << new QStandardItem(date);
    row << new QStandardItem(fournisseur);
    row << new QStandardItem(montantTotal);
    factureModel->appendRow(row);

    ui->lineEditID->clear();
    ui->lineEditDate->clear();
    ui->lineEditFournisseur->clear();
    ui->lineEditMontant->clear();
}

void MainWindow::on_pushButtonSupprimerFacture_clicked() {
    QString idFactureASupprimer = ui->lineEditSupprimerID->text();
    for (int row = 0; row < factureModel->rowCount(); ++row) {
        if (factureModel->item(row, 0)->text() == idFactureASupprimer) {
            factureModel->removeRow(row);
            ui->lineEditSupprimerID->clear();
            return;
        }
    }
    QMessageBox::warning(this, "Erreur", "Facture avec cet ID non trouvée.");
}

void MainWindow::on_pushButtonModifierFacture_clicked() {
    bool conversionOk;
    int factureId = ui->lineEditModifierFactureId_2->text().toInt(&conversionOk);
    if (!conversionOk || factureId <= 0) {
        QMessageBox::warning(this, "Erreur", "ID de facture invalide.");
        return;
    }

    QString nouveauFournisseur = ui->lineEditModifierFournisseur->text();
    double nouveauMontant = ui->lineEditModifierMontant->text().toDouble();
    QString nouvelleDate = ui->lineEditModifierDate->text();

    bool factureTrouvee = false;
    for (int i = 0; i < factureModel->rowCount(); ++i) {
        if (factureModel->data(factureModel->index(i, 0)).toInt() == factureId) {
            factureModel->setData(factureModel->index(i, 1), nouveauFournisseur);
            factureModel->setData(factureModel->index(i, 2), nouveauMontant);
            factureModel->setData(factureModel->index(i, 3), nouvelleDate);
            factureTrouvee = true;
            QMessageBox::information(this, "Succès", "Facture modifiée avec succès.");
            break;
        }
    }

    if (!factureTrouvee) {
        QMessageBox::warning(this, "Erreur", "Facture non trouvée.");
    }

    ui->tableViewFactures->reset();
}

// Equipment Functions
void MainWindow::on_pushButtonAddEquipment_clicked() {
    QString machineName = ui->lineEditMachineName->text();
    QString machineBrand = ui->lineEditMachineBrand->text();
    QString arrivalDate = ui->lineEditArrivalDate->text();

    QList<QStandardItem *> row;
    row << new QStandardItem(machineName);
    row << new QStandardItem(machineBrand);
    row << new QStandardItem(arrivalDate);
    equipmentModel->appendRow(row);

    ui->lineEditMachineName->clear();
    ui->lineEditMachineBrand->clear();
    ui->lineEditArrivalDate->clear();
}
void MainWindow::on_pushButtonAddEquipment_clicked() {
    QString equipmentId = ui->lineEditEquipmentID->text();
    // Use equipmentId here
}

void MainWindow::on_pushButtonModifyEquipment_clicked() {
    //int equipmentId = ui->lineEditModifyEquipmentId->text().toInt();
    // QString newBrand = ui->lineEditModifyMachineBrand->text();
    //QString newDate = ui->lineEditModifyDate->text();

    bool found = false;
    for (int i = 0; i < equipmentModel->rowCount(); ++i) {
        if (equipmentModel->item(i, 0)->text().toInt() == equipmentId) {
            equipmentModel->setData(equipmentModel->index(i, 1), newBrand);
            equipmentModel->setData(equipmentModel->index(i, 2), newDate);
            found = true;
            QMessageBox::information(this, "Succès", "Équipement modifié avec succès.");
            break;
        }
    }

    if (!found) {
        QMessageBox::warning(this, "Erreur", "Équipement non trouvé.");
    }

    ui->tableViewEquipment->reset();
}

// PDF Import Function
void MainWindow::on_pushButtonImportPDF_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open PDF File", "", "PDF Files (*.pdf)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "PDF Importé", "Fichier PDF chargé : " + fileName);
        // Add code here for processing the PDF if needed
    }
}

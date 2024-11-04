#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>

#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , equipmentModel(new QStandardItemModel(this)) // Modèle pour les équipements
{
    ui->setupUi(this);

    // Configurer les en-têtes du modèle d'équipement
    equipmentModel->setHorizontalHeaderLabels({"ID Équipement", "Nom Machine", "Marque", "Date Arrivée"});
    ui->tableViewEquipment->setModel(equipmentModel); // Supposons qu'il y a une QTableView nommée tableViewEquipment
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Fonction pour ajouter un équipement
void MainWindow::on_addEquipmentButton_clicked() {
    QString  equipmentID= ui->lineEditEquipmentID->text();
    QString machineName = ui->lineEditMachineName->text();
    QString machineBrand = ui->lineEditMachineBrand->text();
    QString arrivalDate = ui->lineEditArrivalDate->text();

    QList<QStandardItem *> row;
    row << new QStandardItem(equipmentID);
    row << new QStandardItem(machineName);
    row << new QStandardItem(machineBrand);
    row << new QStandardItem(arrivalDate);
    equipmentModel->appendRow(row);

    // Effacer les champs de saisie
    ui->lineEditEquipmentID->clear();
    ui->lineEditMachineName->clear();
    ui->lineEditMachineBrand->clear();
    ui->lineEditArrivalDate->clear();
}



// Fonction pour modifier un équipement
void MainWindow::on_modifyEquipmentButton_clicked() {

}

void MainWindow::on_pushButton_5_clicked()
{    QString equipmentIDToModify = ui->lineEditEquipmentID->text();
    QString newMachineName = ui->lineEditMachineName->text();
    QString newMachineBrand = ui->lineEditMachineBrand->text();
    QString newArrivalDate = ui->lineEditArrivalDate->text();

    bool equipmentFound = false;
    for (int row = 0; row < equipmentModel->rowCount(); ++row) {
        if (equipmentModel->item(row, 0)->text() == equipmentIDToModify) {
            equipmentModel->setData(equipmentModel->index(row, 1), newMachineName);
            equipmentModel->setData(equipmentModel->index(row, 2), newMachineBrand);
            equipmentModel->setData(equipmentModel->index(row, 3), newArrivalDate);
            equipmentFound = true;
            QMessageBox::information(this, "Succès", "Équipement modifié avec succès.");
            break;
        }
    }

    if (!equipmentFound) {
        QMessageBox::warning(this, "Erreur", "Équipement avec cet ID non trouvé.");
    }

    ui->lineEditEquipmentID->clear();
    ui->lineEditMachineName->clear();
    ui->lineEditMachineBrand->clear();
    ui->lineEditArrivalDate->clear();

}


void MainWindow::on_Mod_2_clicked()
{    QString equipmentIDToModify = ui->lineEditEquipmentID->text();
    QString newMachineName = ui->lineEditMachineName->text();
    QString newMachineBrand = ui->lineEditMachineBrand->text();
    QString newArrivalDate = ui->lineEditArrivalDate->text();

    bool equipmentFound = false;
    for (int row = 0; row < equipmentModel->rowCount(); ++row) {
        if (equipmentModel->item(row, 0)->text() == equipmentIDToModify) {
            equipmentModel->setData(equipmentModel->index(row, 1), newMachineName);
            equipmentModel->setData(equipmentModel->index(row, 2), newMachineBrand);
            equipmentModel->setData(equipmentModel->index(row, 3), newArrivalDate);
            equipmentFound = true;
            QMessageBox::information(this, "Succès", "Équipement modifié avec succès.");
            break;
        }
    }

    if (!equipmentFound) {
        QMessageBox::warning(this, "Erreur", "Équipement avec cet ID non trouvé.");
    }

    ui->lineEditEquipmentID->clear();
    ui->lineEditMachineName->clear();
    ui->lineEditMachineBrand->clear();
    ui->lineEditArrivalDate->clear();

}


void MainWindow::on_lineEditDeleteEquipmentID_clicked()
{     QString equipmentIDToDelete = ui->lineEditDeleteEquipmentID->text();
    for (int row = 0; row < equipmentModel->rowCount(); ++row) {
        if (equipmentModel->item(row, 0)->text() == equipmentIDToDelete) {
            equipmentModel->removeRow(row);
            ui->lineEditEquipmentID->clear();
            QMessageBox::information(this, "Succès", "Équipement supprimé avec succès.");
            return;
        }
    }
    QMessageBox::warning(this, "Erreur", "Équipement avec cet ID non trouvé.");
}





#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cond(false)
    // factureModel(new QStandardItemModel(this))
    , maintenanceModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    ui->home->setEnabled(false);
    connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::on_modifier_clicked);
    // Setup table headers for factureModel and maintenanceModel
    //factureModel->setHorizontalHeaderLabels({"ID Facture", "Date", "Fournisseur", "Montant Total"});
    //ui->tableViewFactures->setModel(factureModel);
    maintenanceModel->setHorizontalHeaderLabels({"ID", "Task", "Equipment", "Action", "Start Date", "End Date", "Status"});
    ui->tab_mat->setModel(maintenanceModel);

    loadMaintenanceData();
}






MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMaintenanceData()
{
    maintenanceModel->clear();
    maintenanceModel->setHorizontalHeaderLabels({"ID", "Task", "Equipment", "Action", "Start Date", "End Date", "Status"});
    maintenanceManager.loadFromDatabase(maintenanceModel);
}

void MainWindow::on_pushButton_2_clicked()
{
      ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_pushButton_clicked()
{
     ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_13_clicked()
{
     ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_close_clicked()
{
    this->close();
}


void MainWindow::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->home->setEnabled(false);
}


void MainWindow::on_pushButton_14_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
}


void MainWindow::on_pushButton_9_clicked()
{
     ui->stackedWidget_3->setCurrentIndex(2);
}


void MainWindow::on_pushButton_15_clicked()
{
    ui->home->setEnabled(true);
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_16_clicked()
{
    cond=true;
    ui->home->setEnabled(true);
    ui->stackedWidget->setCurrentIndex(7);
}


void MainWindow::on_pushButton_17_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
}


void MainWindow::on_pushButton_18_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
}

void MainWindow::on_home_clicked()
{
    if (cond==true)
    {
        // Navigate to page 1 (e.g., index 1)
        ui->stackedWidget->setCurrentIndex(7);
    }
    else
    {
        // Navigate to page 2 (e.g., index 2)
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_close_2_clicked()
{
    this->close();
}


void MainWindow::on_pushButton_20_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_pushButton_19_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_23_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_22_clicked()
{
   ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_pushButton_21_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->home->setEnabled(false);
}

void MainWindow::on_addmaintain_clicked() {
    // Retrieve values from the input fields
    bool conversionOk;
    int id = ui->idedit->text().toInt(&conversionOk);
    QString task = ui->nomtache->text();
    QString equipment = ui->marqueequipement->currentText();
    QString action = ui->pafaire->text();
    QString startDate = ui->dateEdit->date().toString("dd/MM/yyyy");
    QString endDate = ui->dateEdit_2->date().toString("dd/MM/yyyy");

    // Retrieve status based on radio button selection
    QString status;
    if (ui->radioButton->isChecked()) {
        status = "not started";
    } else if (ui->radioButton_2->isChecked()) {
        status = "in progress";
    } else if (ui->radioButton_3->isChecked()) {
        status = "finished";
    }
    if (!conversionOk || id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide (entier positif).");
        return; // Sortir de la fonction si l'ID est invalide
    }

    if (task.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Tâche' ne peut pas être vide.");
        return; // Sortir si la tâche est vide
    }

    if (equipment.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un équipement.");
        return; // Sortir si aucun équipement n'est sélectionné
    }

   /* if (action.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Action' ne peut pas être vide.");
        return; // Sortir si l'action est vide
    }*/


    if (startDate > endDate) {
        QMessageBox::warning(this, "Erreur", "La date de début ne peut pas être supérieure à la date de fin.");
        return; // Sortir si la date de début est supérieure à la date de fin
    }


    // Debug output for checking the data
    qDebug() << "ID:" << id;
    qDebug() << "Task:" << task;
    qDebug() << "Equipment:" << equipment;
    qDebug() << "Action:" << action;
    qDebug() << "Start Date:" << startDate;
    qDebug() << "End Date:" << endDate;
    qDebug() << "Status:" << status;

    // Add a new row to the maintenance model
    QList<QStandardItem *> row;
    row << new QStandardItem(QString::number(id));  // Convert id to QString
    row << new QStandardItem(task);
    row << new QStandardItem(equipment);
    row << new QStandardItem(action);
    row << new QStandardItem(startDate);
    row << new QStandardItem(endDate);
    row << new QStandardItem(status);
    maintenanceModel->appendRow(row);

    // Add maintenance to the database
    if (maintenanceManager.addToDatabase(id, task, equipment, action, startDate, endDate, status)) {
        loadMaintenanceData();
        QMessageBox::information(this, "Success", "Maintenance record added successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to add maintenance record.");
    }

    loadMaintenanceData();
    // Clear the input fields after adding
    ui->idedit->clear();
    ui->nomtache->clear();
    ui->marqueequipement->clear();
    ui->pafaire->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(QDate::currentDate());
    ui->radioButton->setChecked(true);  // Default to 'not started'
}





void MainWindow::on_pushbutton55_clicked()
{
    QString idToDelete = ui->id_supp->text();  // Get the ID of the maintenance act to delete

    // Ensure the ID field is not empty
    if (idToDelete.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Please enter a valid maintenance ID to delete.");
        return;
    }

    // Create a database query to delete the record with the specified ID
    QSqlQuery query;
    query.prepare("DELETE FROM MAINTENANCE WHERE ID = :id");
    query.bindValue(":id", idToDelete.toInt());  // Convert the ID to integer if necessary

    // Execute the query and check if it was successful
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to delete maintenance record: " + query.lastError().text());
        return;
    }

    // Check if any rows were affected (i.e., record deleted)
    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Erreur", "Maintenance record with this ID not found.");
    } else {
        // Remove the row from the model in the view
        for (int row = 0; row < maintenanceModel->rowCount(); ++row) {
            if (maintenanceModel->item(row, 0)->text() == idToDelete) {  // Check if ID matches (assuming ID is in column 0)
                maintenanceModel->removeRow(row);  // Remove the row from the model
                break;
            }
        }

        QMessageBox::information(this, "Success", "Maintenance record deleted successfully.");
    }

    // Clear the input field after deletion
    ui->id_supp->clear();
}

void MainWindow::on_modifier_clicked() {
    QSqlQuery query;

    // Loop through each row in the model
    for (int row = 0; row < maintenanceModel->rowCount(); ++row) {
        int id = maintenanceModel->item(row, 0)->text().toInt();
        QString task = maintenanceModel->item(row, 1)->text();
        QString equipment = maintenanceModel->item(row, 2)->text();
        QString action = maintenanceModel->item(row, 3)->text();
        QString startDate = maintenanceModel->item(row, 4)->text();
        QString endDate = maintenanceModel->item(row, 5)->text();
        QString status = maintenanceModel->item(row, 6)->text();

        // Prepare the SQL update query
        query.prepare("UPDATE MAINTENANCE SET TASK = :task, EQUIPMENT = :equipment, ACTION = :action, "
                      "START_DATE = TO_DATE(:startDate, 'DD/MM/YYYY'), END_DATE = TO_DATE(:endDate, 'DD/MM/YYYY'), "
                      "STATUS = :status WHERE ID = :id");
        query.bindValue(":task", task);
        query.bindValue(":equipment", equipment);
        query.bindValue(":action", action);
        query.bindValue(":startDate", startDate);
        query.bindValue(":endDate", endDate);
        query.bindValue(":status", status);
        query.bindValue(":id", id);

        // Execute the query and check for errors
        if (!query.exec()) {
            qDebug() << "Failed to update row with ID:" << id << "Error:" << query.lastError().text();
        }
    }

    QMessageBox::information(this, "Success", "Database updated successfully from the table.");
}

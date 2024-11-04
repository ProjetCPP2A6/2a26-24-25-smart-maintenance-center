#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <QStandardItemModel>
#include <QSqlError>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      cond(false),
      PersonnelModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    ui->home->setEnabled(false);
    PersonnelModel->setHorizontalHeaderLabels({"CIN", "Nom", "Prénom", "Date de naissance", "Adresse", "E-mail", "Téléphone", "Role"});
    ui->tab_personnel->setModel(PersonnelModel);
    afficherPersonnel(); // Load personnel data on startup
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
void MainWindow::on_pushButton_16_clicked() { cond = true; ui->home->setEnabled(true); ui->stackedWidget->setCurrentIndex(7); }
void MainWindow::on_pushButton_17_clicked() { ui->stackedWidget_3->setCurrentIndex(0); }
void MainWindow::on_pushButton_18_clicked() { ui->stackedWidget_3->setCurrentIndex(0); }

void MainWindow::on_home_clicked()
{
    if (cond) {
        ui->stackedWidget->setCurrentIndex(7);
    } else {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_close_2_clicked() { this->close(); }
void MainWindow::on_pushButton_20_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_pushButton_19_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_pushButton_23_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_pushButton_22_clicked() { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::on_pushButton_21_clicked() { ui->stackedWidget->setCurrentIndex(6); ui->home->setEnabled(false); }

void MainWindow::on_pb_Ajouter_clicked()
{
    // Récupération des entrées
    int cin = ui->p_CIN->text().toInt();
    QString nom = ui->p_NOM->text().trimmed();
    QString prenom = ui->p_PRENOM->text().trimmed();
    QString dateNaissance = ui->p_datenaiss->text().trimmed();
    QString adresse = ui->p_ADRESSE->text().trimmed();
    QString email = ui->p_MAIL->text().trimmed();
    QString telephoneStr = ui->p_TELEPHONE->text().trimmed();

    // Validation des entrées
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isEmpty() ||
        adresse.isEmpty() || email.isEmpty() || telephoneStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur d'entrée", "Tous les champs doivent être remplis.");
        return;
    }

    bool ok;
    int telephone = telephoneStr.toInt(&ok);
    if (!ok || telephoneStr.length() != 8) {
        QMessageBox::warning(this, "Erreur d'entrée", "Veuillez entrer un numéro de téléphone valide.");
        return;
    }

    // Vérification de l'unicité de CIN
    QSqlQuery checkCINQuery;
    checkCINQuery.prepare("SELECT COUNT(*) FROM PERSONNEL WHERE CIN = :cin");
    checkCINQuery.bindValue(":cin", cin);
    if (checkCINQuery.exec() && checkCINQuery.next() && checkCINQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Erreur", "Ce CIN existe déjà.");
        return;
    }

    // Vérification de la connexion à la base de données
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur de base de données", "La base de données n'est pas ouverte.");
        return;
    }

    // Insertion dans la base de données
    QSqlQuery query;
    query.prepare("INSERT INTO PERSONNEL (CIN, NOM, PRENOM, DATENAISS, ADRESSE, EMAIL, TELEPHONE, ROLE) "
                  "VALUES (:cin, :nom, :prenom, TO_DATE(:dateNaissance, 'YYYY-MM-DD'), :adresse, :email, :telephone, :role)");
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", QDate::fromString(dateNaissance, "yyyy-MM-dd"));

    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":role", ui->p_Role->text().trimmed());

    // Exécution de la requête
    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Le personnel a été ajouté avec succès.");
        afficherPersonnel(); // Rafraîchir le tableau
    } else {
        QMessageBox::warning(this, "Échec", "Échec de l'ajout du personnel : " + query.lastError().text());
    }
}

void MainWindow::on_pb_modifier_clicked()
{
    // Récupération des entrées
    int cin = ui->line_cin->text().toInt();
    QString nom = ui->line_nom->text().trimmed();
    QString prenom = ui->line_prenom->text().trimmed();
    QString dateNaissance = ui->line_naiss->text().trimmed();
    QString adresse = ui->line_adresse->text().trimmed();
    QString email = ui->line_adresseMail->text().trimmed();
    QString telephoneStr = ui->line_telephone->text().trimmed();
    QString role = ui->line_role->text().trimmed();

    // Validation des entrées
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isEmpty() ||
        adresse.isEmpty() || email.isEmpty() || telephoneStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur d'entrée", "Tous les champs doivent être remplis.");
        return;
    }

    bool ok;
    int telephone = telephoneStr.toInt(&ok);
    if (!ok || telephoneStr.length() != 8) {
        QMessageBox::warning(this, "Erreur d'entrée", "Veuillez entrer un numéro de téléphone valide.");
        return;
    }

    // Vérification de la connexion à la base de données
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur de base de données", "La base de données n'est pas ouverte.");
        return;
    }

    // Vérification de l'existence du personnel
    QSqlQuery query;
        query.prepare("SELECT * FROM PERSONNEL WHERE CIN = :cin");
        query.bindValue(":cin", cin);

        if (query.exec() && query.first()) {
            // Mise à jour du personnel
            query.prepare("UPDATE PERSONNEL SET NOM = :nom, PRENOM = :prenom, "
                          "DATENAISS = TO_DATE(:dateNaissance, 'YYYY-MM-DD'), "
                          "ADRESSE = :adresse, EMAIL = :email, TELEPHONE = :telephone, ROLE = :role "
                          "WHERE CIN = :cin");
            query.bindValue(":cin", cin);
            query.bindValue(":nom", nom);
            query.bindValue(":prenom", prenom);
            query.bindValue(":dateNaissance", QDate::fromString(dateNaissance, "yyyy-MM-dd")); // Proper conversion
            query.bindValue(":adresse", adresse);
            query.bindValue(":email", email);
            query.bindValue(":telephone", telephone);
            query.bindValue(":role", role);

            // Exécution de la mise à jour
            if (query.exec()) {
                QMessageBox::information(this, "Succès", "Le personnel a été modifié avec succès.");
                afficherPersonnel(); // Rafraîchir le tableau
            } else {
                QMessageBox::warning(this, "Échec", "Échec de la modification du personnel : " + query.lastError().text());
            }
        } else {
            QMessageBox::warning(this, "Non trouvé", "Aucun personnel avec ce CIN n'a été trouvé.");
        }
    }

void MainWindow::on_Supprimer_clicked()
{
    int cin = ui->L_cin_supp->text().toInt();

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM PERSONNEL WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        // Suppression du personnel
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM PERSONNEL WHERE CIN = :cin");
        deleteQuery.bindValue(":cin", cin);

        if (deleteQuery.exec()) {
            QMessageBox::information(this, "Succès", "Le personnel a été supprimé avec succès.");
            afficherPersonnel(); // Rafraîchir le tableau
        } else {
            QMessageBox::warning(this, "Échec", "Échec de la suppression du personnel : " + deleteQuery.lastError().text());
        }
    } else {
        QMessageBox::warning(this, "Non trouvé", "Aucun personnel avec ce CIN n'a été trouvé.");
    }
}

void MainWindow::afficherPersonnel()
{
    QSqlQuery query("SELECT * FROM PERSONNEL");
    PersonnelModel->setRowCount(0); // Clear previous entries

    while (query.next()) {
        QList<QStandardItem*> items;
        for (int i = 0; i < query.record().count(); ++i) {
            items.append(new QStandardItem(query.value(i).toString()));
        }
        PersonnelModel->appendRow(items); // Add new row to the model
    }
}

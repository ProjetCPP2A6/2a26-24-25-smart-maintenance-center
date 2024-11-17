#include "mainwindow.h"
<<<<<<< HEAD
#include "ui_mainwindow.h"

#include "perso.h"
=======
#include "./ui_mainwindow.h"
>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <QStandardItemModel>
#include <QSqlError>
#include <QRegularExpression>
<<<<<<< HEAD
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QTableWidgetItem>
#include "assiduite.h"


#include "authentification.h"
=======
>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
<<<<<<< HEAD
      persoManager(new Perso(this)) // Create instance of Perso
{
    ui->setupUi(this);

    // Set up the Personnel table model
    ui->tab_personnel->setModel(persoManager->PersonnelModel);
    persoManager->afficherPersonnel(); // Load personnel data at startup

    // Disable the home button initially
    ui->home->setEnabled(false);
=======
      cond(false),
      PersonnelModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    ui->home->setEnabled(false);
    PersonnelModel->setHorizontalHeaderLabels({"CIN", "Nom", "Prénom", "Date de naissance", "Adresse", "E-mail", "Téléphone", "Role"});
    ui->tab_personnel->setModel(PersonnelModel);
    afficherPersonnel(); // Load personnel data on startup
>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770
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
<<<<<<< HEAD
    // Récupérer les entrées
=======
    // Récupération des entrées
>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770
    int cin = ui->p_CIN->text().toInt();
    QString nom = ui->p_NOM->text().trimmed();
    QString prenom = ui->p_PRENOM->text().trimmed();
    QString dateNaissance = ui->p_datenaiss->text().trimmed();
    QString adresse = ui->p_ADRESSE->text().trimmed();
    QString email = ui->p_MAIL->text().trimmed();
    QString telephoneStr = ui->p_TELEPHONE->text().trimmed();
<<<<<<< HEAD
    QString role = ui->p_Role->text().trimmed();

    // Appeler la méthode ajouterPersonnel de la classe Perso
    persoManager->ajouterPersonnel(cin, nom, prenom, dateNaissance, adresse, email, telephoneStr, role);
=======

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
>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770
}

void MainWindow::on_pb_modifier_clicked()
{
<<<<<<< HEAD
    // Récupérer les entrées
=======
    // Récupération des entrées
>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770
    int cin = ui->line_cin->text().toInt();
    QString nom = ui->line_nom->text().trimmed();
    QString prenom = ui->line_prenom->text().trimmed();
    QString dateNaissance = ui->line_naiss->text().trimmed();
    QString adresse = ui->line_adresse->text().trimmed();
    QString email = ui->line_adresseMail->text().trimmed();
    QString telephoneStr = ui->line_telephone->text().trimmed();
    QString role = ui->line_role->text().trimmed();

<<<<<<< HEAD
    // Appeler la méthode modifierPersonnel de la classe Perso
    persoManager->modifierPersonnel(cin, nom, prenom, dateNaissance, adresse, email, telephoneStr, role);
}

void MainWindow::on_Supprimer_clicked()
{
    // Récupérer le CIN pour suppression
    int cin = ui->L_cin_supp->text().toInt();

    // Appeler la méthode supprimerPersonnel de la classe Perso
    persoManager->supprimerPersonnel(cin);
=======
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
>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770
}

void MainWindow::afficherPersonnel()
{
<<<<<<< HEAD
    // Rafraîchir l'affichage des personnels
    persoManager->afficherPersonnel();
}

void MainWindow::on_pushButton_25_clicked()
{
    // Step 1: Open file dialog to choose where to save the PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Save Personnel Data as PDF", "", "PDF Files (*.pdf)");

    if (fileName.isEmpty()) {
        return; // If user cancels the save dialog, exit
    }

    // Step 2: Prepare PDF writer
    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    writer.setPageMargins(QMargins(30, 30, 30, 30)); // Set margins for the PDF

    QPainter painter(&writer); // Prepare QPainter to paint on the PDF

    if (!painter.begin(&writer)) {
        QMessageBox::critical(this, "Error", "Failed to create the PDF.");
        return;
    }

    // Step 3: Set up font for the PDF
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);

    // Step 4: Add header to the PDF
    painter.drawText(100, 100, "Personnel Information");
    painter.drawLine(100, 110, 500, 110);  // Draw a horizontal line after the header

    // Step 5: Write the table headers
    int rowHeight = 30;
    int colWidth = 100;
    int currentY = 130;

    QStringList headers = {"CIN", "Nom", "Prénom", "Date de naissance", "Adresse", "E-mail", "Téléphone", "Role"};
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(100 + i * colWidth, currentY, headers[i]);
    }

    currentY += rowHeight;  // Move down after the header row

    // Step 6: Write the personnel data from the model
    for (int row = 0; row < persoManager->PersonnelModel->rowCount(); ++row) {
        for (int col = 0; col < persoManager->PersonnelModel->columnCount(); ++col) {
            QModelIndex index = persoManager->PersonnelModel->index(row, col);
            QString cellText = index.data().toString();
            painter.drawText(100 + col * colWidth, currentY, cellText);
        }
        currentY += rowHeight;  // Move down to the next row
    }

    // Step 7: End the PDF painting
    painter.end();

    // Step 8: Notify the user of success
    QMessageBox::information(this, "Success", "PDF Exported Successfully!");

    // Optionally, open the PDF after export
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void MainWindow::on_p_trier_clicked()
{
    // Appeler la méthode de tri par CIN dans Perso
    persoManager->trierParCIN();
}

void MainWindow::on_trier_age_clicked()
{
    // Call the trierParAge method to sort the personnel by age
    persoManager->trierParAge();
}


// mainwindow.cpp
void MainWindow::on_rechercher_clicked()
{
    // Récupérer le CIN saisi par l'utilisateur dans un champ QLineEdit
    bool ok;
    int cin = ui->lineEdit_6->text().toInt(&ok);  // Remplacez 'line_cin_recherche' par le nom de votre QLineEdit

    if (ok && cin > 0) {
        // Appeler la méthode rechercher avec le CIN en paramètre
        persoManager->rechercher(cin);
    } else {
        // Afficher un message d'erreur si le CIN est invalide
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide.");
    }
}

void MainWindow::on_consulter_clicked()
{
    bool ok;
    int cin = ui->p_CIN_2->text().toInt(&ok);  // Récupérer le CIN à partir de l'interface utilisateur

    if (ok && cin > 0) {
        Assiduite assiduite;
        bool success = assiduite.consulterAbsences(cin);

        if (success) {
            ui->p_CIN->setText(assiduite.getAbsencesInfo());  // Afficher les absences dans le QTextEdit
        } else {
            QMessageBox::warning(this, "Erreur", "Aucune absence trouvée pour ce CIN.");
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide.");
    }
}




void MainWindow::on_afficherabsc_clicked()
{
    int cin = ui->p_CIN_2->text().toInt();  // Récupérer le CIN
    if (assiduiteManager->consulterAbsences(cin)) {
        ui->textEdit->setText(assiduiteManager->getAbsencesInfo());  // Afficher les informations dans un QTextEdit
    } else {
        ui->textEdit->setText("Aucune absence trouvée pour ce CIN.");
    }
}


void MainWindow::on_enregistrer_clicked()
{
    // Récupérer les valeurs des champs de l'interface utilisateur
    int cin = ui->p_CIN_3->text().toInt();  // Supposons que p_CIN_3 est un QLineEdit pour le CIN
    QString motDePasse = ui->lineEdit_5->text();  // Supposons que lineEdit_5 est un QLineEdit pour le mot de passe

    // Vérifier si les champs sont vides
    if (cin == 0 || motDePasse.isEmpty()) {
        // Afficher un message d'erreur si le CIN ou le mot de passe est vide
        QMessageBox::warning(this, "Erreur", "Le CIN ou le mot de passe est vide!");
        return;  // Ne pas enregistrer
    }

    // Vérifier quel bouton radio est sélectionné pour l'état du compte
    QString etatCompte;
    if (ui->radioButton_3->isChecked()) {
        etatCompte = "actif";
    } else if (ui->radioButton_4->isChecked()) {
        etatCompte = "inactif";
    } else if (ui->radioButton_5->isChecked()) {
        etatCompte = "suspendu";
    } else {
        // Afficher un message d'erreur si aucun état de compte n'est sélectionné
        QMessageBox::warning(this, "Erreur", "Aucun état de compte sélectionné!");
        return;  // Aucun état sélectionné, ne pas enregistrer
    }

    // Créer une instance de la classe Authentification
    Authentification auth;

    // Appeler la méthode pour enregistrer l'utilisateur dans la base de données
    if (auth.enregistrerUtilisateur(cin, motDePasse, etatCompte)) {
        // Afficher un message de succès
        QMessageBox::information(this, "Succès", "Utilisateur enregistré avec succès.");

        // Réinitialiser les champs de l'interface utilisateur
        ui->p_CIN_3->clear();
        ui->lineEdit_5->clear();
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
    } else {
        // Afficher un message d'erreur si l'enregistrement échoue
        QMessageBox::critical(this, "Erreur", "Échec de l'enregistrement de l'utilisateur.");
=======
    QSqlQuery query("SELECT * FROM PERSONNEL");
    PersonnelModel->setRowCount(0); // Clear previous entries

    while (query.next()) {
        QList<QStandardItem*> items;
        for (int i = 0; i < query.record().count(); ++i) {
            items.append(new QStandardItem(query.value(i).toString()));
        }
        PersonnelModel->appendRow(items); // Add new row to the model
>>>>>>> ef0833f43e33143d2fd702f11a9ec073a3649770
    }
}

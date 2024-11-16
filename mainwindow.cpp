#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "perso.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <QStandardItemModel>
#include <QSqlError>
#include <QRegularExpression>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QTableWidgetItem>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      persoManager(new Perso(this)) // Create instance of Perso
{
    ui->setupUi(this);

    // Set up the Personnel table model
    ui->tab_personnel->setModel(persoManager->PersonnelModel);
    persoManager->afficherPersonnel(); // Load personnel data at startup

    // Disable the home button initially
    ui->home->setEnabled(false);
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
    // Récupérer les entrées
    int cin = ui->p_CIN->text().toInt();
    QString nom = ui->p_NOM->text().trimmed();
    QString prenom = ui->p_PRENOM->text().trimmed();
    QString dateNaissance = ui->p_datenaiss->text().trimmed();
    QString adresse = ui->p_ADRESSE->text().trimmed();
    QString email = ui->p_MAIL->text().trimmed();
    QString telephoneStr = ui->p_TELEPHONE->text().trimmed();
    QString role = ui->p_Role->text().trimmed();

    // Appeler la méthode ajouterPersonnel de la classe Perso
    persoManager->ajouterPersonnel(cin, nom, prenom, dateNaissance, adresse, email, telephoneStr, role);
}

void MainWindow::on_pb_modifier_clicked()
{
    // Récupérer les entrées
    int cin = ui->line_cin->text().toInt();
    QString nom = ui->line_nom->text().trimmed();
    QString prenom = ui->line_prenom->text().trimmed();
    QString dateNaissance = ui->line_naiss->text().trimmed();
    QString adresse = ui->line_adresse->text().trimmed();
    QString email = ui->line_adresseMail->text().trimmed();
    QString telephoneStr = ui->line_telephone->text().trimmed();
    QString role = ui->line_role->text().trimmed();

    // Appeler la méthode modifierPersonnel de la classe Perso
    persoManager->modifierPersonnel(cin, nom, prenom, dateNaissance, adresse, email, telephoneStr, role);
}

void MainWindow::on_Supprimer_clicked()
{
    // Récupérer le CIN pour suppression
    int cin = ui->L_cin_supp->text().toInt();

    // Appeler la méthode supprimerPersonnel de la classe Perso
    persoManager->supprimerPersonnel(cin);
}

void MainWindow::afficherPersonnel()
{
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

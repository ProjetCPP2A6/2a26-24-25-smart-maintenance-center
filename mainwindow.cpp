#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
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
#include "assiduite.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>



#include "authentification.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    arduino(nullptr),  // Initialize arduino to nullptr
    persoManager(new Perso(this))  // Create Perso instance after
{
    // Initialize the user interface
    ui->setupUi(this);

    // Connect the 'readbutton' to the appropriate slot
    connect(ui->readbutton, &QPushButton::clicked, this, &MainWindow::on_readbutton_clicked);

    // Other UI initializations
    ui->tab_personnel->setModel(persoManager->PersonnelModel);  // Set model for personnel table
    persoManager->afficherPersonnel();  // Load personnel data at startup

    // Disable the 'home' button initially
    ui->home->setEnabled(false);

    // Initialize serial communication with Arduino
    arduino = new QSerialPort(this);  // Initialize the arduino object

    // Ensure the correct port is set (verify the port with Arduino IDE or device manager)
    arduino->setPortName("COM7");  // Ensure this matches your system's port

    // Set serial communication parameters
    arduino->setBaudRate(QSerialPort::Baud9600);  // Ensure this matches Arduino's baud rate
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);

    // Open the serial port with ReadWrite access
    if (arduino->open(QIODevice::ReadWrite)) {
        qDebug() << "Arduino connected!";
        connect(arduino, &QSerialPort::readyRead, this, &MainWindow::readFromArduino);
    } else {
        qDebug() << "Failed to connect to Arduino!";
    }
}


MainWindow::~MainWindow()
{
    // Nettoyage des ressources allouées
    if (db.isOpen()) {
        db.close();  // Ferme la connexion à la base de données si elle est ouverte
    }
    delete arduino;  // Libérer l'objet Arduino
    delete persoManager;  // Libérer l'objet PersoManager si nécessaire
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
    // Retrieve input data
    int cin = ui->p_CIN->text().toInt();
    QString nom = ui->p_NOM->text().trimmed();
    QString prenom = ui->p_PRENOM->text().trimmed();
    QString dateNaissance = ui->p_datenaiss->text().trimmed();
    QString adresse = ui->p_ADRESSE->text().trimmed();
    QString email = ui->p_MAIL->text().trimmed();
    QString telephoneStr = ui->p_TELEPHONE->text().trimmed();
    QString role = ui->p_Role->text().trimmed();

    // Check if 'NUM_CARTE' is empty or not
    QString numCarte = ui->p_carte->text().trimmed();
    if (numCarte.isEmpty()) {
        numCarte = "NULL";  // Ensure it's NULL if not provided, as per the schema
    }

    // Call the addPersonnel method of Perso class
    persoManager->ajouterPersonnel(cin, nom, prenom, dateNaissance, adresse, email, telephoneStr, role, numCarte);
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
    QString numCarte = ui->lcarte->text().trimmed();

    // Appeler la méthode modifierPersonnel de la classe Perso
    persoManager->modifierPersonnel(cin, nom, prenom, dateNaissance, adresse, email, telephoneStr, role,numCarte);
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
    }
}

void MainWindow::on_stat_2_clicked()
{
    // Query to get the total number of personnel
    QSqlQuery query("SELECT COUNT(*) FROM AMAL.PERSONNEL");

    if (query.next()) {
        int totalPersonnel = query.value(0).toInt();
        ui->label_totalPersonnel->setText("Total Personnel: " + QString::number(totalPersonnel));
    } else {
        QMessageBox::warning(this, "Error", "Failed to retrieve personnel count.");
    }
}

/*void MainWindow::connectDB() {
    db = QSqlDatabase::addDatabase("QOCI");  // Utilisation du driver Oracle (ajustez si vous utilisez MySQL, etc.)
    db.setHostName("localhost");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("amal");
    db.setPassword("amalmanai");

    if (!db.open()) {
        QMessageBox::critical(this, "Échec de la connexion à la base de données", db.lastError().text());
        return; // Ne pas continuer si la connexion échoue
    } else {
        qDebug() << "Base de données connectée avec succès!";
    }
}*/

void MainWindow::closeEvent(QCloseEvent *event) {
    if (db.isOpen()) {
        db.close();  // Fermer la base de données si elle est ouverte
    }
    event->accept();  // Accepter l'événement et fermer la fenêtre
}

void MainWindow::readFromArduino() {
    // Vérifier si la connexion à Arduino est ouverte
    if (!arduino || !arduino->isOpen()) {
        QMessageBox::critical(this, "Erreur de connexion à Arduino", "Impossible de se connecter à Arduino.");
        return; // Quitter si Arduino n'est pas connecté ou ouvert
    }

    static QByteArray buffer; // Buffer pour stocker les données reçues
    QByteArray data = arduino->readAll(); // Lire toutes les données disponibles depuis Arduino

    // Si aucune donnée n'est reçue, afficher un message
    if (data.isEmpty()) {
        qDebug() << "Aucune donnée reçue.";
    } else {
        qDebug() << "Données reçues : " << data;
    }

    buffer.append(data); // Ajouter les nouvelles données au buffer

    // Traiter les lignes dans le buffer
    while (buffer.contains('\n')) {
        int newlineIndex = buffer.indexOf('\n'); // Trouver le caractère de nouvelle ligne
        QByteArray line = buffer.left(newlineIndex).trimmed(); // Extraire la ligne complète
        buffer.remove(0, newlineIndex + 1); // Retirer la ligne traitée du buffer

        QString dataStr = QString::fromUtf8(line); // Convertir en QString

        qDebug() << "Données extraites : " << dataStr; // Afficher les données reçues pour débogage

        // Vérifier si nous avons reçu le NUM_CARTE (identifiant de carte)
        if (dataStr.length() == 8) { // Supposons que NUM_CARTE est une chaîne hexadécimale de 8 caractères
            QString numCarte = dataStr.trimmed(); // Stocker l'ID de carte scannée
            qDebug() << "NUM_CARTE reçu : " << numCarte;

            // Effectuer la requête pour récupérer le CIN associé à ce NUM_CARTE
            QSqlQuery query;
            query.prepare("SELECT CIN FROM AMAL.RENDU WHERE NUM_CARTE = :numCarte");
            query.bindValue(":numCarte", numCarte);

            if (query.exec()) {
                qDebug() << "Requête SQL exécutée avec succès.";
                if (query.next()) {
                    // CIN trouvé dans la base de données, l'afficher
                    int cin = query.value(0).toInt(); // Récupérer la valeur CIN
                    ui->tcin->setText("CIN: " + QString::number(cin));
                     ui->tcart->setText("Num Carte: " + numCarte);  // Afficher numCarte (ID de la carte)
                    qDebug() << "CIN associé à la carte " << numCarte << ": " << cin;
                } else {
                    // NUM_CARTE non trouvé dans la base de données
                                ui->tcin->setText("0");
                                ui->tcart->setText("Num Carte: " + numCarte);  // Afficher numCarte (ID de la carte)
                                qDebug() << "CIN non trouvé pour NUM_CARTE: " << numCarte;

                                // Afficher un message d'erreur si NUM_CARTE n'est pas trouvé
                                QMessageBox::warning(this, "Carte non trouvée", "La carte n'a pas été trouvée dans la base de données.");
                            }
            } else {
                // Gérer les erreurs de la requête SQL
                qDebug() << "Erreur de la requête SQL: " << query.lastError();
                QMessageBox::critical(this, "Erreur de la base de données", "Erreur lors de la requête SQL.");
            }
        } else {
            // Si les données ne sont pas valides, afficher un message
            qDebug() << "Données invalides reçues: " << dataStr;
        }
    }
}

void MainWindow::on_readbutton_clicked() {
    qDebug() << "Bouton 'Lire' cliqué";
    readFromArduino();  // Appeler la fonction qui lit les données de l'Arduino
}

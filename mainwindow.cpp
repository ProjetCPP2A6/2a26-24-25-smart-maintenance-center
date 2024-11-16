#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QRegularExpression>
#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <dialog_statistiques.h>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , equipmentModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    ui->line_equ_ID->setValidator( new QIntValidator(0, 99999999, this));
    ui->tableViewEquipment->setModel(E.Afficher()); //Function Afficher
    ui->comboBox_IDs->setModel(E.Afficher_ID()); // Afficher les IDs fel combobox
    MainWindow::connect(ui->envoyer_dialog_2, SIGNAL(clicked()),this, SLOT(sendMail()));
    Function_Mailing();
    ui->tb_Alertsss->setText(E.read());
    highlightDates();
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(QString e,QString n,QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QString contenu="Contenu";
    ui->setupUi(this);
    ui->recipient_2->setText(e);
}

void MainWindow::on_push_equi_Ajouter_clicked()
{
    int ID = ui->line_equ_ID->text().toInt();
    QString NOM = ui->line_equ_Nom->text();
    QString STATUS = ui->comboBox_Status->currentText();
    QString MARQUE = ui->line_equ_Marque->text();
    QString LOCALISATION = ui->line_equ_localisation->text();
    QDate DATE = ui->date_equ_date->date();
    if(ui->line_equ_ID->text().isEmpty()||LOCALISATION.isEmpty()||NOM.isEmpty()||STATUS.isEmpty()||MARQUE.isEmpty()){
        ui->label_info->setText("Erreur de controle de saisire");
        return;
    }
    Equipement E(ID,NOM,MARQUE,LOCALISATION,STATUS,DATE);
    bool test=E.Ajouter();
    if(test){
        ui->label_info->setText("Ajout effectuer avec succes");
        ui->tableViewEquipment->setModel(E.Afficher());
        ui->comboBox_IDs->setModel(E.Afficher_ID());
        highlightDates();
    }else{
        ui->label_info->setText("Ajout non effectuer");
    }
}

void MainWindow::on_push_equi_Modifier_clicked()
{
    int ID = ui->line_equ_ID->text().toInt();
    QString NOM = ui->line_equ_Nom->text();
    QString STATUS = ui->comboBox_Status->currentText();
    QString MARQUE = ui->line_equ_Marque->text();
    QString LOCALISATION = ui->line_equ_localisation->text();
    QDate DATE = ui->date_equ_date->date();
    if(ui->line_equ_ID->text().isEmpty()||LOCALISATION.isEmpty()||NOM.isEmpty()||STATUS.isEmpty()||MARQUE.isEmpty()){
        ui->label_info->setText("Erreur de controle de saisire");
        return;
    }
    Equipement E(ID,NOM,MARQUE,LOCALISATION,STATUS,DATE);
    bool test=E.Modifier();
    if(test){
        ui->label_info->setText("Modification effectuer avec succes");
        ui->tableViewEquipment->setModel(E.Afficher());
        ui->comboBox_IDs->setModel(E.Afficher_ID());
        highlightDates();
    }else{
        ui->label_info->setText("Modification non effectuer");
    }
}

void MainWindow::on_push_equi_Supp_clicked()
{
    Equipement E;
    E.setId(ui->comboBox_IDs->currentText().toInt());
    bool test=E.Supprimer(E.getId());
    if(test){
        ui->label_info->setText("Suppression effectuer avec succes");
        ui->tableViewEquipment->setModel(E.Afficher());
        ui->comboBox_IDs->setModel(E.Afficher_ID());
        highlightDates();
    }else{
        ui->label_info->setText("Suppression non effectuer");
    }
}

void MainWindow::on_comboBox_IDs_currentIndexChanged(int index)
{
    int id = ui->comboBox_IDs->currentText().toInt();
    QString id_1=QString::number(id);
    QSqlQuery query1;
    query1.prepare("SELECT * FROM EQUIPEMENTS where ID='"+id_1+"'");
    if(query1.exec())
    {
        while (query1.next())
        {
            ui->line_equ_ID->setText(query1.value(0).toString());
            ui->line_equ_Nom->setText(query1.value(1).toString());
            ui->line_equ_Marque->setText(query1.value(2).toString());
            ui->line_equ_localisation->setText(query1.value(3).toString());
            ui->comboBox_Status->setCurrentText(query1.value(4).toString());
            ui->date_equ_date->setDate(query1.value(5).toDate());
        }
    }
    else
    {
        ui->label_info->setText("Erreur de chargement");
    }
}

void MainWindow::on_line_Recherche_textChanged(const QString &arg1)
{
    E.clearTable(ui->tableViewEquipment);
    E.Recherche(ui->tableViewEquipment,arg1);
}

void MainWindow::on_bt_Tri_Nom_clicked()
{
    ui->label_info_affichage->setText("Tri par NOM effectué");
    ui->tableViewEquipment->setModel(E.Afficher_Tri_NOM());
}

void MainWindow::on_bt_Tri_Marque_clicked()
{
    ui->label_info_affichage->setText("Tri par MARQUE effectué");
    ui->tableViewEquipment->setModel(E.Afficher_Tri_MARQUE());
}

void MainWindow::on_bt_Tri_ID_clicked()
{
    ui->label_info_affichage->setText("Tri par ID effectué");
    ui->tableViewEquipment->setModel(E.Afficher_Tri_ID());
}

void MainWindow::on_bt_Tri_Statut_clicked()
{
    ui->label_info_affichage->setText("Tri par STATUS effectué");
    ui->tableViewEquipment->setModel(E.Afficher_Tri_STATUS());
}

void MainWindow::on_bt_Statistique_clicked()
{
    DS = new Dialog_Statistiques();
    DS->setWindowTitle("Statistique");
    DS->choix_pie();
    DS->show();
}

void MainWindow::on_bt_ExportPDF_clicked()
{
    QString currentPath = QDir::currentPath();
    QString pdfPath = currentPath + "/Liste-Equipement.pdf";
    QPdfWriter pdf(pdfPath);
    QPainter painter(&pdf);
    int i = 4000;
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 30));
    //painter.drawPixmap(QRect(100,400,2000,2000),QPixmap("C:/Users/ranes/OneDrive/Desktop/2a29-smart-conservation-company/logo.png"));
    painter.drawText(3000,1500,"LISTE DES EQUIPEMENTS");
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 50));
    painter.drawRect(2700,200,7000,2600);//(X,Y,W,H)
    painter.drawRect(0,3000,9600,500);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 9));
    painter.drawText(300,3300,"ID");
    painter.drawText(1300,3300,"NOM");
    painter.drawText(2300,3300,"MARQUE");
    painter.drawText(4300,3300,"LOCALISATION");
    painter.drawText(6500,3300,"STATUS");
    painter.drawText(8000,3300,"DATE");
    QSqlQuery query;
    query.prepare("<SELECT CAST( GETDATE() AS Date ) ");
    time_t tt;
    struct tm* ti;
    time(&tt);
    ti=localtime(&tt);
    asctime(ti);
    painter.drawText(500,300, asctime(ti));
    query.prepare("select * from EQUIPEMENTS");
    query.exec();
    while (query.next())
    {
        painter.drawText(300,i,query.value(0).toString());
        painter.drawText(1300,i,query.value(1).toString());
        painter.drawText(2300,i,query.value(2).toString());
        painter.drawText(4300,i,query.value(3).toString());
        painter.drawText(6500,i,query.value(4).toString());
        painter.drawText(8000,i,query.value(5).toDate().toString("dd/MM/yyyy"));

        i = i +500;
    }
    int reponse = QMessageBox::question(this, "PDF généré", "Afficher le PDF ?", QMessageBox::Yes |  QMessageBox::No);
    if (reponse == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(pdfPath));
        painter.end();
    }
    if (reponse == QMessageBox::No)
    {
        painter.end();
    }
}

void MainWindow::on_envoyer_dialog_2_clicked()
{
    QString status;
    Smtp* smtp = new Smtp("neirouzghabri1@gmail.com","iltmkrbuhrvrbgye", "smtp.gmail.com", 465); //smtp.gmail.com //iltm krbu hrvr bgye
    smtp->sendMail("neirouzghabri1@gmail.com", ui->recipient_2->text() , ui->subjectLineEdit_2->text() ,ui->message_2->toPlainText());
    if(status == "Message sent")
        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ), tr( "Message sent!\n\n" ) );
}

void MainWindow::Function_Mailing() {
    QSqlQuery query1;
    query1.prepare("SELECT * FROM EQUIPEMENTS WHERE TRUNC(DATEEQ) BETWEEN TRUNC(SYSDATE) AND TRUNC(SYSDATE + 3)");
    if (query1.exec()) {
        while (query1.next()) {
            QString id = query1.value(0).toString();
            QString name = query1.value(1).toString();
            QString marque = query1.value(3).toString();
            qDebug() << "ID:" << id << "| Name:" << name << "| Brand:" << marque;
            E.write(E.time(), "Alert: ID " + id);
            Smtp* smtp = new Smtp("neirouzghabri1@gmail.com", "iltmkrbuhrvrbgye", "smtp.gmail.com", 465);
            smtp->sendMail("neirouzghabri1@gmail.com", "ayoubbezi7@gmail.com",
                           "Alerte : Notification de maintenance",
                           "ID: " + id + "|Nom :" + name + "|Marque :" + marque +
                           " | Cette date de maintenance de l'équipement expirera dans quelques jours (intervalle de 3 jours à compter de la date actuelle)");
        }
    }
}

void MainWindow::on_ClearningAlerts_clicked()
{
    E.clearh();
    ui->tb_Alertsss->setText(E.read());
}

void MainWindow::highlightDates() {
    QSqlQuery query;
    if (query.exec("SELECT STATUS, DATEEQ FROM EQUIPEMENTS")) {
        while (query.next()) {
            QString stat = query.value(0).toString();
            QString dateTimeString = query.value(1).toString();
            QDateTime dateTime = QDateTime::fromString(dateTimeString, Qt::ISODate);
            QDate date = dateTime.date();
            if (date.isValid()) {
                if(stat=="Actif"){
                    QTextCharFormat format;
                    format.setBackground(QBrush(QColor("#43a341")));
                    format.setForeground(QBrush(Qt::black));
                    format.setFontWeight(QFont::Bold);
                    ui->calendarWidget->setDateTextFormat(date, format);
                }
                if(stat=="En maintenance"){
                    QTextCharFormat format;
                    format.setBackground(QBrush(QColor("#9ba341")));
                    format.setForeground(QBrush(Qt::black));
                    format.setFontWeight(QFont::Bold);
                    ui->calendarWidget->setDateTextFormat(date, format);
                }
                if(stat=="Hors service"){
                    QTextCharFormat format;
                    format.setBackground(QBrush(QColor("#a35941")));
                    format.setForeground(QBrush(Qt::black));
                    format.setFontWeight(QFont::Bold);
                    ui->calendarWidget->setDateTextFormat(date, format);
                }
            }
        }
    }
}


void MainWindow::on_calendarWidget_selectionChanged()
{
    QDate SelectedDate = ui->calendarWidget->selectedDate();
    QSqlQuery query;
    bool dateFound = false;
    if (query.exec("SELECT * FROM EQUIPEMENTS")) {
        while (query.next()) {
            QString id = query.value(0).toString();
            QString nom = query.value(1).toString();
            QString marque = query.value(2).toString();
            QString loc = query.value(3).toString();
            QString stat = query.value(4).toString();
            QDate date = query.value(5).toDate();
            if (SelectedDate == date) {
                ui->CalendarLabelInfo->setText(
                            "Date enregistée:<br>"
                            "<b>ID:</b> " + id + "<br>" +
                            "<b>Nom:</b> " + nom + "<br>" +
                            "<b>Marque:</b> " + marque + "<br>" +
                            "<b>Loc:</b> " + loc + "<br>" +
                            "<b>Status:</b> " + stat + "<br>"
                            );
                dateFound = true;
                break;
            }
        }
        if (!dateFound) {
            ui->CalendarLabelInfo->setText("Date est non enregistrée");
        }
    }
}

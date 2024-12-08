#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "assiduite.h"
#include "perso.h"
#include "authentification.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardItemModel>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QTableView>
#include <QFileDialog>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QSslSocket>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QPageSize>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QTextBrowser>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cond(false)
    // factureModel(new QStandardItemModel(this))
    , maintenanceModel(new QStandardItemModel(this))
    ,persoManager(new Perso(this))
    ,serial(new QSerialPort(this))
    , arduino(nullptr)


{
    ui->setupUi(this);
    //ui->home->setEnabled(false);
    connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::on_modifier_clicked);
    // Setup table headers for factureModel and maintenanceModel
    //factureModel->setHorizontalHeaderLabels({"ID Facture", "Date", "Fournisseur", "Montant Total"});
    //ui->tableViewFactures->setModel(factureModel);
    maintenanceModel->setHorizontalHeaderLabels({"ID", "Task", "Equipment", "Action", "Start Date", "End Date", "Status"});

    maintenanceProxyModel = new QSortFilterProxyModel(this);
    maintenanceProxyModel->setSourceModel(maintenanceModel);
    loadMaintenanceData();
    maintenanceProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->tab_mat->setModel(maintenanceProxyModel);
    ui->startDateEdit->setDate(QDate::currentDate());
    connect(ui->intervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateCalendarHighlights);
    connect(ui->startDateEdit, &QDateEdit::dateChanged, this, &MainWindow::updateCalendarHighlights);
    statusLabel = new QLabel("Status: Waiting for data...", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 18px; color: blue;");
    setupSerialPort();
    highestGazLabel = new QLabel(this);
    ui->verticalLayout_2->addWidget(statusLabel); // Add the label to the layout
    ui->tab_personnel->setModel(persoManager->PersonnelModel);
    persoManager->afficherPersonnel(); // Load personnel data at startup
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
    if (serial->isOpen()) {
        serial->close();
    }
    delete ui;

}

void MainWindow::loadMaintenanceData()
{
    maintenanceModel->clear();
    maintenanceModel->setHorizontalHeaderLabels({"ID", "Task", "Equipment", "Action", "Start Date", "End Date", "Status"});
    maintenanceManager.loadFromDatabase(maintenanceModel,ui->comboBox_3);
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
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(QDate::currentDate());
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

    /*if (task.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Tâche' ne peut pas être vide.");
        return; // Sortir si la tâche est vide
    }

    if (equipment.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un équipement.");
        return; // Sortir si aucun équipement n'est sélectionné
    }

   if (action.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Action' ne peut pas être vide.");
        return; // Sortir si l'action est vide
    }

*/
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

void MainWindow::on_reaficher_clicked()
{
    maintenanceModel->sort(0, Qt::AscendingOrder);
}


void MainWindow::on_reaficher_2_clicked()
{
    maintenanceModel->sort(4, Qt::AscendingOrder);
}



void MainWindow::on_reaficher_3_clicked()
{
    maintenanceModel->sort(4, Qt::DescendingOrder);
}









void MainWindow::on_recherche_clicked()
{
    QString searchText = ui->rech->text();
    if (searchText.isEmpty()) {
        maintenanceProxyModel->setFilterFixedString("");  // Réinitialiser le filtre
    } else {
        maintenanceProxyModel->setFilterKeyColumn(-1);  // Applique le filtre sur toutes les colonnes
        maintenanceProxyModel->setFilterFixedString(searchText);  // Définit le filtre de recherche
    }
}







void MainWindow::on_pushButton_4_clicked()
{

        QString fileName = QFileDialog::getSaveFileName(this, "Export to PDF", "", "*.pdf");
        if (fileName.isEmpty()) {
            return;
        }
        if (QFileInfo(fileName).suffix().isEmpty()) {
            fileName.append(".pdf");
        }

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);

        QPainter painter;
        if (!painter.begin(&printer)) {
            qDebug() << "Error opening file for writing.";
            return;
        }

        QAbstractItemModel *model = ui->tab_mat->model();
        if (!model) {
            qDebug() << "No model found for ui->tab_mat.";
            return;
        }

        // Retrieve the printable area
        QRectF pageRect = printer.pageRect(QPrinter::Millimeter); // Use millimeter units for consistency
        int margin = 10; // Margin in millimeters
        qreal availableWidth = pageRect.width() - 2 * margin;
        qreal availableHeight = pageRect.height() - 2 * margin;

        int rows = model->rowCount();
        int columns = model->columnCount();

        // Calculate cell dimensions dynamically
        qreal cellWidth = 1000;
        qreal cellHeight = 500; // Limit max cell height

        // Adjust table positioning to center it
        qreal xOffset = margin;
        qreal yOffset = margin;

        painter.translate(xOffset, yOffset);

        // Draw table headers
        QFont font("Arial", 10);
        painter.setFont(font);

        for (int col = 0; col < columns; ++col) {
            QRectF rect(col * cellWidth, 0, cellWidth, cellHeight);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, model->headerData(col, Qt::Horizontal).toString());
        }

        // Draw table content
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < columns; ++col) {
                QRectF rect(col * cellWidth, (row + 1) * cellHeight, cellWidth, cellHeight);
                painter.drawRect(rect);
                QString cellText = model->data(model->index(row, col)).toString();
                painter.drawText(rect, Qt::AlignCenter, cellText);
            }
        }

        painter.end();
        qDebug() << "PDF exported successfully to" << fileName;


}
void MainWindow::updateCalendarHighlights()
{
    // Get values from the widgets
    QDate startDate = ui->startDateEdit->date();
    int intervalDays = ui->intervalSpinBox->value();

    QTextCharFormat defaultFormat;
    ui->calendarWidget->setDateTextFormat(QDate(), defaultFormat);


    if (intervalDays <= 0) {
        QMessageBox::warning(this, "Invalid Interval", "The interval must be a positive number.");
        return;
    }

    // Clear previous highlights
    for (int i = 0; i <= intervalDays; ++i) { // Adjust range as needed
        QDate dateToClear = startDate.addDays(i);
        ui->calendarWidget->setDateTextFormat(dateToClear, defaultFormat);
    }

    // Highlight the start date
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::yellow); // Use yellow for highlighting
    highlightFormat.setForeground(Qt::black);  // Optional

    ui->calendarWidget->setDateTextFormat(startDate, highlightFormat);

    // Highlight subsequent dates based on interval
    for (int i = 1; i <= intervalDays; ++i) { // Adjust the range to highlight more dates if needed
        QDate nextDate = startDate.addDays(i-1);
        if (!nextDate.isValid()) break; // Safety check for invalid dates
        ui->calendarWidget->setDateTextFormat(nextDate, highlightFormat);//////
    }
}


void MainWindow::sendEmail(const QString &recipient, const QString &subject, const QString &body) {
    QString smtpServer = "smtp.gmail.com";
    int smtpPort = 465;  // SSL/TLS port
    QString senderEmail = "ayoubbezi7@gmail.com";
    QString senderPassword = "cmxd mhlr vdqh izwx";  // App password from Google

    QSslSocket socket;
    socket.connectToHostEncrypted(smtpServer, smtpPort);

    if (!socket.waitForEncrypted(5000)) {
        qDebug() << "Failed to connect or encrypt: " << socket.errorString();
        return;
    }

    // Send EHLO
    socket.write("EHLO localhost\r\n");
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "EHLO Failed: " << socket.errorString();
        return;
    }
    qDebug() << "EHLO Response: " << socket.readAll();

    // Authenticate with Gmail
    socket.write("AUTH LOGIN\r\n");
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "AUTH LOGIN Command Failed";
        return;
    }
    qDebug() << socket.readAll();

    socket.write(QString("%1\r\n").arg(senderEmail.toUtf8().toBase64()).toUtf8());
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "Email Address Authentication Failed";
        return;
    }
    qDebug() << socket.readAll();

    socket.write(QString("%1\r\n").arg(senderPassword.toUtf8().toBase64()).toUtf8());
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "Password Authentication Failed";
        return;
    }
    qDebug() << socket.readAll();

    // Send MAIL FROM
    socket.write(QString("MAIL FROM:<%1>\r\n").arg(senderEmail).toUtf8());
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "MAIL FROM Command Failed";
        return;
    }
    qDebug() << socket.readAll();

    // Send RCPT TO
    socket.write(QString("RCPT TO:<%1>\r\n").arg(recipient).toUtf8());
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "RCPT TO Command Failed";
        return;
    }
    qDebug() << socket.readAll();

    // Send DATA
    socket.write("DATA\r\n");
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "DATA Command Failed";
        return;
    }
    qDebug() << socket.readAll();

    // Format the email content
    QString message = QString("From: %1\r\n"
                              "To: %2\r\n"
                              "Subject: %3\r\n\r\n"
                              "%4\r\n.\r\n")
                          .arg(senderEmail)
                          .arg(recipient)
                          .arg(subject)
                          .arg(body);

    socket.write(message.toUtf8());
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "Message Sending Failed";
        return;
    }
    qDebug() << "Email Response: " << socket.readAll();

    // Send QUIT
    socket.write("QUIT\r\n");
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "QUIT Command Failed";
        return;
    }
    qDebug() << socket.readAll();

    socket.disconnectFromHost();
    qDebug() << "Email sent successfully!";
}








void MainWindow::on_pushButton_27_clicked()
{
    // Collect input data
    int equipmentId = ui->equipmentIdLineEdit->text().toInt();
    int intervalDays = ui->intervalSpinBox->value();
    QDate startDate = ui->startDateEdit->date();
    QString description = ui->descriptionTextEdit->toPlainText();
    QDate newDate = startDate.addDays(intervalDays);
    Maintenance maintenance;

    // Step 1: Save the new schedule to the MAINTENANCE_SCHEDULE table
    bool success = maintenance.scheduleMaintenance(equipmentId, intervalDays, description, startDate);

    // Step 2: If schedule was saved, check and generate tasks
    if (success) {

        // Call the function to check schedules and generate tasks
        maintenance.updateDatabase2(equipmentId,startDate,newDate,"in progress");


        // Show success message
        QMessageBox::information(this, "Task Scheduled",
                                 "The maintenance task has been successfully scheduled and updated.");
    } else {
        // If saving failed, show an error message
        QMessageBox::warning(this, "Error",
                             "Failed to schedule the maintenance task. Please check the input.");
    }
    QString recipient = "yessandsasi@gmail.com";  // Replace with actual recipient email
    QString subject = "Scheduled Maintenance Task";
    QString body = QString("Maintenance ID: %1\nStart Date: %2\nEnd Date: %3\nDescription: %4")
                       .arg(equipmentId)
                       .arg(startDate.toString("yyyy-MM-dd"))
                       .arg(newDate.toString("yyyy-MM-dd"))
                       .arg(description);

    sendEmail(recipient, subject, body);
}



void MainWindow::generateMaintenanceReport(int id)
{
    // Query the MAINTENANCE table to fetch maintenance details by ID
    QSqlQuery maintenanceQuery;
    maintenanceQuery.prepare("SELECT ID, TASK, EQUIPMENT, ACTION, TO_CHAR(START_DATE, 'DD/MM/YYYY'), "
                             "TO_CHAR(END_DATE, 'DD/MM/YYYY'), STATUS FROM MAINTENANCE WHERE ID = :id");
    maintenanceQuery.bindValue(":id", id);

    if (!maintenanceQuery.exec()) {
        ui->textEdit->setText("Failed to retrieve maintenance details: " + maintenanceQuery.lastError().text());
        return;
    }

    // Check if a record was found
    if (!maintenanceQuery.next()) {
        ui->textEdit->setText("No maintenance task found with the given ID.");
        return;
    }

    // Retrieve the maintenance data
    QString taskId = maintenanceQuery.value(0).toString();
    QString task = maintenanceQuery.value(1).toString();
    QString equipment = maintenanceQuery.value(2).toString();
    QString action = maintenanceQuery.value(3).toString();
    QString startDate = maintenanceQuery.value(4).toString();
    QString endDate = maintenanceQuery.value(5).toString();
    QString status = maintenanceQuery.value(6).toString();

    // Query the MAINTENANCE_SCHEDULE table to fetch scheduling details for the equipment ID
    QSqlQuery scheduleQuery;
    scheduleQuery.prepare("SELECT INTERVAL_DAYS, TO_CHAR(NEXT_DATE, 'DD/MM/YYYY'), DESCRIPTION, TO_CHAR(EDIT_TIME, 'DD/MM/YYYY HH24:MI:SS') "
                          "FROM MAINTENANCE_SCHEDULE WHERE EQUIPMENT_ID = :equipmentId");
    scheduleQuery.bindValue(":equipmentId", id);

    QString scheduleDetails;

    if (scheduleQuery.exec() && scheduleQuery.next()) {
        // Retrieve the scheduling data
        QString intervalDays = scheduleQuery.value(0).toString();
        QString nextDate = scheduleQuery.value(1).toString();
        QString description = scheduleQuery.value(2).toString();
        QString editTime = scheduleQuery.value(3).toString();

        // Format the scheduling details
        scheduleDetails = QString(
                              "Scheduling Details:\n"
                              "-----------------------------\n"
                              "Interval Days : %1\n"
                              "Next Date     : %2\n"
                              "Description   : %3\n"
                              "Last Edited   : %4\n"
                              ).arg(intervalDays, nextDate, description, editTime);
    } else {
        scheduleDetails = "No scheduling details found for this maintenance task.\n";
    }

    // Format the entire report
    QString report = QString(
                         "Maintenance Report\n"
                         "-----------------------------\n"
                         "Task ID       : %1\n"
                         "Task Name     : %2\n"
                         "Equipment     : %3\n"
                         "Action        : %4\n"
                         "Start Date    : %5\n"
                         "End Date      : %6\n"
                         "Status        : %7\n\n"
                         "%8"
                         ).arg(taskId, task, equipment, action, startDate, endDate, status, scheduleDetails);

    // Display the report in ui->textEdit
    ui->textEdit->setText(report);

}



void MainWindow::on_generatereport_clicked()
{
    int id = ui->comboBox_3->currentText().toInt();
    generateMaintenanceReport(id);
}


void MainWindow::on_reset_4_clicked()
{
    // Choose a file location for saving the PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Save Report as PDF", "", "PDF Files (*.pdf)");

    // Check if the user canceled the file dialog
    if (filePath.isEmpty()) {
        return;
    }

    // Create a QPdfWriter for the specified file
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));  // Set the page size to A4

    // Set up page margins if needed (optional)
    QMargins margins(30, 30, 30, 30);  // Top, Left, Bottom, Right
    pdfWriter.setPageMargins(margins);

    // Create a QPainter to write text to the PDF
    QPainter painter(&pdfWriter);

    // Retrieve the content from ui->textEdit
    QString text = ui->textEdit->toPlainText();

    // Set font for the PDF (optional, if you want to customize font)
    painter.setFont(QFont("Arial", 10));

    // Get page size from QPdfWriter and calculate the content rectangle
    //QRect contentRect(0, 0, pageSize.width(), pageSize.height());

    // Apply margins to the content rect
   // contentRect.adjust(margins.left(), margins.top(), -margins.right(), -margins.bottom());

    // Draw the text on the PDF, handling line wrapping
    //painter.drawText(contentRect, Qt::TextWordWrap | Qt::AlignLeft, text);

    // Finish painting and save the file
    painter.end();

    // Inform the user that the file was successfully saved
    QMessageBox::information(this, "Success", "Report exported successfully to:\n" + filePath);
}


QString MainWindow::searchDatabase(const QString &userMessage) {
    QSqlQuery query;
    QString sqlQuery;

    // Example: if user asks about ongoing tasks for equipment "X"
    if (userMessage.contains("ongoing tasks", Qt::CaseInsensitive) && userMessage.contains("equipment", Qt::CaseInsensitive)) {
        sqlQuery = "SELECT TASK, EQUIPMENT, START_DATE, END_DATE, STATUS FROM MAINTAINANCE WHERE STATUS = 'ongoing'";

        // Execute the query
        if (query.exec(sqlQuery)) {
            QString result;
            while (query.next()) {
                QString task = query.value("TASK").toString();
                QString equipment = query.value("EQUIPMENT").toString();
                QDate startDate = query.value("START_DATE").toDate();
                QDate endDate = query.value("END_DATE").toDate();
                QString status = query.value("STATUS").toString();

                result += QString("Task: %1\nEquipment: %2\nStart Date: %3\nEnd Date: %4\nStatus: %5\n\n")
                              .arg(task)
                              .arg(equipment)
                              .arg(startDate.toString())
                              .arg(endDate.toString())
                              .arg(status);
            }
            return result.isEmpty() ? "No ongoing tasks found for the given equipment." : result;
        } else {
            return "Error: Could not retrieve data from the database.";
        }
    }

    // If no specific condition is met, return a default response
    return "I'm not sure what you're asking about maintenance tasks.";
}

void MainWindow::sendMessageToOasisAI(const QString &userMessage, QTextBrowser *textBrowser) {
    // Your API key (make sure it is secure)
    QString apiKey = "99c8a9879fb634448afb53f00d592e86";  // Replace with your actual API key

    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

    // Oasis AI API endpoint
    QUrl apiUrl("https://api.oasis.ai/v1/chat/completions");
    QNetworkRequest request(apiUrl);

    // Set request headers, including Authorization header with the API key
    request.setRawHeader("Authorization",apiKey.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Prepare the JSON request body
    QJsonObject json;
    json["model"] = "llama8b";  // Oasis model (you can change this if needed)

    // Get database results (this can be a function that queries the database based on the user message)
    QString databaseResult = searchDatabase(userMessage);  // Search database for relevant data

    // Create the "messages" array
    QJsonArray messages;

    QJsonObject systemMessage;
    systemMessage["role"] = "system";
    systemMessage["content"] = "You are a helpful chatbot  in a smart maintainance app, you can help with facture, personel ,equipement ,ressources, and maintainance task ,and you can generally help in everything in the app but you dont have access to database.";

    QJsonObject userMessageObject;
    userMessageObject["role"] = "user";
    userMessageObject["content"] = userMessage + "\n\nDatabase result:\n" + databaseResult;

    messages.append(systemMessage);
    messages.append(userMessageObject);

    if (userMessage.contains("maintenance", Qt::CaseInsensitive)) {
        QString queryResponse = searchDatabase(userMessage);
        userMessageObject["content"] = queryResponse;  // Modify the user's query to include database results
    }

    // Add messages to the JSON object
    json["messages"] = messages;

    // Convert JSON to QByteArray
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    // Send POST request
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // Handle the response asynchronously
    QObject::connect(reply, &QNetworkReply::finished, [reply, textBrowser]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Parse the response
            QByteArray responseBytes = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseBytes);
            QJsonObject responseObject = responseDoc.object();

            // Extract chatbot's reply
            if (responseObject.contains("choices")) {
                QJsonArray choices = responseObject["choices"].toArray();
                if (!choices.isEmpty()) {
                    QString chatbotReply = choices[0].toObject()["message"].toObject()["content"].toString();
                    qDebug() << "Chatbot reply:" << chatbotReply;

                    // Format the bot's reply with HTML and time
                    QString htmlContent = QString("<div class=\"message bot-message\"><p>%1</p><span class=\"message-time\">%2</span></div>")
                                              .arg(chatbotReply)
                                              .arg(QTime::currentTime().toString("hh:mm AP"));

                    textBrowser->append(htmlContent);  // Add the bot message to the QTextBrowser
                }
            } else {
                qDebug() << "Unexpected response format:" << QString(responseBytes);
            }
        } else {
            qDebug() << "Error code:" << reply->error();
            qDebug() << "Error string:" << reply->errorString();
        }
        reply->deleteLater();
    });}

 void MainWindow::on_pushButton_28_clicked()  {
        QString userMessage = ui->lineEdit_9->text(); // Get the user message from the input field

        // Format the user message with HTML and time
        QString htmlContent = QString("<div class=\"message user-message\"><p>%1</p><span class=\"message-time\">%2</span></div>")
                                  .arg(userMessage)
                                  .arg(QTime::currentTime().toString("hh:mm AP"));

        ui->textBrowser_2->append(htmlContent); // Add the user message to the QTextBrowser

        sendMessageToOasisAI(userMessage, ui->textBrowser_2); // Send message to Oasis AI
    }
 void MainWindow::rescheduleMaintenanceTasks() {
     QDate today = QDate::currentDate();
     QDate tomorrow = today.addDays(1);

     QSqlQuery query;
     QSqlQuery query2;
     query.prepare("SELECT ID, TASK, START_DATE FROM MAINTENANCE WHERE START_DATE = TO_DATE(:today, 'YYYY-MM-DD')");
     query.bindValue(":today", today.toString("yyyy-MM-dd"));

     if (!query.exec()) {
         qDebug() << "Failed to query database:" << query.lastError().text();
         return;
     }

     while (query.next()) {
         int taskId = query.value(0).toInt();
         QString taskName = query.value(1).toString();

         // Update the task's start date
         QSqlQuery updateQuery;
         updateQuery.prepare("UPDATE MAINTENANCE SET START_DATE = TO_DATE(:tomorrow, 'YYYY-MM-DD') WHERE ID = :id");
         updateQuery.bindValue(":tomorrow", tomorrow.toString("yyyy-MM-dd"));
         updateQuery.bindValue(":id", taskId);

         if (updateQuery.exec()) {
             qDebug() << "Task rescheduled successfully: ID =" << taskId;

             // Send notification
             QString emailSubject = "Maintenance Task Rescheduled";
             QString emailBody = QString("The task '%1' scheduled for %2 has been rescheduled to %3 due to a gas leak.")
                                     .arg(taskName)
                                     .arg(today.toString("yyyy-MM-dd"))
                                     .arg(tomorrow.toString("yyyy-MM-dd"));
             sendEmail("yessandsasi@gmail.com", emailSubject, emailBody);
         } else {
             qDebug() << "Failed to update task:" << updateQuery.lastError().text();
         }
     }
 }


 void MainWindow::setupSerialPort() {
     foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
         qDebug() << "Available port:" << info.portName();
         if (info.portName().contains("COM7")) { // Replace "COM3" with your Arduino port
             serial->setPort(info);
             break;
         }
     }

     serial->setBaudRate(QSerialPort::Baud9600);
     serial->setDataBits(QSerialPort::Data8);
     serial->setParity(QSerialPort::NoParity);
     serial->setStopBits(QSerialPort::OneStop);
     serial->setFlowControl(QSerialPort::NoFlowControl);

     if (serial->open(QIODevice::ReadOnly)) {
         qDebug() << "Serial port connected!";
         connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
         statusLabel->setText("Status: Connected to Arduino");
         statusLabel->setStyleSheet("font-size: 18px; color: green;");
     } else {
         qDebug() << "Failed to open serial port!";
         statusLabel->setText("Status: Serial connection failed");
         statusLabel->setStyleSheet("font-size: 18px; color: red;");
     }
 }

 void MainWindow::readSerialData() {
     while (serial->canReadLine()) {
         QString line = serial->readLine().trimmed();
         qDebug() << "Received:" << line;

         if (line.startsWith("Sensor Value:")) {
             // Extract numeric value
             QString valueStr = line.mid(13).trimmed();
             bool ok;
             int sensorValue = valueStr.toInt(&ok);

             if (ok) {
                 qDebug() << "Parsed Sensor Value:" << sensorValue;
                ui->label_38->setText(QString("            %1  ").arg(sensorValue));

                 // Update the highest value
                 if (sensorValue > highest) {
                     highest = sensorValue;
                     qDebug() << "Updated highest value to:" << highest;
                 }

                 // Update the lowest value
                 if (sensorValue < lowest) {
                     lowest = sensorValue;
                     qDebug() << "Updated lowest value to:" << lowest;
                 }

                 // Show warning or normal status based on the sensor value
                 if (sensorValue >= 50) { // Replace `GasThreshold` with your threshold value
                     statusLabel->setText("WARNING: GAS LEAK DETECTED!");
                     statusLabel->setStyleSheet("font: 700 italic 22pt 'Segoe UI'; color: red; background-color: yellow;");
                     ui->label_38->setStyleSheet("color: red; font: 700 italic 36pt 'Segoe UI';");
                     rescheduleMaintenanceTasks();
                 } else {
                     statusLabel->setText("Status: Normal");
                     statusLabel->setStyleSheet("font: 700 italic 22pt 'Segoe UI'; color: green;");
                     ui->label_38->setStyleSheet("color: green; font: 700 italic 36pt 'Segoe UI';");
                 }
             } else {
                 qDebug() << "Failed to parse integer from line:" << valueStr;
             }
         } else {
             qDebug() << "Non-sensor line received:" << line;
         }
     }
 }


 void MainWindow::on_btnHighestGaz_clicked() {
     // Simply display the highest value
     ui->label_3->setText(QString("Maximum Sensor Value recorded: %1").arg(highest));
     qDebug() << "Final highest value:" << highest;
 }








 void MainWindow::on_btnLowestGaz_clicked()
 {
     qDebug() << "Starting to find the lowest sensor value...";

     // Display the lowest gas value on the label
     ui->label_7->setText(QString("Minimum Sensor Value recorded: %1").arg(lowest));
     qDebug() << "Final lowest value:" << lowest;
 }


 void MainWindow::on_pushButton_29_clicked()
 {
     // Get the current timestamp
     QDateTime currentDateTime = QDateTime::currentDateTime();
     QString timestamp = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");  // Ensure it's in the right format

     // Prepare the SQL query to insert data into the GAZVARIATION table
     QSqlQuery query;

     query.prepare("INSERT INTO GAZVARIATION (HIGHESTGAZVALUE, LOWESTGAZVALUE, DATE_VER) "
                   "VALUES (:highest, :lowest, TO_TIMESTAMP(:date, 'YYYY-MM-DD HH24:MI:SS'))");

     // Bind the values
     query.bindValue(":highest", highest);
     query.bindValue(":lowest", lowest);
     query.bindValue(":date", timestamp);

     // Execute the query and check if it was successful
     if (query.exec()) {
         qDebug() << "Data saved successfully!";
         // Optionally, show a message box or label to indicate success
         ui->label_39->setText("Data saved successfully.");
         ui->label_39->setStyleSheet("font-size: 18px; color: green;");
     } else {
         qDebug() << "Error saving data:" << query.lastError().text();
         // Optionally, show a message box or label to indicate failure
         ui->label_39->setText("Error saving data.");
         ui->label_39->setStyleSheet("font-size: 18px; color: red;");
     }
 }
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







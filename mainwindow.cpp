#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ressources.h"
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QButtonGroup>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlDatabase>
#include <QPdfWriter>
#include <QPainter>
#include <QTableView>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QDebug>
#include <QDebugStateSaver>
#include <QLabel>

MainWindow::~MainWindow()
{
    if (arduino->isOpen()) {
        arduino->close();
    }
    delete ui;
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
void MainWindow::on_ajouter_clicked(){

    if (ui->code->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Code field cannot be empty.");
        return;
    }
        static QRegularExpression regex("^\\d+$");
        QRegularExpressionMatch match = regex.match(ui->code->text());
        if (!match.hasMatch()) {
            QMessageBox::warning(this, "Input Error", "Code field must contain only numbers.");
            return;
        }
        int code = ui->code->text().toInt();
        QString description = ui->descr->text();
        QString categorie = ui->categorie->currentText();
        QString date = ui->calendarWidget->selectedDate().toString("dd/MM/yyyy");
        int quantite =ui->qu->text().toInt();

        // Create an instance of the ressources class
        ressources res;
        ui->tablev->setModel(res.afficher());
        // Attempt to add to the database
        if (res.addToDatabase(code, description, categorie, date,quantite)) {
            QMessageBox::information(this, "Success", "Ressource added successfully.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to add the ressource to the database.");
        }
}

MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
    ui = new Ui::MainWindow;
    ui->setupUi(this);
    ui->tablev->setModel(res.afficher());
    ui->tablev->resizeColumnsToContents();
    ui->tablev->resizeRowsToContents();
    arduino=new QSerialPort(this);
    ui->categorie->addItem("");
    ui->categorie->addItem("Electronics");
    ui->categorie->addItem("Materials");
    ui->categorie->addItem("Spare parts");
    ui->cat_2->addItem("");
    ui->cat_2->addItem("Electronics");
    ui->cat_2->addItem("Materials");
    ui->cat_2->addItem("Spare parts");
    ui->sit->addItem("");
    ui->sit->addItem("Normal");
    ui->sit->addItem("Urgent");
    ui->sit->addItem("Tres urgent");
    ui->status->addItem("Not verified");
    ui->status->addItem("In progress");
    ui->status->addItem("Verified");
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.vendorIdentifier() == 0x2341 || info.vendorIdentifier() == 0x1A86) { // Replace with your Arduino's vendor ID
            arduinoPortName = info.portName();
            break;
        }
    }

    if (!arduinoPortName.isEmpty()) {
        arduino->setPortName(arduinoPortName);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);

        if (arduino->open(QIODevice::ReadOnly)) {
            connect(arduino, &QSerialPort::readyRead, this, &MainWindow::readFromArduino);
            qDebug() << "Connected to Arduino on" << arduinoPortName;
        } else {
            qDebug() << "Failed to open port" << arduinoPortName;
        }
    } else {
        qDebug() << "No Arduino device found.";
    }
}
void MainWindow::on_suppr_clicked()
{
    // Retrieve the code to delete from the input field
    QString codeText = ui->id_supp_2->text().trimmed(); // Ensure no leading/trailing spaces
    if (codeText.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Please enter a valid code.");
        return;
    }

    int codeToDelete = codeText.toInt();
    if (codeToDelete <= 0) { // Validate the code
        QMessageBox::warning(this, "Avertissement", "The code must be a positive number.");
        return;
    }

    QSqlQuery query;

    // Prepare the DELETE statement
    query.prepare("DELETE FROM RESSOURCES WHERE CODE = :code");
    query.bindValue(":code", codeToDelete); // Bind the integer value
    ui->tablev->setModel(res.afficher());
    // Execute the query and handle the result
    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            QMessageBox::information(this, "Succès", "Record deleted successfully.");
        } else {
            QMessageBox::warning(this, "Avertissement", "No record found with the given code.");
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Failed to execute query: " + query.lastError().text());
    }

    // Clear the input field after the operation
    ui->id_supp_2->clear();
}

void MainWindow::readFromArduino() {
    static QByteArray buffer; // Buffer to store incoming data
    buffer.append(arduino->readAll()); // Append new data to the buffer

    // Process lines in the buffer
    while (buffer.contains('\n')) {
        int newlineIndex = buffer.indexOf('\n'); // Find the newline character
        QByteArray line = buffer.left(newlineIndex).trimmed(); // Extract the complete line
        buffer.remove(0, newlineIndex + 1); // Remove the processed line from the buffer

        QString dataStr = QString::fromUtf8(line);

        // Parse temperature line
        if (dataStr.startsWith("Température :")) {
            QString temp = dataStr.split(":")[1].split("°C")[0].trimmed();
            ui->tempLabel->setText( ":"+temp + " °C");
            insertIntoDatabase(temp,QString());
            if(temp>70){
                QMessageBox::warning(this, "Danger", "Incendie dans l'entrepôt .");

            }
            else if(temp<0){
                QMessageBox::warning(this, "Danger", "temperature tres bas .");

            }
        }
        // Parse humidity line
        else if (dataStr.startsWith("Humidité :")) {
            QString hum = dataStr.split(":")[1].split("%")[0].trimmed();
            ui->humLabel->setText(":" + hum + " %");
            insertIntoDatabase(QString(), hum);
            if(hum>80){
                QMessageBox::warning(this, "Danger", "humidté tres haute dans l'entrepôt .");

            }
            else if(hum<20){
                QMessageBox::warning(this, "Danger", "humidite tres bas .");

            }
        } else {
            qDebug() << "Unknown data received:" << dataStr;
        }
    }
}
void MainWindow::insertIntoDatabase(const QString &temp, const QString &hum) {
    QSqlQuery query;
    query.prepare("INSERT INTO TEMPERATURE (TEMP, HUMIDTY) VALUES (:temp, :hum)");
    query.bindValue(":temp", temp.isEmpty() ? QVariant(QVariant::String) : temp);
    query.bindValue(":hum", hum.isEmpty() ? QVariant(QVariant::String) : hum);

    if (!query.exec()) {
        qDebug() << "Failed to insert data into database:" << query.lastError().text();
    } else {
        qDebug() << "Data inserted successfully: Temp=" << temp << ", Humidity=" << hum;
    }
}





void MainWindow::on_Mod_3_clicked()
{
    static QRegularExpression regex("^\\d+$");
    QRegularExpressionMatch match = regex.match(ui->code->text());
    if (!match.hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Code field must contain only numbers.");
        return;
    }
    int code = ui->code->text().toInt();
    QString description = ui->descr->text();
    QString categorie = ui->categorie->currentText();
    QString date = ui->calendarWidget->selectedDate().toString("dd/MM/yyyy");
    int quant =ui->qu->text().toInt();


    ressources res;
    ui->tablev->setModel(res.afficher());
    // Attempt to add to the database
    if (res.updateInDatabase(code, description, categorie, date,quant)) {
        QMessageBox::information(this, "Success", "Ressource modified successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to modify the ressource in the database.");
    }
}


void MainWindow::on_rechercher_clicked()
{
    // Get the search term from the UI (e.g., a QLineEdit)
    QString searchTerm = ui->rech_2->text();

    // Prepare the SQL query to search across all columns
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Modify the SQL query to search in all columns: CODE, DESCRIPTION, CATEGORIE, DATE
    query.prepare("SELECT CODE, DESCRIPTION, CATEGORIE, DATEE, QUANTITE FROM RESSOURCES WHERE CODE LIKE :searchTerm OR DESCRIPTION LIKE :searchTerm OR CATEGORIE LIKE :searchTerm OR DATEE LIKE :searchTerm OR QUANTITE LIKE :searchTerm");

    // Bind the search term (using wildcards for partial matching)
    query.bindValue(":searchTerm", "%" + searchTerm + "%");

    // Execute the query and check for errors
    if (query.exec()) {
        // Set the results in the model
        model->setQuery(query);

        // Set custom headers for the columns
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("CODE"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("DESCRIPTION"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("CATEGORIE"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("QUANTITE"));

        // Display results in QTableView
        ui->tablev->setModel(model);
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(model);

        // Enable sorting on the QTableView
        ui->tablev->setModel(proxyModel);
        ui->tablev->setSortingEnabled(true);
        updateSortOrder();

    } else {
        // Show a warning if no records found or error occurs
        QMessageBox::warning(this, "Search", "No records found or an error occurred.");
        delete model;
    }
}
void MainWindow::updateSortOrder()
{

    // Sort by the currently sorted column (default: first column)
    int currentColumn = ui->tablev->horizontalHeader()->sortIndicatorSection();

}

void MainWindow::on_pdf_clicked()
{
    // Open a file dialog to choose where to save the PDF
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) {
        return;  // Cancelled or no file selected
    }

    // Get the model from the table view
    QAbstractItemModel *model = ui->tablev->model();
    if (!model) {
        QMessageBox::warning(this, tr("Error"), tr("No data to export."));
        return;
    }

    // Create an HTML string for the table
    QString html;
    html.append("<html><head><style>");
    html.append("table { border-collapse: collapse; width: 100%; }");
    html.append("th, td { border: 1px solid black; padding: 8px; text-align: center; }");
    html.append("th { background-color: #f2f2f2; }");
    html.append("</style></head><body>");
    html.append("<h2 align='center'>Exported Table</h2>");
    html.append("<table>");

    // Add table headers
    html.append("<tr>");
    for (int col = 0; col < model->columnCount(); ++col) {
        QString headerText = model->headerData(col, Qt::Horizontal).toString();
        html.append(QString("<th>%1</th>").arg(headerText));
    }
    html.append("</tr>");

    // Add table rows
    for (int row = 0; row < model->rowCount(); ++row) {
        html.append("<tr>");
        for (int col = 0; col < model->columnCount(); ++col) {
            QString cellText = model->data(model->index(row, col)).toString();
            html.append(QString("<td>%1</td>").arg(cellText));
        }
        html.append("</tr>");
    }

    html.append("</table></body></html>");

    // Create a QTextDocument for rendering HTML
    QTextDocument document;
    document.setHtml(html);

    // Create the PDF writer
    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(100);  // Set resolution for better quality
    writer.setPageMargins(QMarginsF(15, 15, 15, 15));

    // Paint the HTML content onto the PDF
    QPainter painter(&writer);
    document.drawContents(&painter);
    painter.end();

    // Notify the user that the export is complete
    QMessageBox::information(this, tr("Export to PDF"), tr("The table has been successfully exported to PDF."));
}


void MainWindow::on_ajouter_2_clicked() {
    int code = ui->ccode->text().toInt();
    QString statuss = ui->status->currentText();

    if (code <= 0 || statuss.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid code and status.");
        return;
    }

    ressources res;
    if (res.addToDatabase_2(statuss,code)) {
        QMessageBox::information(this, "Success", "Control added successfully.");
        ui->code->clear();
        ui->status->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the control. Ensure the CODE exists in RESSOURCES.");
    }
}




void MainWindow::on_Mod_5_clicked()
 {
        int code = ui->ccode->text().toInt();
        QString newStatus = ui->status->currentText();

        // Ensure the inputs are valid
        if (code <= 0 || newStatus.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter a valid code and select a status.");
            return;
        }

        ressources res;
        if (res.modifyControl(code, newStatus)) {
            QMessageBox::information(this, "Success", "Control updated successfully.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to update the control. Ensure the CODE exists in the database.");
        }



}
 void MainWindow::animateProgressBar(int targetValue, const QString &style) {
     // Set the style of the progress bar
     ui->progressBar->setStyleSheet(style);

     // Create an animation for the progress bar value
     QPropertyAnimation *animation = new QPropertyAnimation(ui->progressBar, "value");
     animation->setDuration(1000); // Animation duration in milliseconds
     animation->setStartValue(ui->progressBar->value());
     animation->setEndValue(targetValue);
     animation->setEasingCurve(QEasingCurve::OutCubic); // Smooth transition curve
     animation->start(QAbstractAnimation::DeleteWhenStopped); // Automatically delete animation when done
 }



void MainWindow::on_check_clicked()
{
    int code = ui->ccode_2->text().toInt();

    // Ensure the input is valid
    if (code <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid CODE.");
        return;
    }

    ressources res;
    QString situation = res.getSituation(code);

    if (situation.isEmpty()) {
        QMessageBox::critical(this, "Error", "No record found for the given CODE.");
        animateProgressBar(0, "QProgressBar { background: white; color: black; }"
                              "QProgressBar::chunk { background: red; }");
        return;
    }

    // Update the progress bar based on the situation
    if (situation == "Not verified") {
        animateProgressBar(0, "QProgressBar { background: white; color: black; }"
                              "QProgressBar::chunk { background: red; }");
    } else if (situation == "In progress") {
        animateProgressBar(50, "QProgressBar { background: white; color: black; }"
                               "QProgressBar::chunk { background: orange; }");
    } else if (situation == "Verified") {
        animateProgressBar(100, "QProgressBar { background: white; color: black; }"
                                "QProgressBar::chunk { background: green; }");
    } else {
        QMessageBox::warning(this, "Warning", "Unknown situation status.");
        animateProgressBar(0, "QProgressBar { background: white; color: black; }"
                              "QProgressBar::chunk { background: gray; }");
    }
}

void MainWindow::on_ex_clicked() {
    QString codeText = ui->codeccc->text().trimmed();
    QString situationText = ui->sit->currentText().trimmed(); // Get the selected situation
    QString categoryText = ui->cat_2->currentText().trimmed();

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Build the query string without SITUATION (since it's deleted from the database)
    QString queryString = "SELECT CODE, CATEGORIE FROM RESSOURCES WHERE 1=1";

    if (!codeText.isEmpty()) {
        queryString += " AND CODE LIKE :code";
        query.bindValue(":code", "%" + codeText + "%");
    }

    if (!categoryText.isEmpty() && categoryText != "All") {
        queryString += " AND CATEGORIE LIKE :category";
        query.bindValue(":category", "%" + categoryText + "%");
    }

    query.prepare(queryString);

    qDebug() << "Query String: " << queryString;  // Debugging the query string

    // Debug the bound values
    qDebug() << "Code: " << codeText;
    qDebug() << "Situation: " << situationText;
    qDebug() << "Category: " << categoryText;

    if (!query.exec()) {
        QMessageBox::critical(this, "SQL Error", "Failed to execute query: " + query.lastError().text());
        return;
    }

    model->setQuery(query);

    // Step 3: Export to PDF
    exportToPDF(model,codeText, situationText,categoryText);  // Pass the situation text to the export function

    delete model;
}

void MainWindow::exportToPDF(QSqlQueryModel *model,const QString &codeText ,const QString &situationText,const QString &categoryText) {
    // Ask the user where to save the PDF
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) {
        QMessageBox::information(this, "No File Selected", "Please select a location to save the PDF.");
        return; // No file selected, abort export
    }

    // Create a QTextDocument to hold the HTML content
    QTextDocument document;

    // Generate the HTML content
    QString html = "<html>";
    html += "<head><style>";
    html += "table { width: 100%; border-collapse: collapse; }";
    html += "th, td { border: 1px solid black; padding: 8px; text-align: left; }";
    html += "th { background-color: #f2f2f2; }";
    html += "h1, h2 { text-align: center; }";
    html += "</style></head>";
    html += "<body>";

    // Add company name and title
    html += "<h1>Company Name</h1>";
    html += "<h2>Resource Request Report</h2>";

    // Add a professional request message
    html += "<p>Dear Sir/Madam,</p>";
    html += "<p>We kindly request the following resources for our project:</p>";
    html += "<p>Please find the details of the resources requested below. "
            "We appreciate your prompt attention to this matter.</p>";
    html += "<p>Thank you for your cooperation.</p>";
    html += "<p>Best regards,</p>";
    html += "<p>Your Company Name</p>";

    // Add the table with the model data
    html += "<table>";
    html += "<tr><th>Code</th><th>Category</th><th>Situation</th></tr>"; // No situation in table since it's not in the database
    html +="<tr><td>"+codeText+"</td><td>"+categoryText+"</td><td>"+situationText+"</td></tr>";
    for (int row = 0; row < model->rowCount(); ++row) {
        html += "<tr>";
        html += "<td>" + model->data(model->index(row, 0)).toString() + "</td>";
        html += "<td>" + model->data(model->index(row, 1)).toString() + "</td>";
        html += "</tr>";
    }

    html += "</table>";
    html += "</body></html>";

    // Set the generated HTML to the QTextDocument
    document.setHtml(html);

    // Create a QPdfWriter to save the document as a PDF
    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(150); // High-quality resolution
    writer.setPageMargins(QMargins(20, 20, 20, 20));

    // Use QPainter to render the QTextDocument
    QPainter painter(&writer);
    document.drawContents(&painter);
    painter.end();

    // Inform the user of successful export
    QMessageBox::information(this, "Export Complete", "The PDF report has been successfully generated.");
}





void MainWindow::on_pushButton_8_clicked()
{
    ui->code->clear();
    ui->descr->clear();
    ui->categorie->setCurrentIndex(0);
    ui->calendarWidget->setSelectedDate(QDate::currentDate());


}
void MainWindow::on_btn_alerts_clicked() {
    ressources res;
    QList<QPair<QString, QString>> alerts = res.getAlertDetails();
    ui->lert->setText(QString::number(res.getAlertCount()));


    if (alerts.isEmpty()) {
        QMessageBox::information(this, "No Alerts", "There are no alerts at the moment.");
        return;
        ui->lert->setText("0");
    }

    ui->tabWidget_9->setCurrentIndex(3);

    QPair<QString, QString> alert = alerts.first();
    ui->codeccc->setText(alert.first);
    ui->cat_2->setCurrentText(alert.second);
}



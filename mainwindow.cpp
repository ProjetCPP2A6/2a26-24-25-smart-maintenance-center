#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTableView>
#include <QAbstractItemModel>
#include <QPageSize>
#include "AnalyseFacture.h"
#include <QSqlQueryModel>
#include <QGraphicsDropShadowEffect>
#include <QPdfWriter>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QSerialPort>
QString currentImagePath;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      cond(false),
      factureModel(new QStandardItemModel(this)) // Initialize the model

{
    ui->setupUi(this);

    connect(ui->btnAnalyzeStatus, &QPushButton::clicked, this, &MainWindow::on_AnalyzePaymentStatus_Clicked);
    connect(ui->rechercherButton, &QPushButton::clicked, this, &MainWindow::rechercherFacture);
    // Removed the database connection code
    // qDebug() << QSqlDatabase::drivers(); // This was used to check available drivers
    ui->home->setEnabled(false);
    factureModel->setHorizontalHeaderLabels({"ID _facture", "Date", "Fournisseur", "Montant Total","prix_produit","quantite","statut"});
    ui->tableViewFactures->setModel(factureModel); // Link model to QTableView
    ui->tabWidget->repaint();
    qDebug() << "Current image path:" << currentImagePath;
    QString description = ui->descriptionEdit->text();
    tableModel = new QStandardItemModel(0, 3, this);  // 0 rows, 2 columns
    tableModel->setHorizontalHeaderLabels({"Image", "Description","authentification"});
    ui->tableView->setModel(tableModel);
    ui->tableView->verticalHeader()->setDefaultSectionSize(150);
    ui->imageLabel->setFixedSize(300, 300);  // Adjust to a larger fixed size
    ui->descriptionEdit->setStyleSheet("QLineEdit { color: blue; }");
    ui->tableView->setColumnWidth(0, 490);

           // Connect buttons to slots
    connect(ui->UploadImage, &QPushButton::clicked, this, &MainWindow::on_UploadImage_Clicked);
    connect(ui->AddToTab, &QPushButton::clicked, this, &MainWindow::on_AddToTab_Clicked);
    // Initialize the table view
    supplierModel = new QStandardItemModel(0, 2, this);  // 0 rows, 2 columns
    supplierModel->setHeaderData(0, Qt::Horizontal, "Fournisseur");
    supplierModel->setHeaderData(1, Qt::Horizontal, "Coordonnées");
    ui->tableViewFactures->setSortingEnabled(true);
    ui->tableViewFournisseur->setModel(supplierModel);
    ui->tableViewFournisseur->setRowHeight(0, 400);
     ui->tableViewFournisseur->setColumnWidth(0, 400);
        // Connect the refresh button
    connect(ui->triantQuantite, &QPushButton::clicked, this, &MainWindow::on_triantQuantite_clicked);
    connect(ui->triantPrix, &QPushButton::clicked, this, &MainWindow::on_triantPrix_clicked);
    connect(ui->exportPdfButtonFournisseur, &QPushButton::clicked, this, &MainWindow::on_exportPdfButtonFournisseur_clicked);
    applyBlueTextToLineEdits();
    salarieModel = new QStandardItemModel(this);
    salarieModel->setHorizontalHeaderLabels({"ID Salarié", "Nom", "Salaire"});
    ui->tableViewSalaries->setModel(salarieModel);



}


MainWindow::~MainWindow()
{
    delete ui;
    if (arduino->isOpen()) {
            arduino->close();  // Assurez-vous de fermer le port série lors de la destruction
        }
}





void MainWindow::on_pushButtonAddSalarie_clicked() {
    // Récupération des données des champs de saisie
    QString idSalarie = ui->lineEditIDSalarie->text();
    QString nomSalarie = ui->lineEditNomSalarie->text();
    QString salaireStr = ui->lineEditSalaire->text();

    // Validation des données saisies
    if (idSalarie.isEmpty() || nomSalarie.isEmpty() || salaireStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    bool ok;
    double salaire = salaireStr.toDouble(&ok);
    if (!ok || salaire < 0) {
        QMessageBox::warning(this, "Erreur", "Salaire invalide !");
        return;
    }

    // Vérification de l'existence du salarié dans le modèle
    bool salarieExists = false;
    for (int row = 0; row < salarieModel->rowCount(); ++row) {
        QString existingID = salarieModel->item(row, 0)->text();  // Colonne 0 : ID du salarié
        if (existingID == idSalarie) {
            salarieExists = true;
            break;
        }
    }

    if (salarieExists) {
        QMessageBox::warning(this, "Erreur", "Ce salarié existe déjà !");
        return;
    }

    // Ajout du salarié au modèle
    QList<QStandardItem *> row;
    row << new QStandardItem(idSalarie);
    row << new QStandardItem(nomSalarie);
    row << new QStandardItem(QString::number(salaire, 'f', 2));
    salarieModel->appendRow(row);

    // Réinitialisation des champs de saisie
    ui->lineEditIDSalarie->clear();
    ui->lineEditNomSalarie->clear();
    ui->lineEditSalaire->clear();

    // Mettre à jour le montant total des salaires
    mettreAJourMontantTotal();
}


void MainWindow::mettreAJourMontantTotal() {
    double total = 0.0;

    // Parcourir tous les salaires dans le modèle
    for (int row = 0; row < salarieModel->rowCount(); ++row) {
        QString salaireStr = salarieModel->item(row, 2)->text();  // Supposons que le salaire est dans la troisième colonne
        total += salaireStr.toDouble();
    }

    // Afficher le montant total dans un champ de texte
    ui->lineEditMontantTotal->setText(QString::number(total, 'f', 2));
}

void MainWindow::applyBlueTextToLineEdits()
{
    QList<QLineEdit *> lineEdits = this->findChildren<QLineEdit *>();
    for (QLineEdit *lineEdit : lineEdits) {
        lineEdit->setStyleSheet("color: blue;");
    }
}
void MainWindow::exportTableFournisseurToPdf(const QString &filePath) {
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4); // Correct usage
    pdfWriter.setResolution(300);

    // Set margins for the PDF
    int leftMargin = 100;
    int topMargin = 100;
    int rightMargin = 100;
    int bottomMargin = 100;

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        QMessageBox::warning(this, tr("Error"), tr("Could not create PDF file."));
        return;
    }

    // Title for the PDF
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(leftMargin, topMargin, "Fournisseur List");
    painter.setFont(QFont("Arial", 12));

    int y = topMargin + 400; // Starting y position after the title
    int lineHeight = 100; // Increased line height for more space between rows
    int columnWidth = 500; // Adjusted column width for better readability

    // Draw table headers
    int x = leftMargin; // Starting X position for the first column
    for (int col = 0; col < ui->tableViewFournisseur->model()->columnCount(); ++col) {
        QString header = ui->tableViewFournisseur->model()->headerData(col, Qt::Horizontal).toString();
        painter.drawText(x, y, header);
        x += columnWidth; // Move X position for the next column
    }

    y += lineHeight; // Move to the first row of data

    // Draw table rows
    for (int row = 0; row < ui->tableViewFournisseur->model()->rowCount(); ++row) {
        x = leftMargin; // Reset X position for each new row
        for (int col = 0; col < ui->tableViewFournisseur->model()->columnCount(); ++col) {
            QModelIndex index = ui->tableViewFournisseur->model()->index(row, col);
            QString cellData = ui->tableViewFournisseur->model()->data(index).toString();
            painter.drawText(x, y, cellData);
            x += columnWidth; // Move X position for the next column
        }
        y += lineHeight; // Move to the next row

        // Start a new page if content exceeds page height
        if (y > pdfWriter.height() - bottomMargin) {
            pdfWriter.newPage();
            y = topMargin + 40; // Reset y for the new page
        }
    }

    painter.end();

    QMessageBox::information(this, tr("Success"), tr("PDF exported successfully!"));
}
void MainWindow::on_exportPdfButtonFournisseur_clicked() {
    // Open file dialog to get the file path
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (!filePath.isEmpty()) {
        // Call export function to generate the PDF
        exportTableFournisseurToPdf(filePath);
    }
}

void MainWindow::on_triantPrix_clicked()
{
    int col = 4;  // Assuming 'prixProduit' is the 6th column (index 5)
    Qt::SortOrder order = ui->tableViewFactures->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder ?
                          Qt::DescendingOrder : Qt::AscendingOrder;

    ui->tableViewFactures->sortByColumn(col, order);
}
void MainWindow::on_triantQuantite_clicked()
{
    // Obtenir l'index de la colonne 'quantité' (index de la colonne 5 par exemple)
    int col = 5; // ajustez en fonction de la colonne exacte dans votre modèle

    // Tri croissant ou décroissant selon l'état actuel
    Qt::SortOrder order = ui->tableViewFactures->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder ?
                          Qt::DescendingOrder : Qt::AscendingOrder;

    ui->tableViewFactures->sortByColumn(col, order);
}

// Exemple d'un signal de tri spécifique pour Prix
void MainWindow::on_exportPDFButton_clicked()
{
    // Open a file dialog to select the PDF save location
    QString filePath = QFileDialog::getSaveFileName(this, "Export PDF", "", "*.pdf");
    if (filePath.isEmpty())
        return;

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive))
        filePath += ".pdf";

    // Configure QPrinter for PDF output
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    QPainter painter(&printer);
    // Add a title at the top of the PDF
        QFont titleFont("Arial", 25, QFont::Bold); // Title font
        painter.setFont(titleFont);
        QRectF pageRect = printer.pageRect(QPrinter::Point);
        QRectF titleRect(pageRect.x(), pageRect.y(), pageRect.width(), 100);
        painter.drawText(titleRect, Qt::AlignCenter, "Tab Facture Analyse");


        // Move down for graphics content
        int titleHeight = 50; // Adjust this to control the space between the title and the content
        painter.translate(0, titleHeight);

    // Export graphicsViewAnalyse if it has content
    QGraphicsScene *scene = ui->graphicsViewAnalyse->scene();
    if (scene) {
        QRectF sceneRect = scene->sceneRect();
        QSizeF pageSize = printer.pageRect(QPrinter::Point).size();
        qreal scale = qMin(pageSize.width() / sceneRect.width(), pageSize.height() / sceneRect.height());
        scale *= 0.20;
        painter.scale(scale, scale);
        // Center the content
        qreal xOffset = (pageSize.width() / scale - sceneRect.width()) / 2.0;
        qreal yOffset = (pageSize.height() / scale - sceneRect.height() - titleHeight / scale) / 2.0;
        painter.translate(xOffset, yOffset);
        scene->render(&painter);
        painter.resetTransform();  // Reset painter for additional content
    }

    // Optionally, add more content to the PDF (like a table or text)
    // Example: painter.drawText(...);

    painter.end();

    QMessageBox::information(this, "Success", "PDF exported successfully to " + filePath);
}






void MainWindow::on_UploadImage_Clicked()
{

// Open file dialog to select an image

QString filePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg)");

if (!filePath.isEmpty()) {

QPixmap pixmap(filePath);
pixmap = pixmap.scaled(150, 150, Qt::KeepAspectRatio); // Adjust to match row height
currentImagePath = filePath;

// Display the image in the QLabel

ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio));

ui->imageLabel->setAlignment(Qt::AlignCenter);
ui->imageLabel->setFixedSize(300, 300);  // Adjust to a larger fixed size


// Save the image path to use it later when adding to the tab

QMessageBox::information(this, "Success", "Image uploaded successfully!");

} else {

QMessageBox::warning(this, "Error", "No image selected.");

}
}
void MainWindow::on_AddToTab_Clicked()
{
    if (currentImagePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No image uploaded!");
        return;
    }

    QString description = ui->descriptionEdit->text();
    if (description.isEmpty()) {
        QMessageBox::warning(this, "Error", "Description is empty!");
        return;
    }

    // Create a new row in the model
    QList<QStandardItem *> rowItems;

    // Add image as the first item (assuming you want to store image path or icon)
    QStandardItem *imageItem = new QStandardItem();
    QPixmap pixmap(currentImagePath);
    pixmap = pixmap.scaled(500, 500); // Adjust image size
    imageItem->setIcon(QIcon(pixmap)); // Set the image as an icon
    imageItem->setSizeHint(QSize(500, 500)); // Set the cell size

    imageItem->setIcon(QIcon(pixmap));
    rowItems.append(imageItem);

    // Add description as the second item
    QStandardItem *descriptionItem = new QStandardItem(description);
    rowItems.append(descriptionItem);
    QStandardItem *authItem = new QStandardItem();
        authItem->setCheckable(true);  // Set the item to be checkable
        authItem->setCheckState(Qt::Unchecked);  // Initially, the checkbox is unchecked
        rowItems.append(authItem);
    // Add the row to the model
    tableModel->appendRow(rowItems);

    // Clear the inputs for the next upload
    ui->imageLabel->clear();
    ui->descriptionEdit->clear();
    currentImagePath.clear();
}



void MainWindow::on_AnalyzeMonth_Clicked()

{

AnalyseFacture analyser;

analyser.analyzeInvoicesByMonth(ui->tableViewFactures, ui->graphicsViewAnalyse);

}

void MainWindow::on_AnalyzePaymentStatus_Clicked()

{

AnalyseFacture analyser;

analyser.analyzePaymentStatus(ui->tableViewFactures, ui->graphicsViewAnalyse);

}

// Add Invoice
void MainWindow::on_pushButtonAddFacture_clicked() {

      // Get the text entered in the line edit
    QString idFacture = ui->lineEditID->text();
    QString date = ui->lineEditDate->text();
    QString fournisseur = ui->lineEditFournisseur->text();
    QString montantTotal = ui->lineEditMontant->text();
    QString prixProduit= ui->lineEditPrixProduit->text();
    QString quantite = ui->lineEditquantite->text();
    QString statut = ui->lineEditstatut->text();
    // Check if fournisseurName is already in tableViewFournisseur
        QAbstractItemModel *sourceModel = ui->tableViewFournisseur->model();
        bool fournisseurExists = false;

        // Check if fournisseur already exists in the model
        for (int row = 0; row < sourceModel->rowCount(); ++row) {
            QString existingFournisseur = sourceModel->data(sourceModel->index(row, 0)).toString();  // Assuming the fournisseur is in the first column
            if (existingFournisseur == fournisseur) {
                fournisseurExists = true;
                break;
            }
        }

        // If fournisseur doesn't exist, add it to the model
        if (!fournisseurExists) {
            // Add the fournisseur to the table
            int newRow = supplierModel->rowCount();
            supplierModel->insertRow(newRow);
            supplierModel->setData(supplierModel->index(newRow, 0), fournisseur);
            supplierModel->setData(supplierModel->index(newRow, 1), "");  // Optionally leave coordinates empty for now
        }

    // Add to model
    QList<QStandardItem *> row;
    row << new QStandardItem(idFacture);
    row << new QStandardItem(date);
    row << new QStandardItem(fournisseur);
    row << new QStandardItem(montantTotal);
    row << new QStandardItem(prixProduit);
    row << new QStandardItem(quantite);
     row << new QStandardItem(statut);
    factureModel->appendRow(row);
    ui->lineEditID->clear();
    ui->lineEditDate->clear();
    ui->lineEditFournisseur->clear();
    ui->lineEditquantite->clear();
    ui->lineEditPrixProduit->clear();
    ui->lineEditstatut->clear();
    ui->lineEditMontant->clear();
    // Add to database
    QSqlQuery query;
        query.prepare("INSERT INTO FACTURES (ID_FACTURES, DATE_FACTURES, FOURNISEUR, QUANTITÉ, PRIX_PRODUIT) "
                      "VALUES (:id, TO_DATE(:date, 'YYYY-MM-DD'), :fournisseur, :quantite, :prixProduit)");
        query.bindValue(":id", idFacture);
        query.bindValue(":date", date);
        query.bindValue(":fournisseur", fournisseur);
        query.bindValue(":quantite", quantite);      // Conversion de QString en int pour la quantité
        query.bindValue(":prixProduit", prixProduit); // Conversion de QString en double pour le prix

        // Exécuter la requête et gérer les erreurs
        //if (!query.exec()) {
            //qDebug() << "Erreur lors de l'insertion : " << query.lastError();
           // QMessageBox::warning(this, "Erreur", "Échec de l'ajout de la facture dans la base de données: " + query.lastError().text());
            //return;
        //}
        QMessageBox::information(this, "Succès", "Facture ajoutée avec succès dans la base de données.");

            // Réinitialiser les champs de saisie
            ui->lineEditID->clear();
            ui->lineEditDate->clear();
            ui->lineEditFournisseur->clear();
            ui->lineEditquantite->clear();
            ui->lineEditPrixProduit->clear();

        }

// Modify Invoice
void MainWindow::exportTableToPdf(const QString &filePath) {
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4); // Correct usage
    pdfWriter.setResolution(300);

    // Set margins for the PDF
    int leftMargin = 100;
    int topMargin = 100;
    int rightMargin = 100;
    int bottomMargin = 100;

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        QMessageBox::warning(this, tr("Error"), tr("Could not create PDF file."));
        return;
    }

    // Title for the PDF
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(leftMargin, topMargin, "Tab Facture");
    painter.setFont(QFont("Arial", 70));

    int y = topMargin + 400; // Starting y position after the title
    int lineHeight = 100; // Increased line height for more space between rows
    int columnWidth = 500; // Increased column width for better readability

    // Draw table headers
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    int x = leftMargin; // Starting X position for the first column
    for (int col = 0; col < ui->tableViewFactures->model()->columnCount(); ++col) {
        QString header = ui->tableViewFactures->model()->headerData(col, Qt::Horizontal).toString();
        painter.drawText(x, y, header);
        x += columnWidth; // Move X position for the next column
    }

    y += lineHeight; // Move to the first row of data

    // Draw table rows
    painter.setFont(QFont("Arial", 12));
    for (int row = 0; row < ui->tableViewFactures->model()->rowCount(); ++row) {
        x = leftMargin; // Reset X position for each new row
        for (int col = 0; col < ui->tableViewFactures->model()->columnCount(); ++col) {
            QModelIndex index = ui->tableViewFactures->model()->index(row, col);
            QString cellData = ui->tableViewFactures->model()->data(index).toString();
            painter.drawText(x, y, cellData);
            x += columnWidth; // Move X position for the next column
        }
        y += lineHeight; // Move to the next row

        // Start a new page if content exceeds page height
        if (y > pdfWriter.height() - bottomMargin) {
            pdfWriter.newPage();
            y = topMargin + 40; // Reset y for the new page
        }
    }

    painter.end();

    QMessageBox::information(this, tr("Success"), tr("PDF exported successfully!"));
}


void MainWindow::on_exportPdfButton_clicked() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (!filePath.isEmpty()) {
        exportTableToPdf(filePath);
    }
}

void MainWindow::rechercherFacture() {
    // Récupérer l'ID de la facture depuis le QLineEdit
    QString factureId = ui->lineEditFournisseurSearch->text();

    if (factureId.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de facture.");
        return;
    }

    bool found = false;

    // Parcourir les lignes du modèle pour trouver la facture correspondant à l'ID recherché
    for (int row = 0; row < factureModel->rowCount(); ++row) {
        // Vérifier si la valeur de la colonne "ID Facture" correspond à l'ID recherché
        QStandardItem *item = factureModel->item(row, 0);  // Colonne 0 = "ID Facture"

        if (item && item->text() == factureId) {
            // Si trouvé, afficher la ligne (mettre en surbrillance ou autre action)
            ui->tableViewFactures->selectRow(row); // Sélectionne la ligne correspondante
            found = true;
        }
    }

    if (!found) {
        QMessageBox::information(this, "Aucun résultat", "Aucune facture trouvée pour cet ID de facture.");
    }
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
        QModelIndex index = factureModel->index(i, 0);
        if (factureModel->data(index).toInt() == factureId) {
            // Update in model
            factureModel->setData(factureModel->index(i, 1), nouveauFournisseur);
            factureModel->setData(factureModel->index(i, 2), nouveauMontant);
            factureModel->setData(factureModel->index(i, 3), nouvelleDate);

            // Update in database
            QSqlQuery query;
            query.prepare("UPDATE Factures SET Fournisseur = :fournisseur, MontantTotal = :montantTotal, Date = :date "
                          "WHERE ID = :id");
            query.bindValue(":id", factureId);
            query.bindValue(":fournisseur", nouveauFournisseur);
            query.bindValue(":montantTotal", nouveauMontant);
            query.bindValue(":date", nouvelleDate);

            if (query.exec()) {
                QMessageBox::information(this, "Succès", "Facture modifiée avec succès.");
            } else {
                QMessageBox::warning(this, "Erreur", "Échec de la modification de la facture dans la base de données: " + query.lastError().text());
            }

            factureTrouvee = true;
            break;
        }
    }

    if (!factureTrouvee) {
        QMessageBox::warning(this, "Erreur", "Facture non trouvée.");
    }

    ui->tableViewFactures->reset();
}


// Delete Invoice
void MainWindow::on_pushButtonSupprimerFacture_clicked() {
    QString idFactureASupprimer = ui->lineEditSupprimerID->text();

    for (int row = 0; row < factureModel->rowCount(); ++row) {
        if (factureModel->item(row, 0)->text() == idFactureASupprimer) {
            // Remove from model
            factureModel->removeRow(row);

            // Remove from database
            QSqlQuery query;
            query.prepare("DELETE FROM Factures WHERE ID = :id");
            query.bindValue(":id", idFactureASupprimer);

            if (!query.exec()) {
                QMessageBox::warning(this, "Erreur", "Échec de la suppression de la facture dans la base de données: " + query.lastError().text());
            } else {
                QMessageBox::information(this, "Succès", "Facture supprimée avec succès.");
            }

            ui->lineEditSupprimerID->clear();
            return;
        }
    }

    QMessageBox::warning(this, "Erreur", "Facture avec cet ID non trouvée.");
}
void MainWindow::on_pushButtonSearchFournisseur_clicked() {
    // Récupérer le nom du fournisseur depuis le champ de saisie
    QString fournisseur = ui->lineEditFournisseur_2->text(); // Remplacez ici

    int factureCount = 0;

    // Parcourir les lignes du modèle pour compter les factures du fournisseur
    for (int i = 0; i < factureModel->rowCount(); ++i) {
        QStandardItem *item = factureModel->item(i, 2); // Colonne "Fournisseur"
        if (item && item->text() == fournisseur) {
            factureCount++;
        }
    }

    // Calculer et mettre à jour la barre de progression
    int regularitePercentage = (factureCount >= 4) ? 100 : (factureCount * 25);
    ui->progressBarRegularite->setValue(regularitePercentage);
    qDebug() << "Nombre de factures pour" << fournisseur << ":" << factureCount;
    }



// Additional Button Functions for Navigation
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
void MainWindow::on_home_clicked() {
    if (cond) ui->stackedWidget->setCurrentIndex(7);
    else ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_close_2_clicked() { this->close(); }
void MainWindow::on_pushButton_20_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_pushButton_19_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_pushButton_23_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_pushButton_22_clicked() { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::on_pushButton_21_clicked() { ui->stackedWidget->setCurrentIndex(6); ui->home->setEnabled(false); }

void MainWindow::on_rechercherButton_clicked()
{

}


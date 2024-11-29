#include "ExportPDF.h"
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QPageSize> // Pour utiliser QPageSize::A4

void ExportPDF::exportTableViewToPDF(QTableView *tableView)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);

    // Demander un fichier de sortie
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }

    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOrientation(QPrinter::Portrait);

    QPainter painter;
    if (!painter.begin(&printer)) {
        qWarning() << "Impossible de commencer à dessiner dans le fichier PDF.";
        return;
    }

    // Récupération des données du QTableView
    QAbstractItemModel *model = tableView->model();
    int rowCount = model->rowCount();
    int columnCount = model->columnCount();

    int x = 50, y = 50, cellWidth = 100, cellHeight = 30;

    // Dessiner les en-têtes
    for (int col = 0; col < columnCount; ++col) {
        QRect rect(x + col * cellWidth, y, cellWidth, cellHeight);
        painter.drawText(rect, Qt::AlignCenter, model->headerData(col, Qt::Horizontal).toString());
    }
    y += cellHeight;

    // Dessiner les lignes de données
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < columnCount; ++col) {
            QRect rect(x + col * cellWidth, y, cellWidth, cellHeight);
            QString data = model->data(model->index(row, col)).toString();
            painter.drawText(rect, Qt::AlignCenter, data);
        }
        y += cellHeight;
    }

    painter.end();
    qDebug() << "PDF exporté avec succès à :" << fileName;
}


#ifndef EXPORTPDF_H
#define EXPORTPDF_H

#include <QTableView> // Nécessaire pour utiliser QTableView

class ExportPDF
{
public:
    // Méthode statique pour exporter un QTableView en PDF
    static void exportTableViewToPDF(QTableView *tableView);
};

#endif // EXPORTPDF_H

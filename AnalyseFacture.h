#ifndef ANALYSEFACTURE_H
#define ANALYSEFACTURE_H

#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QDate>

class AnalyseFacture : public QObject
{
    Q_OBJECT

public:
    explicit AnalyseFacture(QObject *parent = nullptr);
    ~AnalyseFacture();

    void analyzeInvoicesByMonth(QTableView *tableView, QGraphicsView *graphicsView);
    void analyzePaymentStatus(QTableView *tableView, QGraphicsView *graphicsView);

private:
    QStandardItemModel* extractModel(QTableView *tableView);
};

#endif // ANALYSEFACTURE_H

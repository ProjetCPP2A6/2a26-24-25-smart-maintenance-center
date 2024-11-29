#include "AnalyseFacture.h"
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QDate>
#include <QMap>
#include <cmath>
#include <QDebug>

AnalyseFacture::AnalyseFacture(QObject *parent)
    : QObject(parent)
{
}

AnalyseFacture::~AnalyseFacture()
{
}

// Helper: Extract the model from QTableView
QStandardItemModel* AnalyseFacture::extractModel(QTableView *tableView)
{
    return qobject_cast<QStandardItemModel*>(tableView->model());
}

// Analyze invoices by month (Bar Chart)
void AnalyseFacture::analyzeInvoicesByMonth(QTableView *tableView, QGraphicsView *graphicsView)
{
    QStandardItemModel *model = extractModel(tableView);
    if (!model) return;

    QMap<QString, int> monthInvoiceCount;

    // Count invoices by month
    for (int row = 0; row < model->rowCount(); ++row) {
        QDate date = QDate::fromString(model->item(row, 2)->text(), "yyyy-MM-dd"); // Assuming column 2 is 'date'
        QString month = date.toString("yyyy-MM");

        if (!month.isEmpty())
            monthInvoiceCount[month]++;
    }

    // Create a graphics scene for the bar chart
    QGraphicsScene *scene = new QGraphicsScene();
    int barWidth = 50, spacing = 10;
    int x = 0, maxHeight = 200;

    // Draw bars for each month
    for (auto it = monthInvoiceCount.begin(); it != monthInvoiceCount.end(); ++it) {
        int barHeight = it.value() * 10; // Scale factor
        scene->addRect(x, maxHeight - barHeight, barWidth, barHeight, QPen(Qt::black), QBrush(Qt::blue));

        // Add month label below the bar
        QGraphicsTextItem *label = scene->addText(it.key());
        label->setPos(x, maxHeight + 10);
        x += barWidth + spacing;
    }

    // Set the scene for the graphics view
    graphicsView->setScene(scene);
    graphicsView->setMinimumSize(800, 400);
}

// Analyze payment status (Pie Chart)
void AnalyseFacture::analyzePaymentStatus(QTableView *tableView, QGraphicsView *graphicsView)
{
    QStandardItemModel *model = extractModel(tableView);
    if (!model) return;

    QMap<QString, int> paymentStatusCount;

    // Count payment statuses
    for (int row = 0; row < model->rowCount(); ++row) {
        QString status = model->item(row, 6)->text(); // Assuming column 6 is 'statut'

        if (!status.isEmpty())
            paymentStatusCount[status]++;
    }

    // Create a graphics scene for the pie chart
    QGraphicsScene *scene = new QGraphicsScene();
    QRectF rect(50, 50, 300, 300);  // Circle for the pie chart
    int startAngle = 0;

    // Draw each slice of the pie chart
    for (auto it = paymentStatusCount.begin(); it != paymentStatusCount.end(); ++it) {
        // Calculate the angle for the slice
        int spanAngle = static_cast<int>(360.0 * (double(it.value()) / model->rowCount()));

        // Define the center of the pie and the radius
        QPointF center = rect.center();
        int radius = 100;  // Adjust radius size as needed

        // Define the starting angle of the slice (accumulating with each slice)
        int startAngleInDegrees = startAngle;
        int endAngleInDegrees = startAngle + spanAngle;

        // Create the pie slice (polygon with 3 points: center, start, and end points)
        QPolygonF slice;
        slice << center  // Center of the pie
              << QPointF(center.x() + radius * cos(startAngleInDegrees * M_PI / 180),
                         center.y() + radius * sin(startAngleInDegrees * M_PI / 180))  // Start point
              << QPointF(center.x() + radius * cos(endAngleInDegrees * M_PI / 180),
                         center.y() + radius * sin(endAngleInDegrees * M_PI / 180));    // End point

        // Create the QGraphicsPolygonItem (slice of the pie)
        QGraphicsPolygonItem *pieSlice = new QGraphicsPolygonItem(slice);

        // Set the color for the slice: Green for "paye", Red for "non-paye"
        if (it.key().contains("paye", Qt::CaseInsensitive)) {
            pieSlice->setBrush(QBrush(Qt::green));  // Green for paid status
        } else {
            pieSlice->setBrush(QBrush(Qt::red));    // Red for unpaid status
        }

        // Add the slice to the scene
        scene->addItem(pieSlice);

        // Add label for each slice to display the status and count
        QGraphicsTextItem *label = scene->addText(it.key() + " (" + QString::number(it.value()) + ")");
        label->setPos(center.x() + 1.2 * radius * cos((startAngleInDegrees + spanAngle / 2.0) * M_PI / 180),
                      center.y() + 1.2 * radius * sin((startAngleInDegrees + spanAngle / 2.0) * M_PI / 180));

        // Update start angle for the next slice
        startAngle += spanAngle;
    }


    // Set the scene for the graphics view
    graphicsView->setScene(scene);
    graphicsView->setMinimumSize(400, 400);
}

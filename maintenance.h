#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <QString>
#include <QStandardItemModel>

class Maintenance
{
public:
    Maintenance() = default;
    bool addToDatabase(int id, const QString &task, const QString &equipment, const QString &action, const QString &startDate, const QString &endDate, const QString &status);
    bool deleteFromDatabase(int id);
    void loadFromDatabase(QStandardItemModel *model);
};

#endif // MAINTENANCE_H

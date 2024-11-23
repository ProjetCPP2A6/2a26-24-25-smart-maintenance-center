#ifndef MAINTENANCE_H
#define MAINTENANCE_H
#include <QComboBox>
#include <QString>
#include <QStandardItemModel>

class Maintenance
{
public:
    Maintenance() = default;
    bool addToDatabase(int id, const QString &task, const QString &equipment, const QString &action, const QString &startDate, const QString &endDate, const QString &status);
    bool deleteFromDatabase(int id);
    bool updateDatabase(int id, const QString &task, const QString &equipment, const QString &action, const QString &startDate, const QString &endDate, const QString &status);
    bool updateDatabase2(int id, const QDate &startDate, const QDate &endDate, const QString &status);
    void loadFromDatabase(QStandardItemModel *model, QComboBox *comboBox);
    bool scheduleMaintenance(int equipmentId, int intervalDays, const QString &description, const QDate &startDate);
    void checkAndGenerateTasks(QStandardItemModel *model);
};

#endif // MAINTENANCE_H

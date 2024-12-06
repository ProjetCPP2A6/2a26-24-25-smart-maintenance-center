#include "maintenance.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItem>
#include <QDebug>
#include <QDate>


bool Maintenance::addToDatabase(int id, const QString &task, const QString &equipment, const QString &action, const QString &startDate, const QString &endDate, const QString &status)
{
    QSqlQuery query;
    query.prepare("INSERT INTO MAINTENANCE (ID, TASK, EQUIPMENT, ACTION, START_DATE, END_DATE, STATUS) "
                  "VALUES (:id, :task, :equipment, :action, TO_DATE(:startDate, 'DD/MM/YYYY'), TO_DATE(:endDate, 'DD/MM/YYYY'), :status)");
    query.bindValue(":id", id);
    query.bindValue(":task", task);
    query.bindValue(":equipment", equipment);
    query.bindValue(":action", action);
    query.bindValue(":startDate", startDate);
    query.bindValue(":endDate", endDate);
    query.bindValue(":status", status);

    if (!query.exec()) {
        qDebug() << "Add to database failed: " << query.lastError();
        return false;
    }
    return true;
}

bool Maintenance::deleteFromDatabase(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM MAINTENANCE WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Delete from database failed: " << query.lastError();
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Maintenance::updateDatabase(int id, const QString &task, const QString &equipment, const QString &action, const QString &startDate, const QString &endDate, const QString &status)
{
    QSqlQuery query;
    query.prepare("UPDATE MAINTENANCE "
                  "SET TASK = :task, EQUIPMENT = :equipment, ACTION = :action, "
                  "START_DATE = TO_DATE(:startDate, 'DD/MM/YYYY'), "
                  "END_DATE = TO_DATE(:endDate, 'DD/MM/YYYY'), STATUS = :status "
                  "WHERE ID = :id");
    query.bindValue(":id", id);
    query.bindValue(":task", task);
    query.bindValue(":equipment", equipment);
    query.bindValue(":action", action);
    query.bindValue(":startDate", startDate);
    query.bindValue(":endDate", endDate);
    query.bindValue(":status", status);

    if (!query.exec()) {
        qDebug() << "Update failed: " << query.lastError();
        return false;
    }
    return true;
}
bool Maintenance::updateDatabase2(int id, const QDate &startDate, const QDate &endDate, const QString &status)
{
    QSqlQuery query;
    query.prepare("UPDATE MAINTENANCE "
                  "SET STATUS = :status, " // Comma added here
                  "START_DATE = TO_DATE(:startDate, 'DD/MM/YYYY'), "
                  "END_DATE = TO_DATE(:endDate, 'DD/MM/YYYY') "
                  "WHERE ID = :id");
    query.bindValue(":id", id);
    query.bindValue(":startDate", startDate.toString("dd/MM/yyyy")); // Ensure proper format
    query.bindValue(":endDate", endDate.toString("dd/MM/yyyy"));
    query.bindValue(":status", status); // Use the input status

    if (!query.exec()) {
        qDebug() << "Update failed: " << query.lastError();
        return false;
    }
    return true;
}

// maintenance.cpp
void Maintenance::loadFromDatabase(QStandardItemModel *model, QComboBox *comboBox)
{
    QSqlQuery query("SELECT ID, TASK, EQUIPMENT, ACTION, TO_CHAR(START_DATE, 'DD/MM/YYYY'), TO_CHAR(END_DATE, 'DD/MM/YYYY'), STATUS FROM MAINTENANCE");
    while (query.next()) {
        // Create a row for the table model
        QList<QStandardItem *> row;
        row << new QStandardItem(query.value(0).toString());  // ID
        row << new QStandardItem(query.value(1).toString());  // TASK
        row << new QStandardItem(query.value(2).toString());  // EQUIPMENT
        row << new QStandardItem(query.value(3).toString());  // ACTION
        row << new QStandardItem(query.value(4).toString());  // START_DATE
        row << new QStandardItem(query.value(5).toString());  // END_DATE
        row << new QStandardItem(query.value(6).toString());  // STATUS

        // Append the row to the model
        model->appendRow(row);

        // Add the ID to the combo box
        comboBox->addItem(query.value(0).toString());
    }
}

bool Maintenance::scheduleMaintenance(int equipmentId, int intervalDays, const QString &description, const QDate &startDate)
{
    QSqlQuery query;
    QDateTime dateprog = QDateTime::currentDateTime();
    query.prepare("INSERT INTO MAINTENANCE_SCHEDULE (EQUIPMENT_ID, INTERVAL_DAYS, NEXT_DATE, DESCRIPTION, EDIT_TIME) "
                  "VALUES (:equipmentId, :intervalDays, :nextDate, :description, :dateprog)");
    query.bindValue(":equipmentId", equipmentId);
    query.bindValue(":intervalDays", intervalDays);
    query.bindValue(":nextDate", startDate.addDays(intervalDays));
    query.bindValue(":description", description);
     query.bindValue(":dateprog", dateprog);


    if (!query.exec()) {
        qDebug() << "Failed to schedule maintenance: " << query.lastError();
        return false;
    }
    return true;
}



#include "maintenance.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItem>
#include <QDebug>

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

void Maintenance::loadFromDatabase(QStandardItemModel *model)
{
    QSqlQuery query("SELECT ID, TASK, EQUIPMENT, ACTION, TO_CHAR(START_DATE, 'DD/MM/YYYY'), TO_CHAR(END_DATE, 'DD/MM/YYYY'), STATUS FROM MAINTENANCE");

    while (query.next()) {
        QList<QStandardItem *> row;
        row << new QStandardItem(query.value(0).toString());
        row << new QStandardItem(query.value(1).toString());
        row << new QStandardItem(query.value(2).toString());
        row << new QStandardItem(query.value(3).toString());
        row << new QStandardItem(query.value(4).toString());
        row << new QStandardItem(query.value(5).toString());
        row << new QStandardItem(query.value(6).toString());
        model->appendRow(row);
    }
}

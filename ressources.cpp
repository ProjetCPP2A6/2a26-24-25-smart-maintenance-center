#include "ressources.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlRecord>
#include <QTableView>
ressources::ressources(QObject *parent)
    : QObject(parent){

}



ressources::~ressources() { }

bool ressources::addToDatabase(int code, const QString &desc, const QString &cate, const QString &date) {
    // Check if the date is in the correct format (DD/MM/YYYY)
    QRegExp dateRegex("^\\d{2}/\\d{2}/\\d{4}$");
    if (!dateRegex.exactMatch(date)) {
        qDebug() << "Invalid date format. Expected DD/MM/YYYY.";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO RESSOURCES (CODE, DESCRIPTION, CATEGORIE, DATEE) "
                  "VALUES (:code, :description, :categorie, TO_DATE(:date, 'DD/MM/YYYY'))");

    // Bind values
    query.bindValue(":code", code);
    query.bindValue(":description", desc);
    query.bindValue(":categorie", cate);
    query.bindValue(":date", date);

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Database insertion error:" << query.lastError().text();
        return false;
    }

    qDebug() << "Record added successfully.";
    return true;
}
bool ressources::updateInDatabase(int code, const QString &newDesc, const QString &newCate, const QString &newDate) {
    // Check if the date is in the correct format (DD/MM/YYYY)
    QRegExp dateRegex("^\\d{2}/\\d{2}/\\d{4}$");
    if (!dateRegex.exactMatch(newDate)) {
        qDebug() << "Invalid date format. Expected DD/MM/YYYY.";
        return false;
    }

    // Prepare the SQL UPDATE query
    QSqlQuery query;
    query.prepare("UPDATE RESSOURCES SET DESCRIPTION = :description, CATEGORIE = :categorie, DATEE = TO_DATE(:date, 'DD/MM/YYYY') WHERE CODE = :code");

    // Bind values to the placeholders in the query
    query.bindValue(":description", newDesc);
    query.bindValue(":categorie", newCate);
    query.bindValue(":date", newDate);
    query.bindValue(":code", code);

    // Execute the query and check if it was successful
    if (!query.exec()) {
        qDebug() << "Database update error:" << query.lastError().text();
        return false;
    }

    qDebug() << "Record updated successfully.";
    return true;
}

QSqlQueryModel* ressources:: afficher(){
    QSqlQueryModel * model=new QSqlQueryModel();
    model->setQuery("select * from ressources");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("CODE"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("DESCRIPTION"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("CATEGORIE"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("DATE"));

    return model;

}

QSqlQueryModel* ressources::searchRecords(const QString &column, const QString &searchTerm) {
    // Validate column name to prevent SQL injection (optional but recommended)
    QStringList validColumns = {"CODE", "DESCRIPTION", "CATEGORIE", "DATEE"};
    if (!validColumns.contains(column)) {
        qDebug() << "Invalid column name.";
        return nullptr;
    }

    // Create the query model
    QSqlQueryModel *model = new QSqlQueryModel();

    // Prepare the SQL query to search for records based on the given column and search term
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM RESSOURCES WHERE %1 LIKE :searchTerm").arg(column));

    // Bind the search term (using wildcards for partial matching)
    query.bindValue(":searchTerm", "%" + searchTerm + "%");

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Database search error:" << query.lastError().text();
        delete model;  // Clean up the model if there's an error
        return nullptr;
    }

    // Set the results in the model
    model->setQuery(query);

    // Set custom headers for the columns
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CODE"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("DESCRIPTION"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("CATEGORIE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE"));

    return model;
}
bool ressources::addToDatabase_2(const QString &statuss,int code) {
    QSqlQuery checkQuery;

    // Check if the code exists in the parent table
    checkQuery.prepare("SELECT COUNT(*) FROM RESSOURCES WHERE CODE = :code");
    checkQuery.bindValue(":code", code);

    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        qDebug() << "Error: CODE does not exist in RESSOURCES table.";
        return false;
    }

    // Proceed to insert into CONTROL table
    QSqlQuery query;
    query.prepare("INSERT INTO CONTROL (SITUATION, CODE) VALUES (:status, :code)");
    query.bindValue(":status", statuss);
    query.bindValue(":code", code);

    if (!query.exec()) {
        qDebug() << "Database insertion error:" << query.lastError().text();
        return false;
    }

    qDebug() << "Record added successfully.";
    return true;
}
bool ressources::modifyControl(int code, const QString &newStatus) {
    QSqlQuery query;

    // Prepare the update query
    query.prepare("UPDATE CONTROL SET SITUATION = :newStatus WHERE CODE = :code");

    // Bind values
    query.bindValue(":newStatus", newStatus);
    query.bindValue(":code", code);

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Database update error:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No record found with the given CODE.";
        return false;
    }

    qDebug() << "Record updated successfully.";
    return true;
}
QString ressources::getSituation(int code) {
    QSqlQuery query;

    // Prepare the query to fetch the SITUATION for the given CODE
    query.prepare("SELECT SITUATION FROM CONTROL WHERE CODE = :code");
    query.bindValue(":code", code);

    if (!query.exec()) {
        qDebug() << "Error fetching SITUATION:" << query.lastError().text();
        return QString();
    }

    if (query.next()) {
        return query.value(0).toString(); // Return the SITUATION value
    } else {
        qDebug() << "No record found for CODE:" << code;
        return QString();
    }
}






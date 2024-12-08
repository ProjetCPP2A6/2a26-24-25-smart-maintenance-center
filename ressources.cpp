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

bool ressources::addToDatabase(int code, const QString &desc, const QString &cate, const QString &date,int quantite) {
    // Check if the date is in the correct format (DD/MM/YYYY)
    QRegExp dateRegex("^\\d{2}/\\d{2}/\\d{4}$");
    if (!dateRegex.exactMatch(date)) {
        qDebug() << "Invalid date format. Expected DD/MM/YYYY.";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO RESSOURCES (CODE, DESCRIPTION, CATEGORIE, DATEE, QUANTITE) "
                  "VALUES (:code, :description, :categorie, TO_DATE(:date, 'DD/MM/YYYY'),:quantite)");

    // Bind values
    query.bindValue(":code", code);
    query.bindValue(":description", desc);
    query.bindValue(":categorie", cate);
    query.bindValue(":date", date);
    query.bindValue(":quantite",quantite);

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Database insertion error:" << query.lastError().text();
        return false;
    }

    qDebug() << "Record added successfully.";
    return true;
}
bool ressources::updateInDatabase(int code, const QString &newDesc, const QString &newCate, const QString &newDate,int newquantite) {
    // Check if the date is in the correct format (DD/MM/YYYY)
    QRegExp dateRegex("^\\d{2}/\\d{2}/\\d{4}$");
    if (!dateRegex.exactMatch(newDate)) {
        qDebug() << "Invalid date format. Expected DD/MM/YYYY.";
        return false;
    }

    // Prepare the SQL UPDATE query
    QSqlQuery query;
    query.prepare("UPDATE RESSOURCES SET DESCRIPTION = :description, CATEGORIE = :categorie, DATEE = TO_DATE(:date, 'DD/MM/YYYY'), QUANTITE = :quantite WHERE CODE = :code");

    // Bind values to the placeholders in the query
    query.bindValue(":description", newDesc);
    query.bindValue(":categorie", newCate);
    query.bindValue(":date", newDate);
    query.bindValue(":quantite",newquantite);
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
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("QUANTITE"));



    return model;

}

QSqlQueryModel* ressources::searchRecords(const QString &column, const QString &searchTerm) {
    // Validate column name to prevent SQL injection (optional but recommended)
    QStringList validColumns = {"CODE", "DESCRIPTION", "CATEGORIE", "DATEE","QUANTITE"};
    if (!validColumns.contains(column)) {
        qDebug() << "Invalid column name.";
        return nullptr;
    }

    // Create the query model
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare(QString("SELECT CODE ,DESCRIPTION, CATEGORIE, DATEE,QUANTITE FROM RESSOURCES WHERE %1 LIKE :searchTerm").arg(column));

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
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("QUANTITE"));



    return model;
}
bool ressources::addToDatabase_2(const QString &statuss,int code) {
    QSqlQuery checkQuery;

    checkQuery.prepare("SELECT COUNT(*) FROM RESSOURCES WHERE CODE = :code");
    checkQuery.bindValue(":code", code);

    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        qDebug() << "Error: CODE does not exist in RESSOURCES table.";
        return false;
    }

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

    query.prepare("UPDATE CONTROL SET SITUATION = :newStatus WHERE CODE = :code");

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
int ressources::getAlertCount() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM RESSOURCES WHERE ALERTE > QUANTITE");

    if (!query.exec()) {
        qDebug() << "Error retrieving alert count:" << query.lastError().text();
        return 0;
    }

    if (query.next()) {
        return query.value(0).toInt();  // Return the count of resources where ALERTE > QUANTITE
    }
    return 0;
}

QList<QPair<QString, QString>> ressources::getAlertDetails() {
    QList<QPair<QString, QString>> alerts;  // List to hold the results
    QSqlQuery query;

    // Define the SQL query to select code and category where ALERTE > QUANTITE
    QString sqlQuery = "SELECT CODE, CATEGORIE FROM RESSOURCES WHERE ALERTE > QUANTITE";

    // Log the SQL query being executed for debugging purposes
    qDebug() << "Executing query:" << sqlQuery;

    // Bind any parameters if necessary (no parameters in this case)
    if (!query.prepare(sqlQuery)) {
        qDebug() << "Failed to prepare query:" << query.lastError().text();
        return alerts;  // Return empty list if query preparation fails
    }

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return alerts;  // Return empty list if query execution fails
    }

    // Process the results
    while (query.next()) {
        // Retrieve the code and category from the current row
        QString code = query.value(0).toString();
        QString categorie = query.value(1).toString();

        // Debug: Output the fetched data
        qDebug() << "Fetched alert - Code:" << code << ", Categorie:" << categorie;

        // If both code and category are valid, append them to the result list
        if (!code.isEmpty() && !categorie.isEmpty()) {
            alerts.append(qMakePair(code, categorie));
        } else {
            qDebug() << "Skipping empty code or category.";
        }
    }

    // Log the number of alerts found
    qDebug() << "Number of alerts found:" << alerts.size();

    return alerts;  // Return the list of alerts
}







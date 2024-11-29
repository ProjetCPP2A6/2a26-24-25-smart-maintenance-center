#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

Connection::Connection() {}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");// Ensure this is the correct driver
    db.setDatabaseName("CPP_project"); // Replace with your ODBC DSN or connection name
    db.setUserName("simba"); // Replace with your database username
    db.setPassword("adem"); // Replace with the password

    if (db.open()) {
        qDebug() << "Connected to Oracle database using QODBC!";
        test = true;  // Set test to true if the connection is successful
    } else {
        qDebug() << "Connection failed: " << db.lastError().text();
    }

    return test;  // Return the result of the connection attempt
}

#include "connection.h"

connection::connection(){}

bool connection::CreateConnexion()
{
    bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet2A");
    db.setUserName("neirouz");
    db.setPassword("esprit18");
    if (db.open())
        test=true;
    return  test;
}

void connection::FermerConnexion()
{
    db.close();
}

bool connection::OuvrirConnexion()
{
    if (db.open())
        return true ;
    else
        return false;

}

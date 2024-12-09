#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("smart m m center");//inserer le nom de la source de données
    db.setUserName("firass");//inserer nom de l'utilisateur
    db.setPassword("firass");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;





    return  test;
}

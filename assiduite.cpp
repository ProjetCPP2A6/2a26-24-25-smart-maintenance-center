#include "assiduite.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>  // Include QMessageBox for message boxes

Assiduite::Assiduite(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("smartm");//inserer le nom de la source de données
    db.setUserName("ayoub");//inserer nom de l'utilisateur
    db.setPassword("esprit18");//inserer mot de passe de cet utilisateur

    if (!db.open()) {
        // Show error message in a QMessageBox
        QMessageBox::critical(nullptr, "Erreur de connexion",
                              "Erreur de connexion à la base de données:\n" + db.lastError().text());
    } else {
        // Show success message in a QMessageBox
        QMessageBox::information(nullptr, "Connexion réussie", "Connexion à la base de données réussie!");
    }
}

Assiduite::~Assiduite()
{
    // Fermeture de la connexion à la base de données
    db.close();
}

bool Assiduite::consulterAbsences(int cin)
{
    QSqlQuery query;
    query.prepare("SELECT \"DATE_ARRIVÉ\", \"DATE_FIN\", \"STATUT\" FROM ASSIDUITÉ WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        // Show error message in a QMessageBox
        QMessageBox::critical(nullptr, "Erreur de requête SQL",
                              "Erreur de requête SQL:\n" + query.lastError().text());
        return false;
    }

    absencesInfo.clear();  // Reset the information before filling it
    while (query.next()) {
        QString dateArrivee = query.value("DATE_ARRIVÉ").toString();
        QString dateFin = query.value("DATE_FIN").toString();
        QString statut = query.value("STATUT").toString();

        absencesInfo += "Date d'arrivée: " + dateArrivee + "\n";
        absencesInfo += "Date de fin: " + dateFin + "\n";
        absencesInfo += "Statut: " + statut + "\n\n";
    }

    if (absencesInfo.isEmpty()) {
        // Show message if no absences were found
        QMessageBox::information(nullptr, "Absences",
                                 "Aucune absence trouvée pour CIN: " + QString::number(cin));
        return false;
    }

    return true;
}

QString Assiduite::getAbsencesInfo() const
{
    return absencesInfo;
}

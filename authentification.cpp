#include "authentification.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>  // Inclure le module pour les boîtes de message
#include <QRegularExpression>  // Inclure la classe QRegularExpression pour la validation du CIN

Authentification::Authentification(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("smartm");//inserer le nom de la source de données
    db.setUserName("ayoub");//inserer nom de l'utilisateur
    db.setPassword("esprit18");//inserer mot de passe de cet utilisateur

    if (!db.open()) {
        qDebug() << "Erreur de connexion à la base de données:" << db.lastError().text();
    } else {
        qDebug() << "Connexion à la base de données réussie!";
    }
}

Authentification::~Authentification()
{
    // Fermeture de la connexion à la base de données
    db.close();
}

bool Authentification::enregistrerUtilisateur(int cin, const QString &motDePasse, const QString &etatCompte)
{
    // Validation des entrées (simple validation ici, à personnaliser selon vos besoins)
    if (motDePasse.isEmpty() || etatCompte.isEmpty()) {
        // Si le mot de passe ou l'état du compte est vide, afficher un message d'erreur
        QMessageBox::warning(nullptr, "Erreur d'enregistrement", "Le mot de passe ou l'état du compte est vide!");
        return false;
    }

    // Validation du CIN pour s'assurer qu'il contient uniquement des chiffres
    QString cinString = QString::number(cin);
    QRegularExpression re("^\\d+$");  // Expression régulière pour vérifier que le CIN contient uniquement des chiffres
    QRegularExpressionMatch match = re.match(cinString);

    if (!match.hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur d'enregistrement", "Le CIN doit contenir uniquement des chiffres!");
        return false;
    }

    if (cin == 0) {  // Vérifier si le CIN est invalide (cin == 0 signifie que la conversion a échoué)
        QMessageBox::warning(nullptr, "Erreur d'enregistrement", "Le CIN est invalide!");
        return false;
    }


    // Requête SQL pour insérer un nouvel utilisateur
    QSqlQuery query;
    query.prepare("INSERT INTO AUTHENTIFICATION (CIN, MOT_DE_PASSE, ETAT_COMPTE) "
                  "VALUES (:cin, :mot_de_passe, :etat_compte)");
    query.bindValue(":cin", cin);
    query.bindValue(":mot_de_passe", motDePasse);
    query.bindValue(":etat_compte", etatCompte);

    // Exécution de la requête d'insertion
    if (!query.exec()) {
        qDebug() << "Erreur d'insertion:" << query.lastError().text();
        return false;
    }

    qDebug() << "Utilisateur ajouté avec succès.";
    return true;
}

#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class Authentification : public QObject
{
    Q_OBJECT  // La macro Q_OBJECT doit être présente pour que Qt fonctionne correctement avec cette classe

public:
    explicit Authentification(QObject *parent = nullptr);  // Constructeur
    ~Authentification();  // Destructeur

    bool enregistrerUtilisateur(int cin, const QString &motDePasse, const QString &etatCompte);  // Méthode d'enregistrement

private:
    QSqlDatabase db;  // Base de données
};

#endif // AUTHENTIFICATION_H

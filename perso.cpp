#include "perso.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QSqlRecord>
#include <QRandomGenerator>
Perso::Perso(QObject *parent)
    : QObject(parent),
      PersonnelModel(new QStandardItemModel(this))
{
    PersonnelModel->setHorizontalHeaderLabels({"CIN", "Nom", "Prénom", "Date de naissance", "Adresse", "E-mail", "Téléphone", "Role","NumCarte"});
}

Perso::~Perso() { }

QString Perso::generateUniqueId(int cin)
{
    // Generate a unique ID by appending a random number (between 1000 and 9999) to the CIN
    QString uniqueId = QString::number(cin) + QString::number(QRandomGenerator::global()->bounded(1000, 9999));
    return uniqueId;
}

void Perso::ajouterPersonnel(int cin, const QString& nom, const QString& prenom, const QString& dateNaissance,
                              const QString& adresse, const QString& email, const QString& telephoneStr, const QString& role,const QString& numCarte)
{
    // Validation des entrées
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isEmpty() ||
        adresse.isEmpty() || email.isEmpty() || telephoneStr.isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur d'entrée", "Tous les champs doivent être remplis.");
        return;
    }

    bool ok;
    int telephone = telephoneStr.toInt(&ok);
    if (!ok || telephoneStr.length() != 8) {
        QMessageBox::warning(nullptr, "Erreur d'entrée", "Veuillez entrer un numéro de téléphone valide.");
        return;
    }

    // Vérification de l'unicité de CIN
    QSqlQuery checkCINQuery;
    checkCINQuery.prepare("SELECT COUNT(*) FROM PERSONNEL WHERE CIN = :cin");
    checkCINQuery.bindValue(":cin", cin);
    if (checkCINQuery.exec() && checkCINQuery.next() && checkCINQuery.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Erreur", "Ce CIN existe déjà.");
        return;
    }

    // Vérification de la connexion à la base de données
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas ouverte.");
        return;
    }

    // Générer un ID unique pour le personnel
    QString uniqueId = generateUniqueId(cin);

    // Insertion dans la base de données
    QSqlQuery query;
    query.prepare("INSERT INTO PERSONNEL (CIN, NOM, PRENOM, DATENAISS, ADRESSE, EMAIL, TELEPHONE, ROLE, NUM_CARTE) "
                                          "VALUES (:cin, :nom, :prenom, TO_DATE(:dateNaissance, 'YYYY-MM-DD'), :adresse, :email, :telephone, :role, :numCarte)");
                            query.bindValue(":numCarte", QVariant());  // Insert NULL for NUM_CARTE if not provided
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", QDate::fromString(dateNaissance, "yyyy-MM-dd"));
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":role", role);
    query.bindValue(":NUM_CARTE", NUM_CARTE);  // Bind the generated unique ID

    // Exécution de la requête
    if (query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Le personnel a été ajouté avec succès.");
        afficherPersonnel();
    } else {
        QMessageBox::warning(nullptr, "Échec", "Échec de l'ajout du personnel : " + query.lastError().text());
    }
}


void Perso::modifierPersonnel(int cin, const QString& nom, const QString& prenom, const QString& dateNaissance,
                               const QString& adresse, const QString& email, const QString& telephoneStr, const QString& role,const QString& numCarte)
{
    // Validation des entrées
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isEmpty() ||
        adresse.isEmpty() || email.isEmpty() || telephoneStr.isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur d'entrée", "Tous les champs doivent être remplis.");
        return;
    }

    bool ok;
    int telephone = telephoneStr.toInt(&ok);
    if (!ok || telephoneStr.length() != 8) {
        QMessageBox::warning(nullptr, "Erreur d'entrée", "Veuillez entrer un numéro de téléphone valide.");
        return;
    }

    // Vérification de la connexion à la base de données
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas ouverte.");
        return;
    }

    // Vérification de l'existence du personnel
    QSqlQuery query;
    query.prepare("SELECT * FROM PERSONNEL WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    if (query.exec() && query.first()) {
        // Mise à jour du personnel
        query.prepare("INSERT INTO PERSONNEL (CIN, NOM, PRENOM, DATENAISS, ADRESSE, EMAIL, TELEPHONE, ROLE, NUM_CARTE) "
                      "VALUES (:cin, :nom, :prenom, TO_DATE(:dateNaissance, 'YYYY-MM-DD'), :adresse, :email, :telephone, :role, :numCarte)");
        query.bindValue(":numCarte", QVariant());  // Insert NULL for NUM_CARTE if not provided
        query.bindValue(":cin", cin);
        query.bindValue(":nom", nom);
        query.bindValue(":prenom", prenom);
        query.bindValue(":dateNaissance", QDate::fromString(dateNaissance, "yyyy-MM-dd"));
        query.bindValue(":adresse", adresse);
        query.bindValue(":email", email);
        query.bindValue(":telephone", telephone);
        query.bindValue(":role", role);

        // Exécution de la mise à jour
        if (query.exec()) {
            QMessageBox::information(nullptr, "Succès", "Le personnel a été modifié avec succès.");
            afficherPersonnel();
        } else {
            QMessageBox::warning(nullptr, "Échec", "Échec de la modification du personnel : " + query.lastError().text());
        }
    } else {
        QMessageBox::warning(nullptr, "Non trouvé", "Aucun personnel avec ce CIN n'a été trouvé.");
    }
}

void Perso::supprimerPersonnel(int cin)
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM PERSONNEL WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        // Suppression du personnel
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM PERSONNEL WHERE CIN = :cin");
        deleteQuery.bindValue(":cin", cin);
        if (deleteQuery.exec()) {
            QMessageBox::information(nullptr, "Succès", "Le personnel a été supprimé avec succès.");
            afficherPersonnel();
        } else {
            QMessageBox::warning(nullptr, "Échec", "Échec de la suppression du personnel : " + deleteQuery.lastError().text());
        }
    } else {
        QMessageBox::warning(nullptr, "Non trouvé", "Aucun personnel avec ce CIN n'a été trouvé.");
    }
}

void Perso::afficherPersonnel()
{
    QSqlQuery query("SELECT * FROM PERSONNEL");
    PersonnelModel->setRowCount(0); // Clear previous entries

    while (query.next()) {
        QList<QStandardItem*> items;
        for (int i = 0; i < query.record().count(); ++i) {
            items.append(new QStandardItem(query.value(i).toString()));
        }
        PersonnelModel->appendRow(items); // Add new row to the model
    }
}

void Perso::trierParCIN()
{
    // Trier le modèle en fonction de la première colonne (CIN)
    PersonnelModel->sort(0, Qt::AscendingOrder);  // Tri ascendant par la première colonne (CIN)
}
void Perso::trierParAge()
{
    // Create a list to store the rows
    QList<QList<QStandardItem*>> rows;

    // Retrieve all personnel entries
    QSqlQuery query("SELECT * FROM PERSONNEL ORDER BY DATENAISS ASC");  // Order by birthdate (ascending)

    while (query.next()) {
        QList<QStandardItem*> row;
        for (int i = 0; i < query.record().count(); ++i) {
            row.append(new QStandardItem(query.value(i).toString()));
        }
        rows.append(row);  // Add the row to the list
    }

    // Now clear the model and append rows in the sorted order
    PersonnelModel->clear();
    PersonnelModel->setHorizontalHeaderLabels({"CIN", "Nom", "Prénom", "Date de naissance", "Adresse", "E-mail", "Téléphone", "Role","NumCarte"});

    // Add the sorted rows to the model
    for (const auto& row : rows) {
        PersonnelModel->appendRow(row);
    }
}
void Perso::rechercher(int cin)
{
    // Vérification que le CIN est valide
    if (cin <= 0) {
        QMessageBox::warning(nullptr, "Erreur de recherche", "Le CIN doit être valide.");
        return;
    }

    // Préparer la requête SQL pour rechercher le personnel en fonction du CIN
    QSqlQuery query;
    query.prepare("SELECT * FROM PERSONNEL WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    // Exécuter la requête
    if (query.exec()) {
        PersonnelModel->setRowCount(0); // Effacer le modèle avant de le remplir avec les résultats

        if (query.next()) {
            QList<QStandardItem*> items;
            for (int i = 0; i < query.record().count(); ++i) {
                items.append(new QStandardItem(query.value(i).toString()));
            }
            PersonnelModel->appendRow(items); // Ajouter la ligne au modèle
        } else {
            // Si aucun résultat n'est trouvé
            QMessageBox::information(nullptr, "Aucun résultat", "Aucun personnel trouvé avec ce CIN.");
        }
    } else {
        QMessageBox::warning(nullptr, "Erreur de recherche", "Échec de la recherche : " + query.lastError().text());
    }
}

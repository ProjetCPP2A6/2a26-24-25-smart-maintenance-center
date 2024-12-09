#ifndef PERSO_H
#define PERSO_H

#include <QObject>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDate>

class Perso : public QObject
{
    Q_OBJECT

public:
    explicit Perso(QObject *parent = nullptr);
    ~Perso();

    // Method to add a new personnel entry
    void ajouterPersonnel(int cin, const QString& nom, const QString& prenom, const QString& dateNaissance,
                          const QString& adresse, const QString& email, const QString& telephoneStr, const QString& role,const QString& numCarte);

    // Method to modify an existing personnel entry
    void modifierPersonnel(int cin, const QString& nom, const QString& prenom, const QString& dateNaissance,
                           const QString& adresse, const QString& email, const QString& telephoneStr, const QString& role,const QString& numCarte);

    // Method to delete a personnel entry
    void supprimerPersonnel(int cin);

    // Method to refresh the personnel display in the model
    void afficherPersonnel();

    // Method to sort personnel by CIN
    void trierParCIN();

    // Method to sort personnel by age (birthdate)
    void trierParAge();

private:
    // Helper method to validate the phone number
    bool validatePhoneNumber(const QString &phoneStr);

    // Helper method to generate a unique ID based on CIN
    QString generateUniqueId(int cin);
    // perso.h
public:
    void rechercher(int cin);  // Rendre cette méthode publique

public:
    QStandardItemModel *PersonnelModel;
};

#endif // PERSO_H

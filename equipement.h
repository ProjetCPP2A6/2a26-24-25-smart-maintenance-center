#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H
#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QTableView>
class Equipement
{
public:
    Equipement();
    Equipement(int,QString,QString,QString,QString,QDate);

    /** Getters **/
    int getId() { return ID; };
    QString getMarque() { return MARQUE; };
    QString getNom() { return NOM; };
    QString getStatus() { return STATUS; };
    QString getLocalisation() { return LOCALISATION; };
    QDate getDate() { return DATE; };

    /** Setters **/
    void setId(int ID) { this->ID = ID; };
    void setMarque(QString MARQUE) { this->MARQUE = MARQUE; };
    void setDate(QDate DATE) { this->DATE = DATE; };
    void setLocalisation(QString LOCALISATION) { this->LOCALISATION = LOCALISATION; };
    void setNom(QString NOM) { this->NOM = NOM; };
    void setStatus(QString STATUS) { this->STATUS = STATUS; };

    /** Les Functions **/
    bool Ajouter();
    bool Modifier();
    bool Supprimer(int);
    QSqlQueryModel* Afficher();
    QSqlQueryModel* Afficher_ID();
    QSqlQueryModel* Afficher_Tri_ID();
    QSqlQueryModel* Afficher_Tri_STATUS();
    QSqlQueryModel* Afficher_Tri_NOM();
    QSqlQueryModel* Afficher_Tri_MARQUE();
    QString read();
    void write(QString,QString);
    QString time();
    void clearh();
    void clearTable (QTableView* table);
    void Recherche(QTableView *table, QString x);

private:
    int ID;
    QString MARQUE,NOM,LOCALISATION,STATUS;
    QDate DATE;
};

#endif // EQUIPEMENT_H

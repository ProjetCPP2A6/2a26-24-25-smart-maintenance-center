#include "factures.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Facture::Facture(int id, const QString &date, const QString &fournisseur, double montantTotal, double prixProduit, int quantite)
    : id(id), date(date), fournisseur(fournisseur), montantTotal(montantTotal), prixProduit(prixProduit), quantite(quantite)
{
}

// Getters
int Facture::getId() const { return id; }
QString Facture::getDate() const { return date; }
QString Facture::getFournisseur() const { return fournisseur; }
double Facture::getMontantTotal() const { return montantTotal; }
double Facture::getPrixProduit() const { return prixProduit; }
int Facture::getQuantite() const { return quantite; }

// Setters
void Facture::setId(int id) { this->id = id; }
void Facture::setDate(const QString &date) { this->date = date; }
void Facture::setFournisseur(const QString &fournisseur) { this->fournisseur = fournisseur; }
void Facture::setMontantTotal(double montantTotal) { this->montantTotal = montantTotal; }
void Facture::setPrixProduit(double prixProduit) { this->prixProduit = prixProduit; }
void Facture::setQuantite(int quantite) { this->quantite = quantite; }

// Method to insert Facture into the database
bool Facture::insertIntoDatabase()
{
    QSqlQuery query;
    query.prepare("INSERT INTO FACTURES (ID_FACTURES, DATE_FACTURES, FOURNISEUR, QUANTITÉ, PRIX_PRODUIT) "
                  "VALUES (:id, TO_DATE(:date, 'YYYY-MM-DD'), :fournisseur, :quantite, :prixProduit)");
    query.bindValue(":id", id);
    query.bindValue(":date", date);
    query.bindValue(":fournisseur", fournisseur);
    query.bindValue(":quantite", quantite);
    query.bindValue(":prixProduit", prixProduit);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'insertion de la facture: " << query.lastError();
        return false;
    }
    return true;
}

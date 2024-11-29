#ifndef FACTURE_H
#define FACTURE_H

#include <QString>

class Facture
{
public:
    // Constructor
    Facture(int id, const QString &date, const QString &fournisseur, double montantTotal, double prixProduit, int quantite);

    // Getters
    int getId() const;
    QString getDate() const;
    QString getFournisseur() const;
    double getMontantTotal() const;
    double getPrixProduit() const;
    int getQuantite() const;

    // Setters
    void setId(int id);
    void setDate(const QString &date);
    void setFournisseur(const QString &fournisseur);
    void setMontantTotal(double montantTotal);
    void setPrixProduit(double prixProduit);
    void setQuantite(int quantite);

    // Method to insert Facture into the database
    bool insertIntoDatabase();

private:
    int id;
    QString date;
    QString fournisseur;
    double montantTotal;
    double prixProduit;
    int quantite;
};

#endif // FACTURE_H

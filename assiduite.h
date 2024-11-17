#ifndef ASSIDUITE_H
#define ASSIDUITE_H
#include <QObject>
#include <QSqlDatabase>
#include <QString>

class Assiduite : public QObject
{
    Q_OBJECT

public:
    explicit Assiduite(QObject *parent = nullptr);  // Constructeur
    virtual ~Assiduite();  // Destructeur virtuel

    bool consulterAbsences(int cin);  // Méthode pour consulter les absences

    QString getAbsencesInfo() const;  // Méthode pour obtenir les infos des absences

private:
    QSqlDatabase db;  // Objet de connexion à la base de données
    QString absencesInfo;  // Variable pour stocker les informations des absences
};

#endif // ASSIDUITE_H

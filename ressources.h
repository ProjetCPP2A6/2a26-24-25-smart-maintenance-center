#ifndef RESSOURCES_H
#define RESSOURCES_H
#include <QString>
#include <QSqlQuery>
#include <QComboBox>
#include <QWidget>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QObject>
#include <QTableView>

class ressources : public QObject
{
    Q_OBJECT

public:
    explicit ressources(QObject *parent = nullptr);
    ~ressources();

    int getAlertCount();
    QList<QPair<QString, QString>> getAlertDetails();
    bool addToDatabase(int code, const QString &desc, const QString &cate, const QString &date,int quantite);
    bool updateInDatabase(int code, const QString &desc, const QString &cate, const QString &date,int newquantite);
    bool addToDatabase_2(const QString &stat,int code);
    bool modifyControl(int code, const QString &newStatus);
    QString getSituation(int code) ;
    QSqlQueryModel* afficher();
    QSqlQueryModel* searchRecords(const QString &column, const QString &searchTerm);



private:

};

#endif // RESSOURCES_H

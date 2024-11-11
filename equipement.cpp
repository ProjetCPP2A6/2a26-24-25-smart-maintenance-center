#include "equipement.h"
#include "QSqlQuery"
Equipement::Equipement()
{
    ID=0;
    NOM="";
    MARQUE="";
    LOCALISATION="";
    STATUS="";
    DATE;
}
Equipement::Equipement(int ID,QString NOM,QString MARQUE,QString LOCALISATION,QString STATUS,QDate DATE)
{
    this->ID=ID;
    this->NOM=NOM;
    this->MARQUE=MARQUE;
    this->LOCALISATION=LOCALISATION;
    this->STATUS=STATUS;
    this->DATE=DATE;
}

bool Equipement::Ajouter(){
    QSqlQuery query; // aamalna type de langauge SQL
    QString id_string=QString::number(ID); //radina ID -> String
    query.prepare("INSERT INTO EQUIPEMENTS(ID,NOM,MARQUE,LOCALISATION,STATUS,DATEEQ) VALUES (:ID, :NOM, :MARQUE, :LOCALISATION, :STATUS, :DATEEQ)"); //Preparina l requet SQL eli bch najoutiw fiha les valeurs fel table mteena
    query.bindValue(":ID", ID); //aatina les valeurs chkounhom bch yethatou fel requet
    query.bindValue(":NOM", NOM);
    query.bindValue(":MARQUE", MARQUE);
    query.bindValue(":LOCALISATION", LOCALISATION);
    query.bindValue(":STATUS", STATUS);
    query.bindValue(":DATEEQ", DATE);
    return query.exec(); //w houni rajaana l execution mtaa l ajout. hiya func bool donc trajaalek cbon wala cest pas bon
}
bool Equipement::Modifier(){
    QSqlQuery query;
    query.prepare("UPDATE EQUIPEMENTS SET NOM=:NOM, MARQUE=:MARQUE, LOCALISATION=:LOCALISATION, STATUS=:STATUS,DATEEQ=:DATEEQ WHERE ID=:ID");
    query.bindValue(":NOM", NOM);
    query.bindValue(":MARQUE", MARQUE);
    query.bindValue(":LOCALISATION", LOCALISATION);
    query.bindValue(":STATUS", STATUS);
    query.bindValue(":DATEEQ", DATE);
    query.bindValue(":ID", ID);
    return query.exec();
}
bool Equipement::Supprimer(int ID){
    QSqlQuery query;
    query.prepare("DELETE FROM EQUIPEMENTS WHERE ID= :ID");
    query.bindValue(0, ID);
    return query.exec();
}
QSqlQueryModel* Equipement::Afficher(){
    QSqlQueryModel* model= new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID) AS ID, NOM, MARQUE, LOCALISATION, STATUS, TO_CHAR(DATEEQ, 'DD/MM/YYYY') AS DATEEQ FROM EQUIPEMENTS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("MARQUE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("LOCALISATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("DATEEQ"));
    return model;
}
QSqlQueryModel* Equipement ::Afficher_ID()
{
    QSqlQueryModel* model=new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID) AS ID from EQUIPEMENTS");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
    return model;
}
/************************/
QSqlQueryModel* Equipement::Afficher_Tri_ID(){
    QSqlQueryModel* model= new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID) AS ID, NOM, MARQUE, LOCALISATION, STATUS, TO_CHAR(DATEEQ, 'DD/MM/YYYY') AS DATEEQ FROM EQUIPEMENTS ORDER BY ID");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("MARQUE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("LOCALISATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("DATEEQ"));
    return model;
}
QSqlQueryModel* Equipement::Afficher_Tri_MARQUE(){
    QSqlQueryModel* model= new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID) AS ID, NOM, MARQUE, LOCALISATION, STATUS, TO_CHAR(DATEEQ, 'DD/MM/YYYY') AS DATEEQ FROM EQUIPEMENTS ORDER BY MARQUE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("MARQUE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("LOCALISATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("DATEEQ"));
    return model;
}
QSqlQueryModel* Equipement::Afficher_Tri_STATUS(){
    QSqlQueryModel* model= new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID) AS ID, NOM, MARQUE, LOCALISATION, STATUS, TO_CHAR(DATEEQ, 'DD/MM/YYYY') AS DATEEQ FROM EQUIPEMENTS ORDER BY STATUS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("MARQUE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("LOCALISATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("DATEEQ"));
    return model;
}
QSqlQueryModel* Equipement::Afficher_Tri_NOM(){
    QSqlQueryModel* model= new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID) AS ID, NOM, MARQUE, LOCALISATION, STATUS, TO_CHAR(DATEEQ, 'DD/MM/YYYY') AS DATEEQ FROM EQUIPEMENTS ORDER BY NOM");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("MARQUE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("LOCALISATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("DATEEQ"));
    return model;
}
void Equipement::clearTable(QTableView *table){
    QSqlQueryModel* modelFeragh= new QSqlQueryModel();
    modelFeragh->clear();
    table->setModel(modelFeragh);
}
void Equipement::Recherche(QTableView *table, QString x){
    QSqlQueryModel *model=new QSqlQueryModel();
    QSqlQuery *query =new QSqlQuery;
    query->prepare("SELECT TO_CHAR(ID) AS ID, NOM, MARQUE, LOCALISATION, STATUS, TO_CHAR(DATEEQ, 'DD/MM/YYYY') AS DATEEQ FROM EQUIPEMENTS where regexp_like(NOM,:NOM) OR regexp_like(STATUS,:STATUS) OR regexp_like(MARQUE,:MARQUE);");
    query->bindValue(":NOM", x);
    query->bindValue(":STATUS", x);
    query->bindValue(":MARQUE", x);
    if(x.isEmpty())
    {
        query->prepare("SELECT TO_CHAR(ID) AS ID, NOM, MARQUE, LOCALISATION, STATUS, TO_CHAR(DATEEQ, 'DD/MM/YYYY') AS DATEEQ FROM EQUIPEMENTS");
    }
    query->exec();
    model->setQuery(*query);
    table->setModel(model);
    table->show();
}

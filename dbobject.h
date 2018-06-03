#ifndef DBOBJECT_H
#define DBOBJECT_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QJsonArray>
class dbObject
{
private:
    QSqlDatabase db;

public:
    dbObject(QString, QString);
    QString ErrorText;
    QJsonArray execute(QString);
    QSqlQuery *query;
    void saveState(QString, QJsonDocument);


};

#endif // DBOBJECT_H

#ifndef DBOBJECT_H
#define DBOBJECT_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QJsonArray>
class Logger
{
private:
    QSqlDatabase db;

public:
    Logger(QString, QString);
    QString ErrorText;
    QJsonArray execute(QString);
    QSqlQuery *query;
    void saveState(QString, QJsonDocument);
    void message(unsigned char level, QString text);

};

#endif // DBOBJECT_H

#ifndef DBOBJECT_H
#define DBOBJECT_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QJsonArray>
#include <QMetaEnum>
#include "main.h"

class Logger : public QObject
{
    Q_OBJECT
    Q_ENUMS(eLevel)

private:
    QSqlDatabase m_Database;


public:
    explicit Logger(QString, QString, QObject *parent = nullptr);
    enum eLevel {       //TODO: figure out how to declare this only once (now in main.h and here)
        FATAL = 0x00, ERROR=0x01, WARN=0x02, INFO=0x04, WATCH=0x08, DEBUG=0x10,
        DATA=0x100, UART=0x200, COMMAND=0x400, NETWORK=0x800, ALL=0xff00
    };


    unsigned int m_Level=ALL|DEBUG;
    QString ErrorText;
    QSqlQuery *query;
    void saveState(QString, QJsonDocument);

public slots:
    QJsonArray execute(QString);
    void message(unsigned int level, QString text);

};

#endif // DBOBJECT_H

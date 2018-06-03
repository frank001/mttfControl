#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include "dbobject.h"

enum State { NONE, INFO, WARNING, ERROR, FATAL };

class logger
{
private:

    unsigned char logLevel;

public:
    dbObject *db;
    logger(dbObject *);
    void message(unsigned char, QString);


};

#endif // LOGGER_H

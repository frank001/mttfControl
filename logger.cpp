#include "logger.h"
#include <QDebug>
#include <QDateTime>
#include "dbobject.h"

logger::logger(dbObject *database) {
    db = database;
    logLevel = 0;
    message(0, "Logger start " + QDateTime::currentDateTime().toUTC().toString());

}


void logger::message(unsigned char level, QString text) {
    QString now = "[" + QDateTime::currentDateTime().toLocalTime().toString("yyyyMMdd hh:mm:ss") +"]: " + text;
    QByteArray ba = now.toLatin1();
    const char *c_str = ba.data();
    qInfo() << c_str;
    db->execute("insert into logControl (level, message) values (0,'" + now +"');");

}


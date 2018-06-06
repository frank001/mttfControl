#include <QCoreApplication>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

#include "dbobject.h"


Logger::Logger(QString name, QString password) {
    db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName("DRIVER={SQL Server};Server=localhost\\SQLEXPRESS;Database=mttf;Uid="+name+";Pwd="+password+";");
    this->message(0, "Logger start " + QDateTime::currentDateTime().toUTC().toString());

}

QJsonArray Logger::execute(QString sql) {
    QJsonArray records;
    if (db.open()) {
        query = new QSqlQuery;
        query->prepare(sql);
        if (!query->exec()) {   //an error occured      //TODO: review this!!!! NOT WORKING... or is it? when accessing from wrong thread it stops here...
            QSqlError err = db.lastError();
            ErrorText = err.text().replace("\\","\\\\");
            exit(EXIT_FAILURE);

        } else {    //process results
            while (query->next()) {
                QJsonObject record;
                for (unsigned char i=0;i<query->record().count();i++) {
                    QString fieldName = query->record().fieldName(i);
                    record.insert(fieldName, QJsonValue::fromVariant(query->value(i)));
                }
                records.push_back(record);
            }
            db.close();
        }
    } else {    //TODO: review this!!!! Done. Solved...
        QSqlError err = db.lastError();
        ErrorText = err.text().replace("\\","\\\\");
        qInfo() << ErrorText;
        qInfo() << "FATAL ERROR. Aborting application.";
        exit(EXIT_FAILURE);

    }
    return records;
}

void Logger::saveState(QString field, QJsonDocument jd) {
    execute("update currentState set "+field+" = '"+ jd.toJson() +"' where id=(select max(id) from currentState);");

}

void Logger::message(unsigned char level, QString text) {
    QString now = "[" + QDateTime::currentDateTime().toLocalTime().toString("yyyyMMdd hh:mm:ss") +"]: " + text;
    QByteArray ba = now.toLatin1();
    const char *c_str = ba.data();
    qInfo() << c_str;
    this->execute("insert into logControl (level, message) values (0,'" + now +"');");

}

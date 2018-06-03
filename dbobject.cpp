#include <QCoreApplication>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QJsonDocument>
#include <QJsonObject>

#include "dbobject.h"


dbObject::dbObject(QString name, QString password) {
    db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName("DRIVER={SQL Server};Server=localhost\\SQLEXPRESS;Database=mttf;Uid="+name+";Pwd="+password+";");
}

QJsonArray dbObject::execute(QString sql) {
    QJsonArray records;
    if (db.open()) {
        query = new QSqlQuery;
        query->prepare(sql);
        if (!query->exec()) {   //an error occured      //TODO: review this!!!! NOT WORKING
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

void dbObject::saveState(QString field, QJsonDocument jd) {
    execute("update currentState set "+field+" = '"+ jd.toJson() +"' where id=(select max(id) from currentState);");

}

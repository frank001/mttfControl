#include <QCoreApplication>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QMetaEnum>
#include "main.h"
#include "logger.h"


Logger::Logger(QString name, QString password, QObject *parent) : QObject(parent) {
    m_Database = QSqlDatabase::addDatabase("QODBC3");
    m_Database.setDatabaseName("DRIVER={SQL Server};Server=localhost\\SQLEXPRESS;Database=mttf;Uid="+name+";Pwd="+password+";");
    this->message(this->DATA|this->INFO, "Logger start " + QDateTime::currentDateTime().toUTC().toString());

}

QJsonArray Logger::execute(QString sql) {
    QJsonArray records;
    if (m_Database.open()) {
        query = new QSqlQuery;
        query->prepare(sql);
        if (!query->exec()) {   //an error occured      //TODO: review this!!!! NOT WORKING... or is it? when accessing from wrong thread it stops here...

            //TODO: escape the sql text!!!!, can't use '' now.

            QSqlError err = m_Database.lastError();
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
            m_Database.close();
        }
    } else {    //TODO: review this!!!! Done. Solved...
        QSqlError err = m_Database.lastError();
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

void Logger::message(unsigned int level, QString text) {
    QMetaObject MetaObject = this->staticMetaObject;
    //QMetaEnum MetaEnum = QMetaEnum::fromType<eLevel>();
    QMetaEnum MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eLevel"));

    unsigned int cur_cat = (level &0xff00);
    unsigned int cur_prio = (level &0x00ff);

    unsigned int m_prio = m_Level & 0x00ff;
    unsigned int m_cat = m_Level & 0xff00;

    if (((cur_cat)&m_cat) != cur_cat) return;   //discard unwanted categories
    if (cur_prio <= m_prio  ) {                 //only include specified priorities

        QString now = "[" + QDateTime::currentDateTime().toLocalTime().toString("yyyyMMdd hh:mm:ss") +"] "
                + MetaEnum.valueToKey(cur_cat) + "|" + MetaEnum.valueToKey(cur_prio) + ": "
                + text;
        QByteArray ba = now.toLatin1();
        const char *c_str = ba.data();
        qInfo() << c_str;
        this->execute("insert into logControl (level, message) values ("+QString::number(level)+" ,'" + now +"');");
    }

}

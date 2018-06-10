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
    message(DATA|INFO, "Logger start " + QDateTime::currentDateTime().toUTC().toString());

}

QJsonArray Logger::execute(QString sql) {
    QJsonArray records;
    QString msg = logRequired(DATA|DEBUG, "Executing query: " + sql);   //TODO: enable logging of queries, avoid loops. almost done. it logs to console only now.
    if (m_Database.open()) {
        query = new QSqlQuery;
        query->prepare(sql);
        if (!query->exec()) {   //an error occured

            //TODO: escape the sql text!!!!, can't use '' now.

            QSqlError err = m_Database.lastError();
            ErrorText = err.text().replace("\\","\\\\");                //TODO: get error text, this is not working
            QString msg = logRequired(FATAL, "LOGGER: An error occured. Aborting application.");
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
    } else {
        QSqlError err = m_Database.lastError();
        ErrorText = err.text().replace("\\","\\\\");
        QString msg = logRequired(FATAL, "LOGGER: Aborting application. Error message:\n" + ErrorText);
        exit(EXIT_FAILURE);

    }
    if (records.count()>0) {    //TODO: improve this. create function to return log yes/no.
                                //now this is executed for each query regardless of actual logging to console or database
        QJsonDocument jd;

        jd.setArray(records);
        logRequired(DATA|DEBUG, "Data: " + QString(jd.toJson()));
    }
    return records;
}

void Logger::saveState(QString field, QJsonDocument jd) {
    execute("update currentState set "+field+" = '"+ jd.toJson() +"' where id=(select max(id) from currentState);");
}

bool Logger::logRequired(unsigned int level, QString text) {
    QMetaObject MetaObject = this->staticMetaObject;
    QMetaEnum MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eLevel"));

    unsigned int cur_cat = (level &0xff00);
    unsigned int cur_prio = (level &0x00ff);

    unsigned int m_prio = m_Level & 0x00ff;
    unsigned int m_cat = m_Level & 0xff00;
    QString msg;


    if (((cur_cat)&m_cat) != cur_cat) return false;   //discard unwanted categories
    if (cur_prio > m_prio  )  return false;
    //{                 //only include specified priorities

        msg = "[" + QDateTime::currentDateTime().toLocalTime().toString("yyyyMMdd hh:mm:ss") +"] "
            + MetaEnum.valueToKey(cur_cat) + "|" + MetaEnum.valueToKey(cur_prio) + ": "
            + text;
        QByteArray ba = msg.toLatin1();
        const char *c_str = ba.data();
        qInfo() << c_str;

    //}
    return true;
}

void Logger::message(unsigned int level, QString text) {

    if (logRequired(level, text)) execute("insert into logControl (level, message) values ("+QString::number(level)+" ,'" + text +"');");

}

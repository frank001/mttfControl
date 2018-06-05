#include <QCoreApplication>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QThread>

#include "logger.h"
#include "dbobject.h"
#include "main.h"
#include "config.h"
#include "uart.h"
#include "tcpserver.h"


dbObject *db = new dbObject("mttfControl", "topSecret");
logger *Log = new logger(db);
Config *cfg = new Config(Log, db);
uart *serial;
tcpServer *socket;
Commands cmd;



int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Log->message(0, "Accessing database.");

    cfg->setConfig(db->execute("select top 1 * from currentState order by id desc;"));

    socket = new tcpServer(cfg);



    socket->startServer();

    serial = new uart(cfg);


    //cfg->setVibrate(0);
    //cfg->logLevel(0);
    //cfg->writeConfig();
    /*while (true) {
        QThread::currentThread()->sleep(10);
    }*/
    //git test

    return a.exec();
}

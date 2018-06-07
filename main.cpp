#include <QCoreApplication>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QThread>

#include "logger.h"
#include "handler.h"
#include "tcpserver.h"
#include "main.h"


Logger db("mttfControl", "topSecret");
tcpServer *tcpserver;
QSerialPort serialPort;



int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    db.message(DATA|INFO, "Accessing database.");

    Handler handler(&serialPort, &db);
    handler.setConfig(db.execute("select top 1 * from currentState order by id desc;"));

    tcpserver = new tcpServer(&handler);
    tcpserver->startServer();


    return a.exec();
}

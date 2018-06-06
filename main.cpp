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
#include <QThread>


dbObject *db = new dbObject("mttfControl", "topSecret");
logger *Log = new logger(db);
//Config *cfg; // = new Config(Log, db);

tcpServer *socket;
//Commands cmd;

QSerialPort serialPort;


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    serialPort.setPortName("COM1");
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.open(QIODevice::WriteOnly);

    Config cfg(&serialPort);

    Log->message(0, "Accessing database.");

    cfg.setConfig(db->execute("select top 1 * from currentState order by id desc;"));

    socket = new tcpServer(&cfg);
    socket->startServer();


    serialPort.setPortName("COM1");
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.open(QIODevice::WriteOnly);
    cfg.serialPort = &serialPort;

    //uart Writer(&serialPort);
    //Writer.write("l7\r");







    //cfg->setVibrate(0);
    //cfg->logLevel(0);
    //cfg->writeConfig();
    /*while (true) {
        QThread::currentThread()->sleep(10);
    }*/
    //git test

    return a.exec();
}

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



tcpServer *tcpserver;


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    Handler *handler = new Handler(&a);

    tcpserver = new tcpServer(handler);
    tcpserver->startServer();


    return a.exec();
}

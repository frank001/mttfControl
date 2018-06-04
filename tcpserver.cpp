#include "tcpserver.h"
#include <QTcpServer>
#include <QTcpSocket>

#include <QObject>
#include <QHostAddress>
#include <QString>
#include <QDebug>
#include "config.h"

tcpServer::tcpServer(Config *parent) : QTcpServer(parent) {
    cfg = parent;
}


void tcpServer::startServer() {
    if(listen(QHostAddress::Any, 1337)) {
        qDebug() << "Server: started";
    } else {
        qDebug() << "Server: not started!";
    }
}

void tcpServer::incomingConnection(qintptr socketDescriptor) {
    // At the incoming connection, make a client
        // and set the socket
    tcpClient *client = new tcpClient(this);
    client->setConfig(cfg);
    client->setSocket(socketDescriptor);
}

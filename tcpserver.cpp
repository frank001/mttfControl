#include "tcpserver.h"
#include <QTcpServer>
#include <QTcpSocket>

#include <QObject>
#include <QHostAddress>
#include <QString>
#include <QDebug>
#include "config.h"
#include "uart.h"
tcpServer::tcpServer(Config *parent) : QTcpServer(parent) {
    cfg = parent;
    //uart(parent->serialPort).write(("l7\r"));

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
    tcpClient *client = new tcpClient(cfg);
    //client->setConfig(cfg);
    client->setSocket(socketDescriptor);
}

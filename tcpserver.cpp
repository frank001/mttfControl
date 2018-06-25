#include "tcpserver.h"
#include <QTcpServer>
#include <QTcpSocket>

#include <QObject>
#include <QHostAddress>
#include <QString>
#include <QDebug>
#include "main.h"
#include "handler.h"
#include "uart.h"

tcpServer::tcpServer(Handler *config, QObject *parent) :
    QTcpServer(parent),
    m_Config(config)
{
    connect(this, &tcpServer::message, m_Config, &Handler::message);
}


void tcpServer::startServer() {
    if(listen(QHostAddress::Any, 1337)) {
        message(NETWORK|WATCH, "TCP Server: started");
    } else {
        message(NETWORK|ERROR, "TCP Server: not started!");
    }
}

void tcpServer::incomingConnection(qintptr socketDescriptor) {
    // At the incoming connection, make a client
        // and set the socket

    message(NETWORK|DEBUG, "Client connecting.");
    tcpClient *client = new tcpClient(m_Config);
    client->setSocket(socketDescriptor);
}

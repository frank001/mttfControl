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

tcpServer::tcpServer(Handler *config, QObject *parent) : QTcpServer(parent), m_Config(config)
{
    //uart(parent->serialPort).write(("l7\r"));
}


void tcpServer::startServer() {
    if(listen(QHostAddress::Any, 1337)) {
        m_Config->message(NETWORK|INFO, "TCP Server: started");
    } else {
        m_Config->message(NETWORK|ERROR, "TCP Server: not started!");
    }
}

void tcpServer::incomingConnection(qintptr socketDescriptor) {
    // At the incoming connection, make a client
        // and set the socket

    m_Config->message(NETWORK|DEBUG, "Client connecting.");
    tcpClient *client = new tcpClient(m_Config);
    //client->setConfig(cfg);
    client->setSocket(socketDescriptor);
}

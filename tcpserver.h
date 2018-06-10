#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>

#include <QObject>
#include <QHostAddress>
#include <QString>
#include <QDebug>
#include "tcpclient.h"
#include "handler.h"

class tcpServer : public QTcpServer
{
    Q_OBJECT

private:
    Handler *m_Config;

public:
    explicit tcpServer(Handler *config = nullptr, QObject *parent = nullptr);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void message(unsigned int level, QString message);
public slots:


};

#endif // TCPSERVER_H

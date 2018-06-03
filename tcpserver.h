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
#include "config.h"

class tcpServer : public QTcpServer
{
    Q_OBJECT

private:
    QJsonObject *cfg;

public:
    explicit tcpServer(QObject *parent = nullptr);
    void startServer(QJsonObject *);

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:

public slots:


};

#endif // TCPSERVER_H

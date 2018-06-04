#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include "tcpTask.h"
#include "config.h"
//#include "tcpserver.h"


class tcpClient : public QObject
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = nullptr);
    void setConfig(Config *);
    void setSocket(qintptr Descriptor);

signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void taskResult(QString);


private:
    QTcpSocket *socket;
    Config *cfg;
    QObject *Parent;
};

#endif // TCPCLIENT_H

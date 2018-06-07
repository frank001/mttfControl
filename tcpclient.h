#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include "tcpTask.h"
#include "handler.h"



class tcpClient : public QObject
{
    Q_OBJECT
public:
    explicit tcpClient(Handler *, QObject *parent = nullptr);
    //void setConfig(Config *);
    void setSocket(qintptr Descriptor);

signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void taskResult(QString);


private:
    QTcpSocket *socket;
    Handler *m_Handler;
    //QObject *Parent;
};

#endif // TCPCLIENT_H

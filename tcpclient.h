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
    void setSocket(qintptr Descriptor);


public slots:
    void connected();
    void disconnected();
    void readyRead();
    void taskResult(QByteArray);
    void stateChanged(bool, QJsonDocument);
    void configChanged(bool, QJsonDocument);
    void cycleChanged(bool, QJsonDocument);

signals:
    void message(unsigned int level, QString message);


private:
    QTcpSocket *socket;
    Handler *m_Handler;

};

#endif // TCPCLIENT_H

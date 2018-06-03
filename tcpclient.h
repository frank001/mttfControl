#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include "tcpTask.h"
#include "config.h"


class tcpClient : public QObject
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = nullptr);
    void setConfig(QJsonObject *);
    void setSocket(qintptr Descriptor);

signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void taskResult(QString);

private:
    QTcpSocket *socket;
    QJsonObject *cfg;

};

#endif // TCPCLIENT_H

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>

class tcpClient : public QObject
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TCPCLIENT_H
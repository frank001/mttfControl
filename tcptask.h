#ifndef TCPTASK_H
#define TCPTASK_H

#include <QObject>
#include <QRunnable>
#include "config.h"

class tcpTask : public QObject, public QRunnable {
    Q_OBJECT
public:
    QByteArray Data;
    tcpTask(QByteArray data);
    void setConfig(QJsonObject *);

public slots:
    QByteArray getData();
    QJsonObject *getConfig();
signals:
    void Result(QString);
protected:
    void run();
    QJsonObject *cfg;

};

#endif // TCPTASK_H

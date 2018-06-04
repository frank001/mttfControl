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
    void setConfig(Config *);

public slots:
    QByteArray getData();
    Config *getConfig();
    void setState(QString msg);
signals:
    void Result(QString);
    void getState();

protected:
    void run();
    Config *cfg;

};


#endif // TCPTASK_H

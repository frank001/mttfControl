#ifndef TCPTASK_H
#define TCPTASK_H

#include <QObject>
#include <QRunnable>
#include "config.h"
#include "commands.h"

class tcpTask : public QObject, public QRunnable {
    Q_OBJECT
public:
    QByteArray Data;
    tcpTask(Config *, QByteArray data);
    void setConfig(Config *);
    Commands *command;
private:



public slots:
    QByteArray getData();
    Config *getConfig();

    void setState(QString msg);

signals:
    void Result(QString);
    void getState();
    void message(unsigned char level, QString msg);

protected:
    void run();
    Config *m_Config;

};


#endif // TCPTASK_H

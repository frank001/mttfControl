#ifndef TCPTASK_H
#define TCPTASK_H

#include <QObject>
#include <QRunnable>
#include "handler.h"
#include "commands.h"

class tcpTask : public QObject, public QRunnable {
    Q_OBJECT
public:
    QByteArray Data;
    tcpTask(Handler *, QByteArray data);
    void setConfig(Handler *);
    Commands *command;
private:



public slots:
    QByteArray getData();
    Handler *getConfig();

    void setState(QString msg);

signals:
    void Result(QByteArray);
    void message(qint16 level, QString msg);

protected:
    void run();
    Handler *m_Handler;

};


#endif // TCPTASK_H

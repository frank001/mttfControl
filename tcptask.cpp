#include "tcptask.h"
#include <QDebug>
#include <QString>
#include <QTextCodec>
//#include "handler.h"
#include <QJsonDocument>
#include "main.h"
#include "commands.h"

tcpTask::tcpTask(Handler *handler, QByteArray data) :
    m_Handler(handler),
    Data(data)
{
    m_Handler = getConfig();

    connect(this, &tcpTask::message, m_Handler, &Handler::message);
    message(NETWORK|WATCH, "tcpTask initialized.");

}

void tcpTask::run() {
    //Handler *handler = getConfig();

    command = m_Handler->m_Command;
    message(NETWORK|DEBUG, "Task started.");       //TODO: create SLOTS/SIGNALS between tasks and Handler. Done.
    QByteArray data = getData();
    QString msg = QString::fromUtf8(data.data());
    message(NETWORK|WATCH, "Request received: " + msg);

    QByteArray ba = command->Handle(msg);


    emit Result(ba);
}

QByteArray tcpTask::getData(){
    return Data;
}

Handler *tcpTask::getConfig(){
    return m_Handler;
}

void tcpTask::setState(QString msg) {
    int i=0;
    i++;
}



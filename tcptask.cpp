#include "tcptask.h"
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include "handler.h"
#include <QJsonDocument>
#include "main.h"
#include "commands.h"

tcpTask::tcpTask(Handler *handler, QByteArray data) :
    m_Handler(handler),
    Data(data)
{
    //command = new Commands(m_Config);
    connect(this, &tcpTask::message, m_Handler, &Handler::message);
    message(NETWORK|WATCH, "tcpTask initialized.");
}

void tcpTask::run() {
    Handler *handler = getConfig();
    //command = new Commands(m_Handler);
    command = m_Handler->m_Command;
    message(NETWORK|DEBUG, "Task started.");       //TODO: create SLOTS/SIGNALS between tasks and Handler. Done.
    QByteArray data = getData();
    QString msg = QString::fromUtf8(data.data());
    message(NETWORK|WATCH, "Request received: " + msg);

    //QJsonDocument jdConfig(*command->Handle(msg));
    //QByteArray ba = jdConfig.toJson();

    QByteArray ba = command->Handle(msg);
    //message(0, "Task done");

    emit Result(ba);
}

QByteArray tcpTask::getData(){
    return Data;
}

Handler *tcpTask::getConfig(){
    return m_Handler;
}
/*void tcpTask::getState() {
    emit(getState());
}*/

void tcpTask::setState(QString msg) {
    int i=0;
    i++;
}



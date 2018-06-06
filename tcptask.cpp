#include "tcptask.h"
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include "config.h"
#include <QJsonDocument>
#include "commands.h"

tcpTask::tcpTask(Config *config, QByteArray data) :
    m_Config(config),
    Data(data)
{
    command = new Commands(m_Config);
    connect(this, &tcpTask::message, m_Config, &Config::message);
    m_Config->message(0, "tcpTask initialized.");
}

void tcpTask::run() {
    // time consumer
    Config *c = getConfig();
    message(0, "Task started.");       //TODO: create SLOTS/SIGNALS between tasks and Handler. Done.
    QByteArray data = getData();
    QString msg = QString::fromUtf8(data.data());


    command->Handle(msg);



    //emit getState();

    int iNumber = 0;
    for(int i = 0; i < 100; i++) {
        iNumber += 1;
    }

    message(0, "Task done");
    QJsonDocument jdConfig(*c->joConfig);
    QByteArray ba = jdConfig.toJson();


    emit Result(ba);
}

QByteArray tcpTask::getData(){
    return Data;
}

Config *tcpTask::getConfig(){
    return m_Config;
}
/*void tcpTask::getState() {
    emit(getState());
}*/

void tcpTask::setState(QString msg) {
    int i=0;
    i++;
}


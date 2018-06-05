#include "tcptask.h"
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include "config.h"
#include <QJsonDocument>
#include "commands.h"

tcpTask::tcpTask(Config *parent, QByteArray data) {
    cfg = parent;
    Data = data;
    qInfo() << "tcpTask();";



}

void tcpTask::run() {
    // time consumer

    qInfo() << "Task started";
    QByteArray data = getData();
    //QString msg = QTextCodec::codecForMib(106)->toUnicode(data);
    QString msg = QString::fromUtf8(data.data());
    qInfo() << msg;

    command.Handle(msg);

    Config *c = getConfig();

    //emit getState();

    int iNumber = 0;
    for(int i = 0; i < 100; i++) {
        iNumber += 1;
    }

    qDebug() << "Task done";
    QJsonDocument jdConfig(*c->joConfig);
    QByteArray ba = jdConfig.toJson();


    emit Result(ba);
}

QByteArray tcpTask::getData(){
    return Data;
}

Config *tcpTask::getConfig(){
    return cfg;
}
/*void tcpTask::getState() {
    emit(getState());
}*/

void tcpTask::setState(QString msg) {
    int i=0;
    i++;
}

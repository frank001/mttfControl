#include "tcptask.h"
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include "config.h"
#include <QJsonDocument>

tcpTask::tcpTask(QByteArray data) {
    Data = data;
    qInfo() << "tcpTask();";


}
void tcpTask::setConfig(QJsonObject *config) {
    cfg = config;
}
void tcpTask::run() {
    // time consumer

    qInfo() << "Task started";
    QByteArray data = getData();
    //QString msg = QTextCodec::codecForMib(106)->toUnicode(data);
    QString msg = QString::fromUtf8(data.data());
    qInfo() << msg;

    QJsonObject *c = getConfig();

    int iNumber = 0;
    for(int i = 0; i < 100; i++) {
        iNumber += 1;
    }

    qDebug() << "Task done";
    //QJsonDocument jdConfig(c);
    //QByteArray ba = jdConfig.toJson();


    emit Result(msg);
}

QByteArray tcpTask::getData(){
    return Data;
}
QJsonObject *tcpTask::getConfig(){
    return cfg;
}

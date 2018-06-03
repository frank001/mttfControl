#include "tcpclient.h"

tcpClient::tcpClient(QObject *parent) : QObject(parent) {
    QThreadPool::globalInstance()->setMaxThreadCount(5);

}
void tcpClient::setConfig(QJsonObject *config) {
    cfg=config;
}
void tcpClient::setSocket(qintptr descriptor){
    socket = new QTcpSocket(this);
    qInfo() << "Socket created";
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(descriptor);

    qDebug() << "Client connected at " << descriptor;
}

void tcpClient::connected() {   //async
    qDebug() << "Client connected event";
}

void tcpClient::disconnected() { //async
    qDebug() << "Client disconnected";
}

void tcpClient::readyRead() {
    qDebug() << "MyClient::readyRead()";
    //qDebug() << socket->readAll();              // Read commands from client at this point

    // Time consumer
    QByteArray ba = socket->readAll();
    tcpTask *tcptask = new tcpTask(ba);
    tcptask->setConfig(cfg);
    tcptask->setAutoDelete(true);

    // using queued connection
    connect(tcptask, SIGNAL(Result(QString)), this, SLOT(taskResult(QString)), Qt::QueuedConnection);

    qDebug() << "Starting a new task using a thread from the QThreadPool";

    // QThreadPool::globalInstance() returns global QThreadPool instance
    QThreadPool::globalInstance()->start(tcptask);

}
void tcpClient::taskResult(QString result) {
    QByteArray Buffer;
    Buffer.append("\r\nTask result = ");
    Buffer.append(result);

    socket->write(Buffer);                      //Read results from the task.
}

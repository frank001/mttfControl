#include "tcpclient.h"

tcpClient::tcpClient(Config *config, QObject *parent) : QObject(parent), m_Config(config) {
    QThreadPool::globalInstance()->setMaxThreadCount(5);
}

void tcpClient::setSocket(qintptr descriptor){
    socket = new QTcpSocket(this);
    m_Config->message(0, "Socket created.");

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(descriptor);
    m_Config->message(0, "Client connected at port " + QString::number(descriptor));
}

void tcpClient::connected() {   //async
    m_Config->message(0, "Client connected event");
}

void tcpClient::disconnected() { //async
    m_Config->message(0, "Client disconnected");
}

void tcpClient::readyRead() {
    m_Config->message(0, "Client data received.");

    QByteArray ba = socket->readAll();
    tcpTask *tcptask = new tcpTask(m_Config, ba);

    tcptask->setAutoDelete(true);

    connect(tcptask, SIGNAL(Result(QString)), this, SLOT(taskResult(QString)), Qt::QueuedConnection);

    m_Config->message(0, "Spawning thread from QThreadPool");
    QThreadPool::globalInstance()->start(tcptask);

}
void tcpClient::taskResult(QString result) {
    QByteArray Buffer;
    Buffer.append(result);
    m_Config->message(0, "Reporting result to client:" + result);
    socket->write(Buffer);
}


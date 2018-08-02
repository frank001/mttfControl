#include "main.h"
#include "tcpclient.h"

tcpClient::tcpClient(Handler *config, QObject *parent) : QObject(parent), m_Handler(config) {
    QThreadPool::globalInstance()->setMaxThreadCount(5);
}

void tcpClient::setSocket(qintptr descriptor){
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &tcpClient::connected);
    connect(socket, &QTcpSocket::disconnected, this, &tcpClient::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &tcpClient::readyRead);

    connect(m_Handler, &Handler::StateChanged, this, &tcpClient::stateChanged);
    connect(m_Handler, &Handler::ConfigChanged, this, &tcpClient::configChanged);
    connect(m_Handler, &Handler::CycleChanged, this, &tcpClient::cycleChanged);
    connect(this, &tcpClient::message, m_Handler, &Handler::message);



    socket->setSocketDescriptor(descriptor);
    message(NETWORK|WATCH, "Client connected at port " + QString::number(descriptor));
}

void tcpClient::connected() {   //async
                                                                    //This function never gets called? it did once...
    message(NETWORK|DEBUG, "Client connected event");
}

void tcpClient::disconnected() { //async
    message(NETWORK|WATCH, "Client disconnected");
}

void tcpClient::readyRead() {
    message(NETWORK|DEBUG, "Client data received.");

    QByteArray ba = socket->readAll();
    tcpTask *tcptask = new tcpTask(m_Handler, ba);

    tcptask->setAutoDelete(true);

    connect(tcptask, &tcpTask::Result, this, &tcpClient::taskResult, Qt::QueuedConnection);
    message(NETWORK|DEBUG, "Spawning thread from QThreadPool");
    QThreadPool::globalInstance()->start(tcptask);

}
void tcpClient::taskResult(QByteArray result) {
    QString msg = result.replace("\n","");
    message(NETWORK|WATCH, "Result: " + msg );
    socket->write(result);
}
void tcpClient::stateChanged(bool log, QJsonDocument jd) {
    socket->write(jd.toBinaryData());
}
void tcpClient::configChanged(bool log, QJsonDocument jd) {
    QString test = jd.toJson();
    socket->write(jd.toBinaryData());
}
void tcpClient::cycleChanged(bool log, QJsonDocument jd) {
    socket->write(jd.toBinaryData());
}


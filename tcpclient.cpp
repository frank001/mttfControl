#include "main.h"
#include "tcpclient.h"

tcpClient::tcpClient(Handler *config, QObject *parent) : QObject(parent), m_Handler(config) {
    QThreadPool::globalInstance()->setMaxThreadCount(5);
}

void tcpClient::setSocket(qintptr descriptor){
    socket = new QTcpSocket(this);
    //m_Config->message(0, "Socket created.");

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(descriptor);
    m_Handler->message(NETWORK|INFO, "Client connected at port " + QString::number(descriptor));
}

void tcpClient::connected() {   //async
    m_Handler->message(NETWORK|DEBUG, "Client connected event");
}

void tcpClient::disconnected() { //async
    m_Handler->message(NETWORK|INFO, "Client disconnected");
}

void tcpClient::readyRead() {
    m_Handler->message(NETWORK|DEBUG, "Client data received.");

    QByteArray ba = socket->readAll();
    tcpTask *tcptask = new tcpTask(m_Handler, ba);

    tcptask->setAutoDelete(true);

    connect(tcptask, SIGNAL(Result(QString)), this, SLOT(taskResult(QString)), Qt::QueuedConnection);

    m_Handler->message(THREAD|DEBUG, "Spawning thread from QThreadPool");
    QThreadPool::globalInstance()->start(tcptask);

}
void tcpClient::taskResult(QString result) {
    QByteArray Buffer;
    Buffer.append(result);
    m_Handler->message(NETWORK|INFO, "Result: " + result);
    socket->write(Buffer);
}


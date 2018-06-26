#include "uart.h"
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QDebug>
#include <QList>
#include <QTextCodec>
#include <QJsonObject>
#include "handler.h"
#include "main.h"

uart::uart(QString portname, int baudrate, QObject *parent) :
    QObject(parent),
    m_Baudrate(baudrate),
    m_Portname(portname)
{


    m_timer = new QTimer();


    connect(&m_serialPort, &QSerialPort::bytesWritten, this, &uart::handleBytesWritten);
    connect(&m_serialPort, &QSerialPort::errorOccurred, this, &uart::handleError);

    //connect(&m_serialPort, &QSerialPort::readyRead, this, &uart::handleReadyRead);    //Qt bug, error about timers and threads

    connect(m_timer, &QTimer::timeout, this, &uart::handleTimeout);





}
void uart::initialize(){
    /*  SerialPortInfo */
    message(UART|INFO, "Iterating serial ports");
    QSerialPortInfo *spi = new QSerialPortInfo();

    ports = spi->availablePorts();
    joPorts.insert("count", ports.count());
    int i;
    for (i=0;i< ports.count();i++) {
        QSerialPortInfo port = ports[i];
        joPorts.insert(QString::number(i), port.portName());
        message(UART|WATCH, "Found port: " + port.portName());
        //qInfo()<<port.portName();
    }

    //*/

    message(UART|INFO, "initializing serial port " + m_Portname + ", baudrate: " + QString::number(m_Baudrate));
    m_serialPort.setPortName(m_Portname);
    m_serialPort.setBaudRate(m_Baudrate);
    m_serialPort.open(QIODevice::ReadWrite);

    m_timer->start(100);
}

void uart::handleBytesWritten(qint64 bytes) {
    m_bytesWritten += bytes;
    if (m_bytesWritten == m_writeData.size()) {
        m_bytesWritten = 0;
        message(UART|DEBUG, "Data successfully sent to port: " + QString::number(m_writeData.size()) + " bytes.");
        m_serialPort.waitForReadyRead(50);
    }
}

void uart::handleTimeout() {
    handleReadyRead();              //Qt bug workaround (polling)
    //message(UART|ERROR, "Operation timed out for port");
}

void uart::handleError(QSerialPort::SerialPortError serialPortError) {
    if (serialPortError == QSerialPort::WriteError) {
        message(UART|ERROR, "An I/O error occured while writing");
    }
}

void uart::write(const QByteArray &writeData) {
    m_writeData = writeData + "\r";
    message(UART|DEBUG, "Writing data to uart: "+writeData);
    const qint64 bytesWritten = m_serialPort.write(m_writeData);
    m_serialPort.waitForBytesWritten(-1);

    if (bytesWritten == -1) {
        message(UART|ERROR, "Failed to write data to port.");
    } else if (bytesWritten != m_writeData.size()) {
        message(UART|ERROR, "Failed to write all the data to port");
    }

}

void uart::handleReadyRead() {
    if (!m_serialPort.isOpen()) {
        message(UART|ERROR, "Port is closed!");
        return;
    }
    QByteArray data = m_serialPort.readAll();
    while (m_serialPort.waitForReadyRead(50)) {
        data.append(m_serialPort.readAll());
    }
    if (data.length()>0) {
        QString sdata = QTextCodec::codecForMib(106)->toUnicode(data);
        message(UART|WATCH, "Serial port data received: " +  sdata);
        if (data=="reset@disabled.@") { //door open
            doorChange(1);
            message(UART|WARN, "Door open.");
        } else if (data=="active.@") { //door closed
            doorChange(0);
            message(UART|WARN, "Door closed.");
        } else if (data!="c@") {   //unknown data received
            message(UART|ERROR, "Unkown data received: " + sdata);
        }

    }
}

void uart::closePort(){
    m_serialPort.close();

}

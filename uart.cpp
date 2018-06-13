#include "uart.h"
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QDebug>
#include <QList>
#include <QTextCodec>
#include "handler.h"
#include "main.h"

uart::uart(QSerialPort *port, QObject *parent) :
    QObject(parent)
{
    //connect(this, &uart::message, (Handler*)parent, &Handler::message);


    m_timer = new QTimer();
    //m_timer->setSingleShot(true);


    connect(&m_serialPort, &QSerialPort::bytesWritten, this, &uart::handleBytesWritten);
    connect(&m_serialPort, &QSerialPort::errorOccurred, this, &uart::handleError);
    //connect(&m_serialPort, &QSerialPort::readyRead, this, &uart::handleReadyRead);

    connect(m_timer, &QTimer::timeout, this, &uart::handleTimeout);

    message(UART|INFO, "initializing serial port");
    m_serialPort.setPortName("COM8");
    m_serialPort.setBaudRate(QSerialPort::Baud115200);
    m_serialPort.open(QIODevice::ReadWrite);

    m_timer->start(100);

    /*  SerialPortInfo


    QSerialPortInfo *spi = new QSerialPortInfo();

    ports = spi->availablePorts();

    for (char i=0;i< ports.count();i++) {
        QSerialPortInfo port = ports[i];
        qInfo()<<port.portName();
    }

    QSerialPort(serialPort);

    serialPort.setPortName("COM3");
    serialPort.setBaudRate((qint32)9600);
    serialPort.setDataBits((QSerialPort::DataBits)8);
    serialPort.setStopBits((QSerialPort::StopBits)1);
    serialPort.setParity(QSerialPort::NoParity);
    qInfo() << "Opening serial port";
    */

}

void uart::handleBytesWritten(qint64 bytes) {
    m_bytesWritten += bytes;
    if (m_bytesWritten == m_writeData.size()) {
        m_bytesWritten = 0;
        message(UART|DEBUG, "Data successfully sent to port: " + QString::number(m_writeData.size()) + " bytes.");
        m_serialPort.waitForReadyRead(50);
        //m_timer->start(50);
        //m_timer->stop();
    }
}

void uart::handleTimeout() {
    handleReadyRead();
    //message(UART|ERROR, "Operation timed out for port");
}

void uart::handleError(QSerialPort::SerialPortError serialPortError) {
    if (serialPortError == QSerialPort::WriteError) {
        message(UART|ERROR, "An I/O error occured while writing");
    }
}

void uart::write(const QByteArray &writeData) {
    m_writeData = writeData + "\r";
    message(UART|WATCH, "Writing data to uart: "+writeData);
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
    //message(UART|DEBUG, "handleReadyRead invoked");
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

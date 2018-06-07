#include "uart.h"
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QDebug>
#include <QList>
#include "handler.h"
#include "main.h"

uart::uart(QSerialPort *port, QObject *parent) :
    QObject(parent),
    m_serialPort(port)
{
    message(UART|DEBUG, "initializing serial port");

    //m_timer = new QTimer();
    //m_timer->setSingleShot(true);

    connect(m_serialPort, &QSerialPort::bytesWritten, this, &uart::handleBytesWritten);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &uart::handleError);
    //connect(m_timer, &QTimer::timeout, this, &uart::handleTimeout);


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
        //m_timer->stop();
    }
}

void uart::handleTimeout() {

    message(UART|ERROR, "Operation timed out for port");
}

void uart::handleError(QSerialPort::SerialPortError serialPortError) {
    if (serialPortError == QSerialPort::WriteError) {
        message(UART|ERROR, "An I/O error occured while writing");
    }
}

void uart::write(const QByteArray &writeData) {
    m_writeData = writeData;
    message(UART|DEBUG, "Writing data to uart: "+writeData);
    const qint64 bytesWritten = m_serialPort->write(writeData);
    m_serialPort->waitForBytesWritten(-1);

    if (bytesWritten == -1) {
        message(UART|ERROR, "Failed to write data to port.");
    } else if (bytesWritten != m_writeData.size()) {
        message(UART|ERROR, "Failed to write all the data to port");
    }
    //m_timer->start(1000);
}

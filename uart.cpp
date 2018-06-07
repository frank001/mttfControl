#include "uart.h"
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QDebug>
#include <QList>
#include "config.h"

uart::uart(QSerialPort *port, QObject *parent) :
    QObject(parent),
    m_serialPort(port),
    m_standardOutput(stdout) {

    m_timer.setSingleShot(true);

    connect(m_serialPort, &QSerialPort::bytesWritten, this, &uart::handleBytesWritten);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &uart::handleError);
    connect(&m_timer, &QTimer::timeout, this, &uart::handleTimeout);
    /*
    //cfg = config;



    //QSerialPort mSerial = new QSerialPort();

    QSerialPortInfo *spi = new QSerialPortInfo();

    ports = spi->availablePorts();

    for (char i=0;i< ports.count();i++) {
        QSerialPortInfo port = ports[i];
        qInfo()<<port.portName();
    }

    QSerialPort(serialPort);


    qInfo() << "initializing serial port";
    serialPort.setPortName("COM3");
    serialPort.setBaudRate((qint32)9600);
    serialPort.setDataBits((QSerialPort::DataBits)8);
    serialPort.setStopBits((QSerialPort::StopBits)1);
    serialPort.setParity(QSerialPort::NoParity);
    qInfo() << "Opening serial port";
    */

}

void uart::handleBytesWritten(qint64 bytes)
{
    m_bytesWritten += bytes;
    if (m_bytesWritten == m_writeData.size()) {
        m_bytesWritten = 0;
        qInfo() << "Data successfully sent to port";
                   //QObject::tr("Data successfully sent to port %1")
                   //         .arg(m_serialPort->portName()) << endl;
        //QCoreApplication::quit();
        //QThread::currentThread()->exit(0);
        //m_timer.stop();
    }
}

void uart::handleTimeout()
{
    qInfo() << "Operation timed out for port";

               //QObject::tr("Operation timed out for port %1, error: %2")
               //         .arg(m_serialPort->portName())
               //         .arg(m_serialPort->errorString())
               //      << endl;
    //QCoreApplication::exit(1);
    //QThread::currentThread()->exit(1);
    //QCoreApplication::quit();
}

void uart::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::WriteError) {
        qInfo() << "An I/O error occured while writing";
                   //QObject::tr("An I/O error occurred while writing"
                   //                     " the data to port %1, error: %2")
                   //         .arg(m_serialPort->portName())
                   //         .arg(m_serialPort->errorString())
                   //      << endl;
        //QCoreApplication::exit(1);
        //QThread::currentThread()->exit(1);
    }
}

void uart::write(const QByteArray &writeData)
{
    m_writeData = writeData;

    const qint64 bytesWritten = m_serialPort->write(writeData);
    m_serialPort->waitForBytesWritten(-1);

    if (bytesWritten == -1) {
        qInfo() << "Failed to write data to port.";
                   //QObject::tr("Failed to write the data to port %1, error: %2")
                   //         .arg(m_serialPort->portName())
                   //         .arg(m_serialPort->errorString())
                   //      << endl;
       //QCoreApplication::exit();
    } else if (bytesWritten != m_writeData.size()) {
        qInfo() << "Failed to write all the data to port";
                   //QObject::tr("Failed to write all the data to port %1, error: %2")
                   //         .arg(m_serialPort->portName())
                   //         .arg(m_serialPort->errorString())
                   //      << endl;
        //QThread::currentThread()->exit();
        //QCoreApplication::exit(1);
    }

    //m_timer.start(5000);
}

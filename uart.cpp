#include "uart.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QDebug>
#include <QList>
#include "config.h"

uart::uart(Config *config) {
    qInfo() << "initializing serial port";

    cfg = config;
    //QSerialPort mSerial = new QSerialPort();
    QSerialPortInfo *spi = new QSerialPortInfo();

    ports = spi->availablePorts();

    for (char i=0;i< ports.count();i++) {
        QSerialPortInfo port = ports[i];
        qInfo()<<port.portName();
    }
    int i=0;
    i++;

}

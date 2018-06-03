#ifndef UART_H
#define UART_H
#include <QList>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include "config.h"

class uart
{
private:
    Config *cfg;
    QList<QSerialPortInfo> ports;

public:
    uart(Config *);
};

#endif // UART_H

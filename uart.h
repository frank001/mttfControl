#ifndef UART_H
#define UART_H
#include <QList>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QThread>
#include <QTextStream>
#include <QTimer>

#include "config.h"

class uart : public QObject
{
    Q_OBJECT



private slots:
    void handleBytesWritten(qint64 bytes);
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);



public:
    explicit uart(QSerialPort *port, QObject *parent = nullptr);
    void write(const QByteArray &writeData);


private:
    Config *cfg;
    QList<QSerialPortInfo> ports;
    QSerialPort *m_serialPort = nullptr;

    QByteArray m_writeData;
    QTextStream m_standardOutput;

    qint64 m_bytesWritten = 0;
    QTimer m_timer;

};

#endif // UART_H

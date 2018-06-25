#ifndef UART_H
#define UART_H
#include <QList>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QThread>
#include <QTextStream>
#include <QTimer>
#include <QJsonObject>


class uart : public QObject
{
    Q_OBJECT



private slots:
    void handleBytesWritten(qint64 bytes);
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);
    void handleReadyRead();


public:
    explicit uart(QString portname, int baudrate, QObject *parent = nullptr);
    void write(const QByteArray &writeData);

    void initialize();

    QJsonObject joPorts;


private:

    QList<QSerialPortInfo> ports;
    QSerialPort m_serialPort;
    QString m_Portname;
    int m_Baudrate;


    QByteArray m_writeData;
    QTextStream m_standardOutput;

    qint64 m_bytesWritten = 0;
    QTimer *m_timer;
public slots:
    void closePort();

signals:
    void message(qint16 level, QString msg);
    void doorChange(int status);
};

#endif // UART_H

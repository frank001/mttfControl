#ifndef CONFIG_H
#define CONFIG_H
#include <QJsonObject>
#include <QtSerialPort/QSerialPort>
#include "logger.h"
#include "uart.h"

class Handler : public QObject {
     Q_OBJECT

private:
    int mLogLevel=0;
    int mVibrate=0;

    int mBaudRate=115200;
    int mDataBits=8;
    int mStopBits=1;
    char mParity ='n';
    QString mPortName="COM1";

    Logger *m_dbObject;


    QSerialPort m_SerialPort;



public:
    //explicit Config(QObject *parent = nullptr);
    explicit Handler(QSerialPort *port, Logger *database, QObject *parent = nullptr);
    uart *m_uart;
    //Config(logger *, dbObject *);
    int logLevel();
    int logLevel(int level);
    void setConfig(QJsonArray);
    void setVibrate(int);
    void writeConfig();
    void writeState();
    QJsonObject *joConfig;
    QJsonObject *joState;
    QSerialPort *serialPort;
    QJsonObject uartResult;



public slots:

    QJsonObject *getConfig();
    void getState();
    //void read(const QJsonObject &json);
    //void write(const QJsonObject &json);
    void uartWrite(QByteArray data);
    void message(unsigned char, QString);
    void setState(QString, QJsonValue value);

};

#endif // CONFIG_H

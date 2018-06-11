#ifndef CONFIG_H
#define CONFIG_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QtSerialPort/QSerialPort>
#include "logger.h"
#include "uart.h"

class Handler : public QObject
{
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


    QSerialPort *m_SerialPort;
    Logger *m_Logger;
    QThread thread;

public:
    explicit Handler(QObject *parent = nullptr);
    uart *m_uart;
    int logLevel();
    int logLevel(int level);
    void setConfig(QJsonArray);
    void setVibrate(int);
    void writeConfig();
    void writeState();

    QJsonDocument jdConfig;
    QJsonDocument jdState;

    QJsonObject *joConfig;
    QJsonObject *joState;

    QJsonObject uartResult;




public slots:

    QJsonObject *getConfig();
    void getState();
    //void read(const QJsonObject &json);
    //void write(const QJsonObject &json);
    void uartWrite(QByteArray data);
    void message(unsigned int, QString);
    void setState(QString, QJsonValue value);



signals:
    QJsonArray logExecute(QString);
    void logMessage(unsigned int level, QString text);

};

#endif // CONFIG_H

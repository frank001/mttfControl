#ifndef CONFIG_H
#define CONFIG_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QtSerialPort/QSerialPort>
#include "logger.h"
#include "uart.h"
#include "cycle.h"
#include "commands.h"

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

    Cycle *m_Cycle;

    QThread thread;

    QJsonDocument jdUpdate(QJsonDocument, QString, QString, QJsonValue);

public:
    explicit Handler(QObject *parent = nullptr);
    uart *m_uart;
    Commands *m_Command;

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

    //QJsonObject *getConfig();
    void getState();
    void getConfig();
    //void read(const QJsonObject &json);
    //void write(const QJsonObject &json);
    void uartWrite(QByteArray data);
    void message(unsigned int, QString);
    void setState(QString, QJsonValue value);
    void setConfig(QString, QJsonValue value);
    void doorChange(int status);
    void lightChange(int level); //0 - off, 1 -  0.1mlux, 2 - 5mlux, 3 - 50lux
    void setHandlerCycle(int);

signals:
    QJsonArray logExecute(QString);
    void logMessage(unsigned int level, QString text);
    void StateChanged(QJsonDocument);
    void ConfigChanged(QJsonDocument);
    //void startCycle(int);
};

#endif // CONFIG_H

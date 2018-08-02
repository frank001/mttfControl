#ifndef CONFIG_H
#define CONFIG_H
#include <QCoreApplication>
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

    int mBaudRate=9600;
    int mDataBits=8;
    int mStopBits=1;
    char mParity = 'n';
    QString mPortName="COM1";

    Logger *m_dbObject;


    QSerialPort *m_SerialPort;
    Logger *m_Logger;

    Cycle *m_Cycle;

    QThread thread;

    QString sConfig;
    QString sState;
    QString sCycle;

    bool jdUpdateState(QString, QString, QJsonValue);
    bool jdUpdateConfig(QString name, QString key, QJsonValue value);
    void setHandlerInitialState(QJsonDocument);
    void setHandlerInitialConfig(QJsonDocument);
    void setHandlerInitialCycle(QJsonDocument);

    int m_CycleCount = 0;


public:
    explicit Handler(QCoreApplication *parent = nullptr);
    uart *m_uart;
    Commands *m_Command;

    QJsonDocument jdConfig;
    QJsonDocument jdState;
    QJsonDocument jdPorts;
    QJsonDocument jdCycle;

    QJsonObject joConfig;
    QJsonObject joState;
    QJsonObject joCycle;

    int logLevel();
    int logLevel(int level);

public slots:

    void message(unsigned int, QString);
    void setHandlerState(QString, QJsonValue value);
    void setHandlerConfig(QString, QJsonValue value);
    void getHandlerState();
    void getHandlerConfig();
    void getHandlerCycle();

    void setHandlerPosition(QString, QString);

    void newHandlerCycle(QString, QJsonValue);

    void setHandlerDoor(int status);
    void setHandlerCycle(int);
    void incrementHandlerCycle();

    void resetHandlerUart(QString);

signals:
    QJsonDocument logExecute(QString);
    void logMessage(unsigned int level, QString text);
    void StateChanged(bool, QJsonDocument);
    void ConfigChanged(bool, QJsonDocument);
    void CycleChanged(bool, QJsonDocument);
    void closePort();
};

#endif // CONFIG_H

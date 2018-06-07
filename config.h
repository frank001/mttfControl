#ifndef CONFIG_H
#define CONFIG_H
#include <QJsonObject>
#include <QtSerialPort/QSerialPort>
#include "dbobject.h"
#include "uart.h"

class Config : public QObject {
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

    QJsonObject *joState;
    QSerialPort m_SerialPort;


public:
    //explicit Config(QObject *parent = nullptr);
    explicit Config(QSerialPort *port, Logger *database, QObject *parent = nullptr);

    //Config(logger *, dbObject *);
    int logLevel();
    int logLevel(int level);
    void setConfig(QJsonArray);
    void setVibrate(int);
    void writeConfig();
    void writeState();
    QJsonObject *joConfig;
    QSerialPort *serialPort;

public slots:

    QJsonObject *getConfig();
    void getState();
    //void read(const QJsonObject &json);
    //void write(const QJsonObject &json);
    void uartWrite(QByteArray data);
    void message(unsigned char, QString);
signals:
    void setState(QString msg);
};

#endif // CONFIG_H

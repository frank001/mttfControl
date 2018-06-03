#ifndef CONFIG_H
#define CONFIG_H
#include <QJsonObject>
#include "logger.h"
#include "dbobject.h"
class Config
{
private:
    int mLogLevel=0;
    int mVibrate=0;

    int mBaudRate=9600;
    int mDataBits=8;
    int mStopBits=1;
    char mParity ='n';
    QString mPortName="COM1";

    logger *log;
    dbObject *db;

    QJsonObject state;

public:
    Config();
    Config(logger *, dbObject *);
    int logLevel();
    int logLevel(int level);
    void setConfig(QJsonArray);
    void setVibrate(int);
    void writeConfig();
    void writeState();
    QJsonObject cfg;

public slots:

    QJsonObject *getConfig();

    //void read(const QJsonObject &json);
    //void write(const QJsonObject &json);
};

#endif // CONFIG_H

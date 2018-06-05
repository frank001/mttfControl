#ifndef CONFIG_H
#define CONFIG_H
#include <QJsonObject>
#include "logger.h"
#include "dbobject.h"


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

    logger *log;
    dbObject *db;

    QJsonObject *joState;

public:
    explicit Config(QObject *parent = nullptr);

    Config(logger *, dbObject *);
    int logLevel();
    int logLevel(int level);
    void setConfig(QJsonArray);
    void setVibrate(int);
    void writeConfig();
    void writeState();
    QJsonObject *joConfig;


public slots:

    QJsonObject *getConfig();
    void getState();
    //void read(const QJsonObject &json);
    //void write(const QJsonObject &json);
signals:
    void setState(QString msg);
};

#endif // CONFIG_H

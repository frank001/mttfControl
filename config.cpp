#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

#include "config.h"
#include "dbobject.h"

//Config::Config() {}
Config::Config(QSerialPort *port, Logger *database, QObject *parent) :
    QObject(parent),
    m_dbObject(database),
    m_SerialPort(port)
{
    //log = l;
    //db = database;

}

void Config::setConfig(QJsonArray ja) {
    joConfig = new QJsonObject();
    joConfig->insert("loglevel", mLogLevel);
    joConfig->insert("baudrate", mBaudRate);
    joConfig->insert("databits", mDataBits);
    joConfig->insert("stopbits", mStopBits);
    joConfig->insert("parity", mParity);
    joConfig->insert("port", mPortName);
    joState = new QJsonObject();
    joState->insert("vibrate", mVibrate);

    if (ja.count()==0) {    //create default state record
       //QJsonDocument jdConfig(cfg);



        m_dbObject->message(0,"Creating default state record.");
        m_dbObject->execute("insert into currentState (config, state) values('','');");
        writeConfig();
        writeState();


    } else { //read current state

        QJsonObject currentState = ja[0].toObject();
        QString config = currentState["config"].toString();
        QJsonDocument curConfig = QJsonDocument::fromJson(config.toUtf8());

        //TODO: create function to change variables syncing the objects joConfig and joState
        mLogLevel = curConfig["loglevel"].toInt(); joConfig->value("loglevel") = mLogLevel;
        mPortName = curConfig["port"].toString(); joConfig->value("port") = mPortName;
        mBaudRate = curConfig["baudrate"].toInt(); joConfig->value("baudrate") = mBaudRate;
        mDataBits = curConfig["databits"].toInt(); joConfig->value("databits") = mDataBits;
        mStopBits = curConfig["stopbits"].toInt(); joConfig->value("stopbits") = mStopBits;
        mParity = curConfig["parity"].toInt(); joConfig->value("parity") = mParity;


        QString state = currentState["state"].toString();
        QJsonDocument curState = QJsonDocument::fromJson(state.toUtf8());


        mVibrate = curState["vibrate"].toInt(); joState->value("vibrate") = mVibrate;

        int i=0;
        i++;
    }

    m_dbObject->message(0,"Configuration complete.");
}

void Config::writeConfig() {



    QJsonDocument jdConfig(*joConfig);
    m_dbObject->saveState("config", jdConfig);
    m_dbObject->message(0, "Configuration saved: " +jdConfig.toJson());
}

void Config::writeState() {


    QJsonDocument jdState(*joState);
    m_dbObject->saveState("state", jdState);
    m_dbObject->message(0, "State saved: "+ jdState.toJson());
}


int Config::logLevel() { return mLogLevel; } //TODO: Review/remove this
int Config::logLevel(int level) {
    mLogLevel = level;
    m_dbObject->message(0,"Log Level changed to: " + QString::number(level));
    return mLogLevel;
}
void Config::setVibrate(int OnOff) {
    mVibrate = OnOff;
    writeState();
}

QJsonObject *Config::getConfig(){

    //QJsonDocument jdConfig(cfg);
    //return jdConfig.toJson();
    return joConfig;
}
void Config::getState() {

    emit(setState("State object"));
}
/*
void Config::setState(QString msg) {
    int i=0;
    i++;
}*/
void Config::uartWrite() {

}
void Config::message(unsigned char level, QString msg) {
    m_dbObject->message(level, msg);
}

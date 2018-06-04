#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

#include "config.h"
#include "logger.h"
#include "dbobject.h"

//Config::Config() {}
Config::Config(logger *l, dbObject *database) {
    log = l;
    db = database;

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



        log->message(0,"Creating default state record.");
        db->execute("insert into currentState (config, state) values('','');");
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

    log->message(0,"Configuration complete.");
}

void Config::writeConfig() {



    QJsonDocument jdConfig(*joConfig);
    db->saveState("config", jdConfig);
    log->message(0, "Configuration saved: " +jdConfig.toJson());
}

void Config::writeState() {


    QJsonDocument jdState(*joState);
    db->saveState("state", jdState);
    log->message(0, "State saved: "+ jdState.toJson());
}


int Config::logLevel() { return mLogLevel; } //TODO: Review/remove this
int Config::logLevel(int level) {
    mLogLevel = level;
    log->message(0,"Log Level changed to: " + QString::number(level));
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



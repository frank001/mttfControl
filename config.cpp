#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

#include "config.h"
#include "logger.h"
#include "dbobject.h"

Config::Config() {}
Config::Config(logger *l, dbObject *database) {
    log = l;
    db = database;

}

void Config::setConfig(QJsonArray ja) {

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

        mLogLevel = curConfig["logLevel"].toInt();
        mPortName = curConfig["port"].toString();
        mBaudRate = curConfig["baudrate"].toInt();
        mDataBits = curConfig["databits"].toInt();
        mStopBits = curConfig["stopbits"].toInt();
        mParity = curConfig["parity"].toInt();


        QString state = currentState["state"].toString();
        QJsonDocument curState = QJsonDocument::fromJson(state.toUtf8());
        mVibrate = curState["vibrate"].toInt();

        int i=0;
        i++;
    }
    log->message(0,"Configuration complete.");
}

void Config::writeConfig() {

    cfg["logLevel"] = mLogLevel;
    cfg["baudrate"] = mBaudRate;
    cfg["databits"] = mDataBits;
    cfg["stopbits"] = mStopBits;
    cfg["parity"] = mParity;
    cfg["port"] = mPortName;

    QJsonDocument jdConfig(cfg);
    db->saveState("config", jdConfig);
    log->message(0, "Configuration saved: " +jdConfig.toJson());
}

void Config::writeState() {
    state["vibrate"] = mVibrate;
    QJsonDocument jdState(state);
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
    return &cfg;
}

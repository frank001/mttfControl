#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QThread>
#include <QJsonDocument>

#include "handler.h"
#include "logger.h"
#include "uart.h"
#include "main.h"
#include "cycle.h"

Handler::Handler(QObject *parent) : QObject(parent)
{
    //Database initialization
    m_Logger = new Logger("mttfControl", "topSecret", this);


    connect(this, &Handler::logExecute, m_Logger, &Logger::execute);
    connect(this, &Handler::logMessage, m_Logger, &Logger::message);
    connect(this, &Handler::StateChanged, m_Logger, &Logger::saveState);



    setHandlerInitialConfig(logExecute("select top 1 config from currentConfig order by id desc;"));
    setHandlerInitialState(logExecute("select top 1 state from currentState order by id desc;"));



    m_uart = new uart(nullptr, &thread);
    connect(m_uart, &uart::message, this, &Handler::message);
    connect(m_uart, &uart::doorChange, this, &Handler::doorChange);
    thread.start();

    m_Command = new Commands(this);

    m_Cycle = new Cycle(this);


}

void Handler::setHandlerInitialState(QJsonDocument jd) {
    if (jd.isEmpty()) {    //create default state record
        logMessage(DATA|WARN,"Creating default state record.");
        QJsonObject s;
            s.insert("running", QJsonValue::fromVariant(0));
            s.insert("cycles", QJsonValue::fromVariant(0));
            s.insert("hours", QJsonValue::fromVariant(0));
            s.insert("vibrate", QJsonValue::fromVariant(0));
            s.insert("tubes", QJsonValue::fromVariant(0));
            s.insert("light", QJsonValue::fromVariant(0));
            s.insert("door", QJsonValue::fromVariant(0));
        QJsonObject state;
        state.insert("state" , s);
        QJsonDocument jds(state);
        jdState = jds;
        logExecute("insert into currentState (state) values('"+ jdState.toJson()+"');");
    } else {
//        jd.object().value("data")["state"].toString().toUtf8();

        //QJsonObject joResponse = jd.object();
        //QJsonValue jvData = joResponse.value("data");
        //QString json = jvData["state"].toString();
        //QByteArray ba = json.toUtf8();

        jdState = QJsonDocument::fromJson(jd.object().value("data")["state"].toString().toUtf8());

        logMessage(DATA|WATCH,"Current state loaded");      //TODO: check for state differences equipment vs database
    }
}

void Handler::setHandlerInitialConfig(QJsonDocument jd) {
    if (jd.isEmpty()) {    //create default config record
        logMessage(DATA|WARN,"Creating default config record.");
        QJsonObject c;
            c.insert("loglevel", QJsonValue::fromVariant(0));
            c.insert("portname", QJsonValue::fromVariant("COM8"));
            c.insert("baudrate", QJsonValue::fromVariant(115200));
        QJsonObject config;
        config.insert("config" , c);
        QJsonDocument jdc(config);
        jdConfig = jdc;
        sConfig = jdConfig.toJson();
        logExecute("insert into currentConfig (config) values('"+ jdConfig.toJson()+"');");
    } else {
        QJsonObject joResponse = jd.object();
        QJsonValue jvData = joResponse.value("data");
        QString json = jvData["config"].toString();
        QByteArray ba = json.toUtf8();
        jdConfig = QJsonDocument::fromJson(ba);
        logMessage(DATA|WATCH,"Current config loaded");

    }
    logMessage(DATA|WATCH,"Configuration complete.");
}

void Handler::writeConfig() {



    QJsonDocument jdConfig(*joConfig);
    //m_dbObject->saveState("config", jdConfig);
    logMessage(DATA|DEBUG, "Configuration saved: " +jdConfig.toJson());
}

void Handler::writeState() {
    QJsonDocument jdState(*joState);
    //m_dbObject->saveState("state", jdState);
    logMessage(DATA|DEBUG, "State saved: "+ jdState.toJson());
}


int Handler::logLevel() { return mLogLevel; } //TODO: Review/remove this
int Handler::logLevel(int level) {
    mLogLevel = level;
    logMessage(DATA|INFO,"Log Level changed to: " + QString::number(level));
    return mLogLevel;
}
void Handler::setVibrate(int OnOff) {
    mVibrate = OnOff;
    writeState();
}

void Handler::getConfig(){
    emit ConfigChanged(false, jdConfig);
}
void Handler::getState() {
    emit StateChanged(false, jdState);
}

bool Handler::jdUpdateState(QJsonDocument *jd, QString name, QString key, QJsonValue value) {
    QJsonObject joState = jdState.object();
    QJsonValue jvState = joState.value(name);
    QJsonObject joValues = jvState.toObject();

    if (joValues.value(key)==value) return false;

    joValues.remove(key);
    joValues.insert(key, value);

    QJsonObject state;
    state.insert(name , joValues);
    QJsonDocument jds(state);
    jdState = jds;

    return true;
}
bool Handler::jdUpdateConfig(QString name, QString key, QJsonValue value) {
    QJsonObject joConfig = jdConfig.object();
    QJsonValue jvConfig = joConfig.value(name);
    QJsonObject joValues = jvConfig.toObject();

    if (joValues.value(key)==value) return false;

    joValues.remove(key);
    joValues.insert(key, value);

    QJsonObject config;
    config.insert(name , joValues);
    QJsonDocument jdc(config);
    jdState = jdc;

    return true;
}


void Handler::setHandlerState(QString key, QJsonValue value) {
    bool success = jdUpdateState(&jdState, "state", key, value);
    emit StateChanged(success, jdState);
}

void Handler::setHandlerConfig(QString key, QJsonValue value) {
    bool success = jdUpdateConfig("config", key, value);
    emit ConfigChanged(success, jdConfig);


}

void Handler::uartWrite(QByteArray data) {
    //m_uart->write(data);
    //uart(this->serialPort).write(data);
}

void Handler::message(unsigned int level, QString msg) {
    logMessage(level, msg);
}
void Handler::doorChange(int status){
    setHandlerState("door", status);
    emit StateChanged(true, jdState);   //temporary. TODO: sync with equipment when door opens or cycle stops... emit StateChange only once
}
void Handler::lightChange(int level) {

}
void Handler::setHandlerCycleRun(int level) {       //level: start, stop, resume
    setHandlerState("running", level);
    if (level)
        m_Cycle->start();
    else
        m_Cycle->stop();
}

void Handler::getHandlerState() {
    emit StateChanged(false, jdState);
}
void Handler::getHandlerConfig() {
    //QString test = jdConfig.toJson();
    emit ConfigChanged(false, jdConfig);
}
void Handler::setHandlerCycleIncrement() {
    //bool success = jdUpdate(jdState, "state", key, value);
    //emit StateChanged(success, jdState);
}

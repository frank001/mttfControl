#include <QCoreApplication>
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



Handler::Handler(QCoreApplication *parent) : QObject(parent)
{
    //Database initialization
    m_Logger = new Logger("mttfControl", "topSecret", this);


    connect(this, &Handler::logExecute, m_Logger, &Logger::execute);
    connect(this, &Handler::logMessage, m_Logger, &Logger::message);
    connect(this, &Handler::StateChanged, m_Logger, &Logger::saveState);
    connect(this, &Handler::ConfigChanged, m_Logger, &Logger::saveConfig);


    setHandlerInitialConfig(logExecute("select top 1 config from currentConfig order by id desc;"));
    setHandlerInitialState(logExecute("select top 1 state from currentState order by id desc;"));
    setHandlerInitialCycle(logExecute("select top 1 data from cycles order by id desc;"));

    m_uart = new uart(joConfig["portname"].toString(), joConfig["baudrate"].toInt(), &thread);                        //TODO: review this. The timer error message is a qt bug. remove the threading.
    connect(m_uart, &uart::message, this, &Handler::message);
    connect(m_uart, &uart::doorChange, this, &Handler::setHandlerDoor);
    connect(this, &Handler::closePort, m_uart, &uart::closePort);
    m_uart->initialize();
    thread.start();

    m_Command = new Commands(this);

    QJsonObject ports;
    ports.insert("ports" , m_uart->joPorts);
    QJsonDocument jdp(ports);
    QString test = jdp.toJson();
    jdPorts = jdp;

    m_Command->jdPorts = jdp;

    m_Cycle = new Cycle(this);



    message(COMMAND|INFO, "Initializing MTTF equipment");

    //TODO: if state is running at startup, cancel initialization and log error. restart cycle from zero.

    m_CycleCount = joState["cycles"].toInt();
    /*m_Command->HandleRaw("setLight", QString::number(joState["light"].toInt()));
    m_Command->HandleRaw("setVibrate", QString::number(joState["vibrate"].toInt()));

    m_Command->HandleRaw("setCycle", QString::number(joState["running"].toInt()));
    m_Command->HandleRaw("setTubes", QString::number(joState["tubes"].toInt()));
    */


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

        jdState = QJsonDocument::fromJson(jd.object().value("data")["state"].toString().toUtf8());
        joState = jdState.object().value("state").toObject();
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
        jdConfig = QJsonDocument::fromJson(jd.object().value("data")["config"].toString().toUtf8());
        joConfig = jdConfig.object().value("config").toObject();
        logMessage(DATA|WATCH,"Current config loaded");

    }
    logMessage(DATA|WATCH,"Configuration complete.");
}

void Handler::setHandlerInitialCycle(QJsonDocument jd) {
    if (jd.isEmpty()) { //create default cycle record
        logMessage(DATA|WARN,"Creating default cycle record.");
        QJsonObject c;

        c.insert("description", QJsonValue::fromVariant("Default Cycle"));
        c.insert("remarks", QJsonValue::fromVariant("Created by mttfControl"));
        QJsonArray ja = QJsonArray();
        for (int i=0;i<18;i++) {
            ja.push_back(QJsonValue::fromVariant("EMPTY" + QString::number(i)));
        }


        c.insert("positions",  ja);

        QJsonObject cycle;
        cycle.insert("cycle" , c);
        QJsonDocument jdc(cycle);
        jdCycle = jdc;
        sCycle = jdCycle.toJson();
        logExecute("insert into cycles (data) values('"+ jdCycle.toJson()+"');");
    } else {
        QString jds = jd.toJson();
        jdCycle = QJsonDocument::fromJson(jd.object().value("data")["data"].toString().toUtf8());
        joCycle = jdCycle.object().value("cycle").toObject();
        sCycle = jdCycle.toJson();
        logMessage(DATA|WATCH,"Current cycle loaded");
    }
}

int Handler::logLevel() { return mLogLevel; } //TODO: Review/remove this
int Handler::logLevel(int level) {
    mLogLevel = level;
    logMessage(DATA|INFO,"Log Level changed to: " + QString::number(level));
    return mLogLevel;
}




bool Handler::jdUpdateState(QString name, QString key, QJsonValue value) {
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
    joState = jdState.object();
    return true;
}
bool Handler::jdUpdateConfig(QString name, QString key, QJsonValue value) {
    QJsonObject joc = jdConfig.object();
    QJsonValue jvc = joc.value(name);
    QJsonObject joValues = jvc.toObject();

    if (joValues.value(key)==value) return false;

    joValues.remove(key);
    joValues.insert(key, value);

    QJsonObject config;
    config.insert(name , joValues);
    QJsonDocument jdc(config);
    jdConfig = jdc;
    joConfig = jdConfig.object();
    return true;
}


void Handler::setHandlerState(QString key, QJsonValue value) {
    bool success = jdUpdateState("state", key, value);
    emit StateChanged(success, jdState);
}

void Handler::setHandlerConfig(QString key, QJsonValue value) {
    bool success = jdUpdateConfig("config", key, value);
    emit ConfigChanged(success, jdConfig);
}

void Handler::message(unsigned int level, QString msg) {
    logMessage(level, msg);
}
void Handler::setHandlerDoor(int status){
    //TODO: sync state with machine and log error
    setHandlerState("door", status);
    emit StateChanged(true, jdState);   //temporary. TODO: sync with equipment when door opens or cycle stops... emit StateChange only once
}

void Handler::setHandlerCycle(int level) {       //level: start, stop, resume
    if (level)
        m_Cycle->start();
    else
        //TODO: sync timerstate with database (new table) and implement resume function
        m_Cycle->stop();
    //m_Command->HandleRaw()
    setHandlerState("running", level);
}

void Handler::getHandlerState() {
    emit StateChanged(false, jdState);
}
void Handler::getHandlerConfig() {
    emit ConfigChanged(false, jdConfig);
}
void Handler::getHandlerCycle() {
    emit CycleChanged(false, jdCycle);
}

void Handler::setHandlerPosition(QString value, QString) {
    //TODO: implement new cycle setting, description and remarks.
    logMessage(DATA|ERROR, "setPosition not yet implemented.");
}
void Handler::newHandlerCycle(QString todo, QJsonValue value) {
    QJsonObject jo = value.toObject();
    QJsonDocument jd = QJsonDocument::fromVariant(value.toVariant());
    logExecute("insert into cycles (data) values ('" + jd.toJson() +"');");
    setHandlerCycle(0);     //stop any running cycle
    m_CycleCount = 0;       //reset cycle count
    setHandlerState("cycles", QJsonValue(m_CycleCount));
    logMessage(DATA|INFO, "New Cycle saved.");
}


void Handler::incrementHandlerCycle() {
    m_CycleCount++;
    jdUpdateState("state", "cycles", QJsonValue(m_CycleCount));
}
void Handler::resetHandlerUart(QString newConfig) {
    int i=0;
    QStringList data = newConfig.split(",");
    jdUpdateConfig("config", "portname", data[0]);
    jdUpdateConfig("config", "baudrate", data[1].toInt());
    emit ConfigChanged(true, jdConfig);

    emit closePort();
    thread.exit();
    m_uart = new uart(joConfig["portname"].toString(), joConfig["baudrate"].toInt(), &thread);
    thread.start();
    //TODO: restruct Handler / uart / command / socket
    //  Handler
    //      - LOGGER
    //      - SERVER
    //          - SOCKET
    //          - CLIENT
    //          - TASK
    //      - UART
    //          - PORT
    //      - COMMAND
    //          - CYCLE



}

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


Handler::Handler(QObject *parent) : QObject(parent)
{
    m_Logger = new Logger("mttfControl", "topSecret", this);
    connect(this, &Handler::logExecute, m_Logger, &Logger::execute);
    connect(this, &Handler::logMessage, m_Logger, &Logger::message);

    //Database initialization

    /*
    logMessage(DATA|DEBUG, "DEBUG: Accessing database.");
    logMessage(DATA|WATCH, "WATCH: Accessing database.");
    logMessage(DATA|INFO, "INFO: Accessing database.");
    logMessage(DATA|WARN, "WARN: Accessing database.");
    logMessage(DATA|ERROR, "ERROR: Accessing database.");
    logMessage(DATA|FATAL, "FATAL: Accessing database.");
    */

    setConfig(logExecute("select top 1 config, state from currentState order by id desc;"));


    //Serial port initialization
    //TODO: implement synced communication (open/close port on each command?) on error stop MTTF run
    //m_SerialPort = new QSerialPort();
    //m_SerialPort->setPortName("COM8");
    //m_SerialPort->setBaudRate(QSerialPort::Baud115200);
    //m_SerialPort->open(QIODevice::WriteOnly);


    m_uart = new uart(nullptr, &thread);
    connect(m_uart, &uart::message, this, &Handler::message);
    connect(m_uart, &uart::doorChange, this, &Handler::doorChange);
    thread.start();


    //connect(m_uart, &uart::message, this, &Handler::message);
    //log = l;
    //db = database;

}


void Handler::setConfig(QJsonArray ja) {
    QJsonObject c;
        c.insert("loglevel", QJsonValue::fromVariant(0));
        c.insert("portname", QJsonValue::fromVariant("COM8"));
        c.insert("baudrate", QJsonValue::fromVariant(115200));
    QJsonObject config;
    config.insert("config" , c);
    QJsonDocument jdc(config);
    jdConfig = jdc;

    QJsonObject s;
        s.insert("vibrate", QJsonValue::fromVariant(0));
        s.insert("tubes", QJsonValue::fromVariant(0));
        s.insert("01mlux", QJsonValue::fromVariant(0));
        s.insert("5mlux", QJsonValue::fromVariant(0));
        s.insert("50lux", QJsonValue::fromVariant(0));
        s.insert("door", QJsonValue::fromVariant(0));
    QJsonObject state;
    state.insert("state" , s);
    QJsonDocument jds(state);
    jdState = jds;




    //QString test = jdState.toJson();
    //QString test = jdConfig.toJson();


    /*
    joConfig = new QJsonObject();
    joConfig->insert("loglevel", mLogLevel);
    joConfig->insert("baudrate", mBaudRate);
    joConfig->insert("databits", mDataBits);
    joConfig->insert("stopbits", mStopBits);
    joConfig->insert("parity", mParity);
    joConfig->insert("port", mPortName);
    joState = new QJsonObject();
    joState->insert("vibrate", mVibrate);
    joState->insert("tubes", 0);
    */
    if (ja.count()==0) {    //create default state record
       //QJsonDocument jdConfig(cfg);



        logMessage(DATA|WARN,"Creating default state record.");
        logExecute("insert into currentState (config, state) values('"+ jdConfig.toJson()+"','"+jdState.toJson()+"');");
        //writeConfig();
        //writeState();


    } else { //read current state

        //jdConfig = QJsonDocument::fromJson(ja.at(0).t);
        //jdState = QJsonDocument::fromJson(ja.at(1).toArray());

        //QJsonDocument jdData;
        //jdConfig = QJsonDocument::fromJson(); ///   :fromJson((QJsonValue)ja.at(0));

        //TODO:!!!!!!!!!
        QString c = jdConfig.toJson();
        QString s = jdState.toJson();

        //QJsonObject currentState = ja[0].toObject();
        //QString config = currentState["config"].toString();
        //QJsonDocument curConfig = QJsonDocument::fromJson(config.toUtf8());

        //TODO: create function to change variables syncing the objects joConfig and joState
        //OR: beter to drop the class declarations and work directly with json instead
        /*
        mLogLevel = curConfig["loglevel"].toInt(); joConfig->value("loglevel") = mLogLevel;
        mPortName = curConfig["port"].toString(); joConfig->value("port") = mPortName;
        mBaudRate = curConfig["baudrate"].toInt(); joConfig->value("baudrate") = mBaudRate;
        mDataBits = curConfig["databits"].toInt(); joConfig->value("databits") = mDataBits;
        mStopBits = curConfig["stopbits"].toInt(); joConfig->value("stopbits") = mStopBits;
        mParity = curConfig["parity"].toInt(); joConfig->value("parity") = mParity;
        */

        //QString state = currentState["state"].toString();
        //QJsonDocument curState = QJsonDocument::fromJson(state.toUtf8());


        //mVibrate = curState["vibrate"].toInt(); joState->value("vibrate") = mVibrate;

        int i=0;
        i++;
    }

    logMessage(DATA|INFO,"Configuration complete.");
}

void Handler::writeConfig() {



    QJsonDocument jdConfig(*joConfig);
    m_dbObject->saveState("config", jdConfig);
    logMessage(DATA|DEBUG, "Configuration saved: " +jdConfig.toJson());
}

void Handler::writeState() {


    QJsonDocument jdState(*joState);
    m_dbObject->saveState("state", jdState);
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
    emit ConfigChanged(jdConfig);
}
void Handler::getState() {
    emit StateChanged(jdState);
}

void Handler::setState(QString key, QJsonValue value) {
    QJsonObject joState = jdState.object();
    QJsonValue jvState = joState.value("state");
    QJsonObject joValues = jvState.toObject();

    joValues.remove(key);
    joValues.insert(key, value);


    QJsonObject state;
    state.insert("state" , joValues);
    QJsonDocument jds(state);
    QString test = jds.toJson();
    jdState = jds;
    jdState.fromJson(jds.toJson());
    int j=0;
    //emit StateChanged(jdState);

}

void Handler::setConfig(QString, QJsonValue value) {

}

void Handler::uartWrite(QByteArray data) {
    //m_uart->write(data);
    //uart(this->serialPort).write(data);
}
void Handler::message(unsigned int level, QString msg) {
    QString config = jdConfig.toJson();
    QString state = jdState.toJson();
    logMessage(level, msg);
}
void Handler::doorChange(int status){
    setState("door", status);
    emit StateChanged(jdState);
}

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

#include "handler.h"
#include "logger.h"
#include "uart.h"
#include "main.h"


Handler::Handler(QSerialPort *port, Logger *database, QObject *parent) :
    QObject(parent),
    m_dbObject(database),
    m_SerialPort(port)
{
    m_SerialPort.setPortName("COM8");                   //TODO: move this after setConfig
    m_SerialPort.setBaudRate(QSerialPort::Baud115200);
    m_SerialPort.open(QIODevice::WriteOnly);
    m_uart = new uart(&m_SerialPort);

    connect(m_uart, &uart::message, this, &Handler::message);
    //log = l;
    //db = database;

}

void Handler::setConfig(QJsonArray ja) {
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

    if (ja.count()==0) {    //create default state record
       //QJsonDocument jdConfig(cfg);



        m_dbObject->message(DATA|WARN,"Creating default state record.");
        m_dbObject->execute("insert into currentState (config, state) values('','');");
        writeConfig();
        writeState();


    } else { //read current state

        QJsonObject currentState = ja[0].toObject();
        QString config = currentState["config"].toString();
        QJsonDocument curConfig = QJsonDocument::fromJson(config.toUtf8());

        //TODO: create function to change variables syncing the objects joConfig and joState
        //OR: beter to drop the class declarations and work directly with json instead

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

    m_dbObject->message(DATA|INFO,"Configuration complete.");
}

void Handler::writeConfig() {



    QJsonDocument jdConfig(*joConfig);
    m_dbObject->saveState("config", jdConfig);
    m_dbObject->message(DATA|DEBUG, "Configuration saved: " +jdConfig.toJson());
}

void Handler::writeState() {


    QJsonDocument jdState(*joState);
    m_dbObject->saveState("state", jdState);
    m_dbObject->message(DATA|DEBUG, "State saved: "+ jdState.toJson());
}


int Handler::logLevel() { return mLogLevel; } //TODO: Review/remove this
int Handler::logLevel(int level) {
    mLogLevel = level;
    m_dbObject->message(DATA|INFO,"Log Level changed to: " + QString::number(level));
    return mLogLevel;
}
void Handler::setVibrate(int OnOff) {
    mVibrate = OnOff;
    writeState();
}

QJsonObject *Handler::getConfig(){

    //QJsonDocument jdConfig(cfg);
    //return jdConfig.toJson();
    return joConfig;
}
void Handler::getState() {

    //emit(setState("State object"));
}

void Handler::setState(QString key, QJsonValue value) {
    //this->joState->value(key) = value;
    joState->remove(key);
    joState->insert(key, value);
}

void Handler::uartWrite(QByteArray data) {
    //m_uart->write(data);
    //uart(this->serialPort).write(data);
}
void Handler::message(unsigned char level, QString msg) {
    m_dbObject->message(level, msg);
}

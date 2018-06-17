#include <QMetaEnum>
#include <QJsonDocument>

#include "commands.h"
#include "handler.h"
#include "uart.h"
#include "main.h"


Commands::Commands(QObject *parent) :
    QObject(parent)

{

    connect(this, &Commands::message, (Handler*)parent, &Handler::message);
    connect(this, &Commands::writeUart, ((Handler*)parent)->m_uart, &uart::write);

    connect(this, &Commands::getHandlerState, (Handler*)parent, &Handler::getHandlerState);
    connect(this, &Commands::getHandlerConfig, (Handler*)parent, &Handler::getHandlerConfig);

    connect(this, &Commands::setHandlerState, (Handler*)parent, &Handler::setHandlerState);


    connect(this, &Commands::setHandlerCycleRun, (Handler*)parent, &Handler::setHandlerCycleRun);


    message(COMMAND|WATCH, "Commands initialized.");



    //connect(m_Handler, &Handler::startCycle, m_cycle, &Cycle::start);

}



QJsonObject joFromString(const QString& in) {
    QJsonObject obj;

        QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

        // check validity of the document
        if(!doc.isNull()) {
            if(doc.isObject()) {
                obj = doc.object();
            }
            else {
                obj = QJsonDocument::fromJson("{ \"Error\":\"Document is not an object\"};").object();
                //qInfo() << "Document is not an object";
            }
        }
        else {
            obj = QJsonDocument::fromJson("{ \"Error\":\"Invalid JSON.\"};").object();
            //qInfo() << "Invalid JSON...\n";
        }

        return obj;
}

QByteArray Commands::HandleRaw(QString cmd, QString value){
    QMetaObject MetaObject = this->staticMetaObject;
    MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eCommands"));

    message(COMMAND|DEBUG, "Command received: " + cmd + ": " + value);

    QString name="TODO";
    QJsonDocument jdResponse;

    QString msg;
    int command =MetaEnum.keysToValue(cmd.toLatin1());
    switch (command) {
    case getConfig:
        message(COMMAND|DEBUG, "Returning config.");
        //m_Handler->getConfig();
        //((Handler*)parent)->getConfig();
        getHandlerConfig();

        break;
    case setConfig:
        message(COMMAND|WARN, "Setting config: (TODO)");
        break;
    case getState:
        message(COMMAND|DEBUG, "Returning state.");
        getHandlerState();
        //((Handler*)parent)->getState();
        //m_Handler->getState();

        break;
    case setState:
        message(COMMAND|WARN, "Setting state: (TODO)"); //is this really needed here? state is set by commands or cycle commands
        break;
    case setVibrate:
        message(COMMAND|DEBUG, "Vibrate: " + value);
        value=="0"?writeUart("l7"):writeUart("h7");
        //m_Handler->setState("vibrate", value.toInt());
        setHandlerState("vibrate", value.toInt());
        break;
    case setTubes:
        message(COMMAND|DEBUG, "Tubes: " + value);
        value=="0"?writeUart("l4"):writeUart("h4");
        setHandlerState("tubes", value.toInt());
        //m_Handler->setState("tubes", value.toInt());
        break;
    case setLight:
        message(COMMAND|DEBUG, "Light: " + value);

        //m_Handler->setState("light", value.toInt());
        switch (value.toInt()) {
        case (llOff):
            writeUart("l1");    //lights off
            break;
        case (ll01mlux):
            writeUart("l1");
            writeUart("h6");
            writeUart("h5");
            writeUart("h1");
            break;
        case (ll5mlux):
            writeUart("l1");
            writeUart("l6");
            writeUart("h5");
            writeUart("h1");
            break;
        case (ll50lux):
            writeUart("l6");
            writeUart("l5");
            writeUart("h1");
            break;
        }
        setHandlerState("light", value.toInt());
        break;
    case getStatus:
        writeUart("all");
        break;
    case setCycle:
        message(COMMAND|INFO, "Cycle running: " + value);
        setHandlerCycleRun(value.toInt());

        //m_cycle->start();       //TODO.
        break;

    default:
        message(COMMAND|ERROR, "Unknown command: " + cmd +": " + value);
        QJsonObject error;
        error.insert("command", QJsonValue::fromVariant(cmd +": " + value));
        error.insert("message", QJsonValue::fromVariant("unknown command."));

        QJsonObject joResponse;
        joResponse.insert("error" , error);

        QJsonDocument jdError(joResponse);
        jdResponse = jdError;

        break;

    }
    QString test = jdResponse.toJson();

    return jdResponse.toBinaryData();

}

QByteArray Commands::Handle(QString word){

    //QMetaObject MetaObject = this->staticMetaObject;
    //MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eCommands"));

    QJsonObject request = joFromString(word);






    QString cmd = request.value("command").toString();
    QString value = request.value("value").toString();
    return HandleRaw(cmd, value);
}


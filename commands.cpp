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
    connect(this, &Commands::getHandlerCycle, (Handler*)parent, &Handler::getHandlerCycle);

    connect(this, &Commands::setHandlerPosition, (Handler*)parent, &Handler::setHandlerPosition);
    connect(this, &Commands::newHandlerCycle, (Handler*)parent, &Handler::newHandlerCycle);

    connect(this, &Commands::setHandlerState, (Handler*)parent, &Handler::setHandlerState);


    connect(this, &Commands::setHandlerCycle, (Handler*)parent, &Handler::setHandlerCycle);
    connect(this, &Commands	::resetHandlerUart, (Handler*)parent, &Handler::resetHandlerUart);

    message(COMMAND|WATCH, "Commands initialized.");

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

QByteArray Commands::HandleRaw(QString cmd, QJsonValue value){
    QMetaObject MetaObject = this->staticMetaObject;
    MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eCommands"));

    message(COMMAND|DEBUG, "Command received: " + cmd + ": " + value.toString());

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

        break;
    case setState:
        message(COMMAND|WARN, "Setting state: (TODO)"); //is this really needed here? state is set by commands or cycle commands
        break;
    case setVibrate:
        message(COMMAND|DEBUG, "Vibrate: " + value.toString());
        value=="0"?writeUart("l7"):writeUart("h7");
        setHandlerState("vibrate", value.toString().toInt());
        break;
    case setTubes:
        message(COMMAND|DEBUG, "Tubes: " + value.toString());
        value=="0"?writeUart("l4"):writeUart("h4");
        setHandlerState("tubes", value.toString().toInt());
        break;
    case setLight:
        message(COMMAND|DEBUG, "Light: " + value.toString());
        switch (value.toString().toInt()) {
        case (lightlevel0):
            writeUart("l1");    //lights off
            break;
        case (lightlevel1):
            writeUart("l1");
            writeUart("h6");
            writeUart("h5");
            writeUart("h1");
            break;
        case (lightlevel2):
            writeUart("l1");
            writeUart("l6");
            writeUart("h5");
            writeUart("h1");
            break;
        case (lightlevel3):
            writeUart("l6");
            writeUart("l5");
            writeUart("h1");
            break;
        }
        setHandlerState("light", value);
        break;
    case getStatus:
        writeUart("all");
        break;
    case setCycle:
        message(COMMAND|INFO, "Cycle running: " + value.toString());
        setHandlerCycle(value.toString().toInt());
        break;
    case getPorts:
        jdResponse = jdPorts;
        break;
    case resetUart:
        resetHandlerUart(value.toString());
        break;
    case setPosition:
        setHandlerPosition(value.toString(), "");
        //message(COMMAND|ERROR, "Command not yet implemented (TODO): " + cmd +": " + value);
        break;
    case newCycle:
        newHandlerCycle("", value);
        //message(COMMAND|ERROR, "Command not yet implemented (TODO): " + cmd +": " + value);
        break;
    case getCycle:
        message(COMMAND|DEBUG, "Returning cycle.");
        getHandlerCycle();
        break;
    default:
        message(COMMAND|ERROR, "Unknown command: " + cmd +": " + value.toString());
        QJsonObject error;
        error.insert("command", QJsonValue::fromVariant(cmd +": " + value.toString()));
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
    QJsonObject request = joFromString(word);
    QString cmd = request.value("command").toString();
    //QString value = request.value("value"); //.toString();
    //TODO: pass value as QJsonValue
    return HandleRaw(cmd, request.value("value"));
}


#include <QMetaEnum>
#include <QJsonDocument>
#include "commands.h"
#include "handler.h"
#include "uart.h"
#include "main.h"

Commands::Commands(Handler *config, QObject *parent) :
    QObject(parent),
    m_Handler(config)
{

    connect(this, &Commands::message, m_Handler, &Handler::message);
    connect(this, &Commands::writeUart, m_Handler->m_uart, &uart::write);

    connect(this, &Commands::setHandlerState, m_Handler, &Handler::setState);

    //connect(this, &Commands::setHandlerConfig, m_Handler, &Handler::setConfig);

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

void Commands::setLightOff() {
    writeUart("l1");
    //QThread::currentThread()->sleep(10);
}

QByteArray Commands::Handle(QString word){

    QMetaObject MetaObject = this->staticMetaObject;
    QMetaEnum MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eCommands"));

    QJsonObject request = joFromString(word);

    QString name="TODO";

    QJsonDocument jdResponse;




    QString cmd = request.value("command").toString();
    message(COMMAND|DEBUG, "Command received: " + cmd);
    QString value = request.value("value").toString();
    QString msg;
    switch (MetaEnum.keysToValue(cmd.toLatin1())) {
    case getConfig:
        message(COMMAND|DEBUG, "Returning config.");
        //jdResponse = m_Handler->jdConfig;
        m_Handler->getConfig();

        break;
    case setConfig:
        message(COMMAND|WARN, "Setting config: (TODO)");
        break;
    case getState:
        message(COMMAND|DEBUG, "Returning state.");
        m_Handler->getState();
        //jdResponse = m_Handler->jdState;
        break;
    case setState:
        message(COMMAND|WARN, "Setting state: (TODO)");
        break;
    case setVibrate:
        message(COMMAND|DEBUG, "Vibrate: " + value);
        value=="0"?writeUart("l7"):writeUart("h7");
        m_Handler->setState("vibrate", value);
        m_Handler->getState();
        //jdResponse = m_Handler->jdState;
        break;
    case setTubes:
        message(COMMAND|DEBUG, "Tubes: " + value);
        value=="0"?writeUart("l4"):writeUart("h4");
        m_Handler->setState("tubes", value);
        m_Handler->getState();
        //jdResponse = m_Handler->jdState;
        break;
    case set01mlux:

        writeUart("l1"); //setLightOff();
        writeUart("h6");
        writeUart("h5");
        writeUart("h1");
        QThread::currentThread()->sleep(10);
        m_Handler->setState("01mlux",1);
        m_Handler->setState("5mlux",0);
        m_Handler->setState("50lux",0);
        m_Handler->getState();
        break;
    case set5mlux:

        writeUart("l1"); //setLightOff();
        writeUart("l6");
        writeUart("h5");
        writeUart("h1");
        QThread::currentThread()->sleep(10);
        m_Handler->setState("01mlux",0);
        m_Handler->setState("5mlux",1);
        m_Handler->setState("50lux",0);
        m_Handler->getState();
        break;
    case set50lux:
        //setLightOff();
        writeUart("l6");
        writeUart("l5");
        writeUart("h1");
        QThread::currentThread()->sleep(10);
        m_Handler->setState("01mlux",0);
        m_Handler->setState("5mlux",0);
        m_Handler->setState("50lux",1);
        m_Handler->getState();
        break;
    case setLights:
        writeUart("l1");
        QThread::currentThread()->sleep(10);
        m_Handler->setState("01mlux",0);
        m_Handler->setState("5mlux",0);
        m_Handler->setState("50lux",0);
        m_Handler->getState();
        //setLightOff();
        break;
    case getStatus:
        writeUart("all");
        break;
    default:
        message(NETWORK|COMMAND|ERROR, "Unknown command: " + word);
        QJsonObject error;
        error.insert("command", QJsonValue::fromVariant(cmd));
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

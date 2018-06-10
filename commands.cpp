#include "commands.h"
//#include <QMetaObject>

#include <QMetaEnum>
#include <QJsonDocument>
//#include <QDebug>
#include "handler.h"
#include "uart.h"
#include "main.h"



Commands::Commands(Handler *config, QObject *parent) :
    QObject(parent),
    m_Handler(config)
{

    connect(this, &Commands::message, m_Handler, &Handler::message);
    connect(this, &Commands::writeUart, m_Handler->m_uart, &uart::write);
    message(COMMAND|INFO, "Commands initialized.");
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


QByteArray Commands::Handle(QString word){

    QMetaObject MetaObject = this->staticMetaObject;
    QMetaEnum MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eCommands"));

    QJsonObject request = joFromString(word);
    //Writer.write("l7\r");

    int i=0;
    int j=0;
    QString name="Response";
    QJsonObject *joResponse = new QJsonObject();
    QString cmd = request.value("command").toString();
    message(COMMAND|DEBUG, "Command received: " + cmd);
    QString value = request.value("value").toString();
    QString msg;
    switch (MetaEnum.keysToValue(cmd.toLatin1())) {
    case getConfig:
        message(COMMAND|DEBUG, "Returning config.");
        joResponse = m_Handler->joConfig;
        name="config";
        break;
    case setConfig:
        message(COMMAND|WARN, "Setting config: (TODO)");
        j++;
        break;
    case getState:
        message(COMMAND|DEBUG, "Returning state.");
        joResponse = m_Handler->joState;
        name="state";
        break;
    case setState:
        message(COMMAND|WARN, "Setting state: (TODO)");
        break;
    case setVibrate:
        message(COMMAND|DEBUG, "Vibrate: " + value);
        value=="0"?writeUart("l7\r"):writeUart("h7\r");
        m_Handler->setState("vibrate", value);
        joResponse = m_Handler->joState;
        name="state";
        break;
    case setTubes:
        message(COMMAND|DEBUG, "Tubes: " + value);
        value=="0"?writeUart("l4\r"):writeUart("h4\r");
        m_Handler->setState("tubes", value);
        joResponse = m_Handler->joState;
        name="state";
        break;
    default:
        message(NETWORK|COMMAND|ERROR, "Unknown command: " + word);
        joResponse->insert("unknown command", word);
        break;

    }

    QJsonDocument jdConfig(*joResponse);
    QString response = "{ \"" + name + "\": "+ jdConfig.toJson() + " };";
    QByteArray ba = response.toLatin1();
    return ba;


}

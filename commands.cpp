#include "commands.h"
//#include <QMetaObject>
#include <QMetaEnum>
#include "config.h"
#include "uart.h"

Commands::Commands(Config *config, QObject *parent) :
    QObject(parent),
    m_Config(config)
{
    connect(this, &Commands::message, m_Config, &Config::message);
}

void Commands::Handle(QString word){

    QMetaObject MetaObject = this->staticMetaObject;
    QMetaEnum MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eCommands"));

    //Writer.write("l7\r");
    message(0, "Command received: " + word);
    int i=0;
    int j=0;
    switch (MetaEnum.keysToValue(word.toLatin1())) {
    case getConfig:
        message(0, "Returning config: (TODO)");   //TODO

        i++;
        break;
    case setConfig:
        message(0, "Setting config: (TODO)");   //TODO
        m_Config->uartWrite("l7\r");
        j++;

        break;
    case getState:
        message(0, "Returning state: (TODO)");   //TODO
        break;
    case setState:
        message(0, "Setting state: (TODO)");   //TODO
        break;
    case setVibrateOn:
        m_Config->uartWrite("h7\r");
        break;
    case setVibrateOff:
        m_Config->uartWrite("l7\r");
        break;
    default:
        message(0, "Unknown command: " + word);
        break;

    }


}

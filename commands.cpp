#include "commands.h"
//#include <QMetaObject>
#include <QMetaEnum>
#include "uart.h"

Commands::Commands(QObject *parent) : QObject(parent) {
    int i=0;
    i++;
    //cfg = parent;


    //switchString("getConfig");
    //switchString("setConfig");
}

void Commands::Handle(QString word){

    QMetaObject MetaObject = this->staticMetaObject;
    QMetaEnum MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eCommands"));

    //Writer.write("l7\r");

    int i=0;
    int j=0;
    switch (MetaEnum.keysToValue(word.toLatin1())) {
    case getConfig:
        i++;

        break;
    case setConfig:
        j++;

        break;
    case getState:
        break;
    case setState:
        break;

    }


}

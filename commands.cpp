#include "commands.h"
//#include <QMetaObject>
#include <QMetaEnum>


Commands::Commands(QObject *parent) : QObject(parent) {
    int i=0;
    i++;
    //switchString("getConfig");
    //switchString("setConfig");
}

void Commands::Handle(QString word){

    QMetaObject MetaObject = this->staticMetaObject;
    QMetaEnum MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("eCommands"));

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

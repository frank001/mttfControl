#ifndef COMMANDS_H
#define COMMANDS_H

#include <QObject>
#include <QMetaEnum>
#include <QJsonDocument>
#include "uart.h"


class Commands : public QObject
{
    Q_OBJECT
    Q_ENUMS(eCommands)

public:
    explicit Commands(QObject *parent = nullptr);
    enum eCommands {
        getConfig, setConfig, getState, setState, setVibrate, setTubes, setLight, getStatus,
        setCycle, stopCycle, getPorts, resetUart
                   };
    enum eLightLevel { lightlevel0, lightlevel1, lightlevel2, lightlevel3 };
    QJsonDocument jdPorts;


private:
    QMetaEnum MetaEnum;

signals:
    void message(unsigned int level, QString msg);
    void writeUart(const QByteArray &writeData);

    void setHandlerState(QString, QJsonValue value);
    void setHandlerConfig(QString, QJsonValue value);

    void setHandlerCycle(int);

    void getHandlerState();
    void getHandlerConfig();

    void resetHandlerUart(QString);



public slots:
    QByteArray Handle(QString);
    QByteArray HandleRaw(QString, QString);


};

#endif // COMMANDS_H

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QObject>
#include <QMetaEnum>
#include <QJsonObject>
#include "handler.h"
#include "uart.h"


class Commands : public QObject
{
    Q_OBJECT
    Q_ENUMS(eCommands)

public:
    explicit Commands(Handler *, QObject *parent = nullptr);
    enum eCommands { getConfig, setConfig, getState, setState, setVibrate, setTubes, set01mlux, set5mlux, set50lux, setLights, getStatus };

private:
    Handler *m_Handler;
    void setLightOff();


signals:
    void message(unsigned int level, QString msg);
    void writeUart(const QByteArray &writeData);

    void setHandlerState(QString, QJsonValue value);
    void setHandlerConfig(QString, QJsonValue value);

    uart *getUart();

public slots:
    QByteArray Handle(QString);

};

#endif // COMMANDS_H

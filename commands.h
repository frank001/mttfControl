#ifndef COMMANDS_H
#define COMMANDS_H

#include <QObject>
#include <QMetaEnum>
#include <QJsonObject>
//#include "handler.h"
#include "uart.h"


class Commands : public QObject
{
    Q_OBJECT
    Q_ENUMS(eCommands)

public:
    explicit Commands(QObject *parent = nullptr);
    enum eCommands { getConfig, setConfig, getState, setState, setVibrate, setTubes, setLight, getStatus, setCycle, stopCycle };
    enum eLightLevel { llOff, ll01mlux, ll5mlux, ll50lux };
    //Handler *m_Handler;

private:
    QMetaEnum MetaEnum;

signals:
    void message(unsigned int level, QString msg);
    void writeUart(const QByteArray &writeData);

    void setHandlerState(QString, QJsonValue value);
    void setHandlerConfig(QString, QJsonValue value);

    void setHandlerCycleRun(int);

    void getHandlerState();
    void getHandlerConfig();
    uart *getUart();


public slots:
    QByteArray Handle(QString);
    QByteArray HandleRaw(QString, QString);


};

#endif // COMMANDS_H

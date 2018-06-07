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
    enum eCommands { getConfig, setConfig, getState, setState, setVibrate, setTubes };

    QJsonObject *m_Response;

private:
    Handler *m_Handler;


signals:
    void message(unsigned char level, QString msg);
    void writeUart(const QByteArray &writeData);
public slots:
    QByteArray Handle(QString);

};

#endif // COMMANDS_H

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QObject>
#include <QMetaEnum>


class Commands : public QObject
{
    Q_OBJECT
    Q_ENUMS(eCommands)

public:
    explicit Commands(QObject *parent = nullptr);
    enum eCommands { getConfig, setConfig, getState, setState };


signals:

public slots:
    void Handle(QString);

};

#endif // COMMANDS_H

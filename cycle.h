#ifndef CYCLE_H
#define CYCLE_H

#include <QObject>

class cycle : public QObject
{
    Q_OBJECT
public:
    explicit cycle(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CYCLE_H
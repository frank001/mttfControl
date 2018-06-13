#ifndef CYCLE_H
#define CYCLE_H

#include <QObject>
#include <QTimer>
//#include "uart.h"
//#include "commands.h"

class cycle : public QObject
{
    Q_OBJECT
private:
    QTimer m_TmrCycle;
    QTimer m_TmrVibrate;
    QTimer m_TmrTubes;
    QTimer m_Tmr01mlux;
    QTimer m_Tmr5mlux;
    QTimer m_Tmr50mlux;

    bool m_bCycle;
    bool m_bVibrate;
    bool m_bTubes;
    bool m_b01mlux;
    bool m_b5mlux;
    bool m_b50lux;

    unsigned int m_CycleStart=0;
    unsigned int m_CycleDuration=720;       //total cycle time 12 minutes
    unsigned int m_VibrateStart=600;        //start vibration at 10 minutes
    unsigned int m_VibrateDuration=120;     //vibration duration 2 minutes
    unsigned int m_TubesStart = 60;        //start tubes after 2 minutes
    unsigned int m_TubesDuration = 660;     //tubes duration 10 minutes
    unsigned int m_01mluxStart = 0;       //always on when other light levels are not - TODO
    unsigned int m_01mLuxDuration = 0;      //TODO
    unsigned int m_5mluxStart = 180;        //5 mlux at 3 minutes
    unsigned int m_5mLuxDuration = 5;       // for 5 seconds
    unsigned int m_50luxStart = 540;        // 50 lux at 9 minutes
    unsigned int m_50LuxDuration = 3;       // for 3 seconds

    //uart *m_uart;
    //Commands *m_command;

private slots:
    void tmrCycleTimeout();
    void tmrTubesTimeout();
    void tmrVibrateTimeout();

public:
    explicit cycle(QObject *parent = nullptr);
    void start();

signals:

public slots:
};

#endif // CYCLE_H

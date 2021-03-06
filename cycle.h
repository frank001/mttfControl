#ifndef CYCLE_H
#define CYCLE_H

#include <QObject>
#include <QTimer>
#include "commands.h"

class Cycle : public QObject
{
    Q_OBJECT
private:

    QTimer m_TmrVibrate;
    QTimer m_TmrTubes;
    QTimer m_Tmr01mlux;
    QTimer m_TmrLightLevel2;
    QTimer m_TmrLightLevel3;
    QTimer m_TmrCycle;

    bool m_bCycle=false;
    bool m_bVibrate=false;
    bool m_bTubes=false;
    bool m_b01mlux=false;
    bool m_bLightLevel2=false;
    bool m_bLightLevel3=false;

    unsigned int m_Speed = 1;           //debugging purpose, speed up/slow down cycle

    unsigned int m_CycleStart=0;
    unsigned int m_CycleDuration=720;       //total cycle time 12 minutes

    unsigned int m_TubesStart = 60;        //start tubes after 1 minutes
    unsigned int m_TubesDuration = 660;     //tubes duration 11 minutes

    unsigned int m_VibrateStart=600;        //start vibration at 10 minutes
    unsigned int m_VibrateDuration=120;     //vibration duration 2 minutes

    unsigned int m_LightLevel1Start = 0;       //always on when other light levels are not - TODO
    unsigned int m_LightLevel1Duration = 0;      //TODO

    unsigned int m_LightLevel2Start = 180;        //5 mlux at 3 minutes
    unsigned int m_LightLevel2Duration = 5;       // for 5 seconds

    unsigned int m_LightLevel3Start = 540;        // 50 lux at 9 minutes
    unsigned int m_LightLevel3Duration = 3;       // for 3 seconds

    //uart *m_uart;
    Commands *m_Command;



private slots:
    void tmrCycleTimeout();
    void tmrTubesTimeout();
    void tmrVibrateTimeout();
    void tmr5mluxTimeout();
    void tmr50luxTimeout();

public:
    explicit Cycle(QObject *parent = nullptr);
    unsigned int m_CycleCount=0;
    void start();
    void stop();

signals:
    void message(int level, QString msg);
    void CycleIncrement();

public slots:
};

#endif // CYCLE_H

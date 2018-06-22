#include "cycle.h"
#include <QTimer>
#include <QThread>
#include "main.h"
#include "handler.h"
#include "commands.h"

//#include "uart.h"

Cycle::Cycle(QObject *parent) :
    QObject(parent)
{
    connect(&m_TmrCycle, &QTimer::timeout, this, &Cycle::tmrCycleTimeout);
    connect(&m_TmrTubes, &QTimer::timeout, this, &Cycle::tmrTubesTimeout);
    connect(&m_TmrVibrate, &QTimer::timeout, this, &Cycle::tmrVibrateTimeout);
    connect(&m_Tmr5mlux, &QTimer::timeout, this, &Cycle::tmr5mluxTimeout);
    connect(&m_Tmr50lux, &QTimer::timeout, this, &Cycle::tmr50luxTimeout);
    connect(this, &Cycle::message, (Handler*)parent , &Handler::message);
    connect(this, &Cycle::CycleIncrement, (Handler*)parent, &Handler::setHandlerCycleIncrement);

    m_Command = ((Handler*)parent)->m_Command;

    m_TmrCycle.setSingleShot(true);
    m_TmrTubes.setSingleShot(true);
    m_TmrVibrate.setSingleShot(true);
    m_Tmr5mlux.setSingleShot(true);
    m_Tmr50lux.setSingleShot(true);
}

void Cycle::start() {
    message(COMMAND|WATCH, "Cycle start.");




    m_TmrVibrate.start((m_VibrateStart-1*m_Speed)*1000/m_Speed);    //TODO: get rid of the minus 1 second.
    m_Tmr5mlux.start((m_5mluxStart-1*m_Speed)*1000/m_Speed);
    m_Tmr50lux.start((m_50luxStart-1*m_Speed)*1000/m_Speed);

    m_TmrTubes.start((m_TubesStart-1*m_Speed)*1000/m_Speed);

    m_bCycle = true;

    m_TmrCycle.start(m_CycleDuration*1000/m_Speed);

    CycleIncrement();

    message(COMMAND|WATCH, "1mlux on.");        //TODO: make this general and just start the timer as the others
    m_Command->HandleRaw("setLight","1");       //TODO: check current light level and only set it when different. Done, see Handler::jdUpdate
}
void Cycle::stop() {
    m_bCycle=false;
    if (m_TmrCycle.isActive()) m_TmrCycle.stop();
    if (m_TmrVibrate.isActive()) m_TmrVibrate.stop();
    if (m_Tmr5mlux.isActive()) m_Tmr5mlux.stop();
    if (m_Tmr50lux.isActive()) m_Tmr50lux.stop();
    if (m_TmrTubes.isActive()) m_TmrTubes.stop();
    m_Command->HandleRaw("setLight","0");       //TODO: see above and sync with current machine state and only emit when cycle restarts
}

void Cycle::tmrCycleTimeout(){  //switch all off
    message(COMMAND|WATCH, "Cycle timeout.");
    m_bCycle = false;
    start();

}
//void Cycle::handleTimeout(bool *b, )

void Cycle::tmrTubesTimeout(){
    message(COMMAND|WATCH, "tube timeout.");
    //if (!m_bCycle) return;
    if (!m_bTubes) {
        message(COMMAND|WATCH, "Tubes on.");
        m_Command->HandleRaw("setTubes","1");
        m_bTubes = true;
        m_TmrTubes.start(((m_TubesDuration*1000)/m_Speed)-100);
    } else {
        message(COMMAND|WATCH, "Tubes off.");
        m_Command->HandleRaw("setTubes","0");
        //switch tubes off;
        m_bTubes = false;
    }
}
void Cycle::tmrVibrateTimeout(){
    message(COMMAND|WATCH, "Vibration timeout.");
    if (!m_bVibrate) {
        message(COMMAND|WATCH, "Vibration on.");
        m_Command->HandleRaw("setVibrate","1");
        m_bVibrate = true;
        m_TmrVibrate.start(((m_VibrateDuration*1000)/m_Speed)-100);
    } else {
        message(COMMAND|WATCH, "Vibration off.");
        m_Command->HandleRaw("setVibrate","0");
        m_bVibrate = false;
        m_TmrVibrate.stop();
    }
}
void Cycle::tmr5mluxTimeout() {
    message(COMMAND|WATCH, "5mlux timeout.");
    if (!m_b5mlux) {
        message(COMMAND|WATCH, "5mlux on.");
        m_Command->HandleRaw("setLight","2");
        m_b5mlux = true;
        m_Tmr5mlux.start(m_5mLuxDuration*1000/m_Speed);
    } else {
        message(COMMAND|WATCH, "1mlux on.");
        m_Command->HandleRaw("setLight","1");
        m_b5mlux = false;
        m_Tmr5mlux.stop();
    }
}
void Cycle::tmr50luxTimeout() {
    message(COMMAND|WATCH, "50lux timeout.");
    if (!m_b50lux) {
        message(COMMAND|WATCH, "50lux on.");
        m_Command->HandleRaw("setLight","3");
        m_b50lux = true;
        m_Tmr50lux.start(m_50LuxDuration*1000/m_Speed);
    } else {
        message(COMMAND|WATCH, "1mlux on.");
        m_Command->HandleRaw("setLight","1");
        m_b50lux = false;
        m_Tmr50lux.stop();
    }
}

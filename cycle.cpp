#include "cycle.h"
#include <QTimer>
#include <QThread>
#include "main.h"
#include "handler.h"
#include "commands.h"

Cycle::Cycle(QObject *parent) :
    QObject(parent)
{
    connect(&m_TmrCycle, &QTimer::timeout, this, &Cycle::tmrCycleTimeout);
    connect(&m_TmrTubes, &QTimer::timeout, this, &Cycle::tmrTubesTimeout);
    connect(&m_TmrVibrate, &QTimer::timeout, this, &Cycle::tmrVibrateTimeout);
    connect(&m_TmrLightLevel2, &QTimer::timeout, this, &Cycle::tmr5mluxTimeout);
    connect(&m_TmrLightLevel3, &QTimer::timeout, this, &Cycle::tmr50luxTimeout);
    connect(this, &Cycle::message, (Handler*)parent , &Handler::message);
    connect(this, &Cycle::CycleIncrement, (Handler*)parent, &Handler::incrementHandlerCycle);

    m_Command = ((Handler*)parent)->m_Command;

    m_TmrCycle.setSingleShot(true);
    m_TmrTubes.setSingleShot(true);
    m_TmrVibrate.setSingleShot(true);
    m_TmrLightLevel2.setSingleShot(true);
    m_TmrLightLevel3.setSingleShot(true);
}

void Cycle::start() {
    message(COMMAND|WATCH, "Cycle start.");

    m_TmrVibrate.start((m_VibrateStart-1*m_Speed)*1000/m_Speed);    //TODO: get rid of the minus 1 second.
    m_TmrLightLevel2.start((m_LightLevel2Start-1*m_Speed)*1000/m_Speed);
    m_TmrLightLevel3.start((m_LightLevel3Start-1*m_Speed)*1000/m_Speed);

    m_TmrTubes.start((m_TubesStart-1*m_Speed)*1000/m_Speed);

    m_bCycle = true;

    m_TmrCycle.start(m_CycleDuration*1000/m_Speed);



    message(COMMAND|WATCH, "Light level 1 on.");        //TODO: make this general and just start the timer as the others
    m_Command->HandleRaw("setLight","1");       //TODO: check current light level and only set it when different. Done, see Handler::jdUpdate

}
void Cycle::stop() {
    m_bCycle=false;
    m_bTubes = false;
    m_bVibrate = false;
    m_bLightLevel2 = false;
    m_bLightLevel3 = false;

    if (m_TmrCycle.isActive()) m_TmrCycle.stop();
    if (m_TmrVibrate.isActive()) m_TmrVibrate.stop();
    if (m_TmrLightLevel2.isActive()) m_TmrLightLevel2.stop();
    if (m_TmrLightLevel3.isActive()) m_TmrLightLevel3.stop();
    if (m_TmrTubes.isActive()) m_TmrTubes.stop();
    m_Command->HandleRaw("setLight","0");       //TODO: see above and sync with current machine state and only emit when cycle restarts

}

void Cycle::tmrCycleTimeout(){  //switch all off
    message(COMMAND|WATCH, "Cycle timeout.");
    m_bCycle = false;
    CycleIncrement();
    start();

}

void Cycle::tmrTubesTimeout(){
    message(COMMAND|WATCH, "tube timeout.");
    if (!m_bTubes) {
        message(COMMAND|WATCH, "Tubes on.");
        m_Command->HandleRaw("setTubes","1");
        m_bTubes = true;
        m_TmrTubes.start(((m_TubesDuration*1000)/m_Speed)-100);
    } else {
        message(COMMAND|WATCH, "Tubes off.");
        m_Command->HandleRaw("setTubes","0");
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
    message(COMMAND|WATCH, "LightLevel 2 timeout.");
    if (!m_bLightLevel2) {
        message(COMMAND|WATCH, "LightLevel 2 on.");
        m_Command->HandleRaw("setLight","2");
        m_bLightLevel2 = true;
        m_TmrLightLevel2.start(m_5mLuxDuration*1000/m_Speed);
    } else {
        message(COMMAND|WATCH, "LightLevel 1 on.");
        m_Command->HandleRaw("setLight","1");
        m_bLightLevel2 = false;
        m_TmrLightLevel2.stop();
    }
}
void Cycle::tmr50luxTimeout() {
    message(COMMAND|WATCH, "LightLevel 3 timeout.");
    if (!m_bLightLevel3) {
        message(COMMAND|WATCH, "LightLevel 3 on.");
        m_Command->HandleRaw("setLight","3");
        m_bLightLevel3 = true;
        m_TmrLightLevel3.start(m_50LuxDuration*1000/m_Speed);
    } else {
        message(COMMAND|WATCH, "LightLevel 1 on.");
        m_Command->HandleRaw("setLight","1");
        m_bLightLevel3 = false;
        m_TmrLightLevel3.stop();
    }
}

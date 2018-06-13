#include "cycle.h"
#include <QTimer>
//#include "uart.h"

cycle::cycle(QObject *parent) :
    QObject(parent)
{
    connect(&m_TmrCycle, &QTimer::timeout, this, &cycle::tmrCycleTimeout);
    connect(&m_TmrTubes, &QTimer::timeout, this, &cycle::tmrTubesTimeout);
    connect(&m_TmrVibrate, &QTimer::timeout, this, &cycle::tmrVibrateTimeout);

    //m_command = new Commands()

    m_TmrCycle.setSingleShot(true);
    m_TmrTubes.setSingleShot(true);

}

void cycle::start() {
    m_TmrCycle.start(m_CycleDuration);
    m_TmrTubes.start(m_TubesStart);

    m_bCycle = true;
}

void cycle::tmrCycleTimeout(){  //switch all off
    m_bCycle = false;
}
void cycle::tmrTubesTimeout(){
    if (!m_bCycle) return;
    if (!m_bTubes) {
        //switch tubes on
        m_bTubes = true;
        m_TmrTubes.start(m_TubesDuration);
    } else {
        //switch tubes off;
        m_bTubes = false;


    }
}
void cycle::tmrVibrateTimeout(){

}

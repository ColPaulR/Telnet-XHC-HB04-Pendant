#include <thread>
#include <mutex>
#include "GrblStatus.h"

GrblStatus::GrblStatus()
{
    // Initialize variables
    SetState(State::Undefined);

    // isMpos; don't care
    // int nAxis; don't know
    // double axis_Position[MAX_N_AXIS]; don't know
    // double axis_Probe[MAX_N_AXIS];  don't know
    // ignore axis_WCO for now as it is not currently used
    // double    axis_WCO[MAX_N_AXIS];  don't know
    //   bool isG91;
    //   bool isG21;
    //   uint32_t feedrate;
    //   int spindle;
    //   uint32_t spindle_speed;
    //   bool flood;
    //   bool mist;
    //   bool ProbeSuccessFlag;
    // NewProbeFlag = 0;
}

void GrblStatus::SetState(State NewState)
{
    mtx.lock();
    myState=NewState;
    mtx.unlock();
}

bool GrblStatus::GetIsMpos ()
{
    bool myReturn;

    mtx.lock();
    myReturn = isMpos;
    mtx.unlock();
    return myReturn;
}

void GrblStatus::SetIsMpos (bool myIsMpos)
{
    mtx.lock();
    isMpos = myIsMpos;
    mtx.unlock();
}

void GrblStatus::ShowDro (const pos_t *new_axes, const pos_t *new_wcos, bool new_isMpos, size_t new_n_axis){
    mtx.lock();
    isMpos = isMpos = new_isMpos;
    nAxis = new_n_axis;
    for (int i = 0; i < new_n_axis; i++)
    {
        axis_Position[i] = new_axes[i];
        axis_WCO[i] = new_wcos[i];
    } 
    mtx.unlock();
}

void GrblStatus::SpindleFeed (uint32_t new_feedrate, uint32_t new_spindle_speed)
{
    mtx.lock();
    feedrate = new_feedrate;
    spindle_speed = new_spindle_speed;
    mtx.unlock();
} 

void GrblStatus::SpindleCoolant(int new_spindle, bool new_flood, bool new_mist)
{
    mtx.lock();
    spindle = new_spindle;
    flood = new_flood;
    mist = new_mist;
    mtx.unlock();
} 
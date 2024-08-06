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
    reurn myReturn;
}

void GrblStatus::SetIsMpos (bool myIsMpos)
{
    mtx.lock();
    isMpos = myIsMpos;
    mtx.unlock();
}
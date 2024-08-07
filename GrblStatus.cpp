#include "GrblStatus.h"
#include "SerialDebug.h"

inline void GrblStatus::mylock()
{
    //    mtx.lock();
    // Use spinlock vs. mutex to be more efficient
     while (lock.test_and_set(std::memory_order_acquire)) // acquire lock
        {
            // Since C++20, it is possible to update atomic_flag's
            // value only when there is a chance to acquire the lock.
            // See also: https://stackoverflow.com/questions/62318642
        #if defined(__cpp_lib_atomic_flag_test)
            while (lock.test(std::memory_order_relaxed)) // test lock
        #endif
                ; // spin
        }
}

inline void GrblStatus::myunlock()
{
    //    mtx.unlock();
    // Use spinlock vs. mutex to be more efficient
    lock.clear(std::memory_order_release); // release lock

}

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
    mylock();
    myState=NewState;
    myunlock();
}

bool GrblStatus::GetIsMpos ()
{
    bool myReturn;

    mylock();
    myReturn = isMpos;
    myunlock();
    return myReturn;
}

void GrblStatus::SetIsMpos (bool myIsMpos)
{
    mylock();
    isMpos = myIsMpos;
    myunlock();
}

void GrblStatus::SetDro (const pos_t *new_axes, const pos_t *new_wcos, bool new_isMpos, size_t new_n_axis){
    mylock();
    isMpos = isMpos = new_isMpos;
    nAxis = new_n_axis;
    for (int i = 0; i < new_n_axis; i++)
    {
        axis_Position[i] = new_axes[i];
        axis_WCO[i] = new_wcos[i];
    } 
    myunlock();
}

void GrblStatus::SetSpindleFeed (uint32_t new_feedrate, uint32_t new_spindle_speed)
{
    mylock();
    feedrate = new_feedrate;
    spindle_speed = new_spindle_speed;
    myunlock();
} 

void GrblStatus::SetSpindleCoolant(int new_spindle, bool new_flood, bool new_mist)
{
    mylock();
    spindle = new_spindle;
    flood = new_flood;
    mist = new_mist;
    myunlock();
} 

void GrblStatus::SetProbe(const pos_t *new_axes, const bool probe_success, size_t n_axis)
{
    mylock();
    for (int i = 0; i < n_axis; i++)
    {
        axis_Probe[i] = new_axes[i];
    }

    // Process success flag
    ProbeSuccessFlag = probe_success;
    myunlock();

    // NEED LOGIC HERE TO HANDLE STATE TRANSITIONS
}

void GrblStatus::SetGcodeModes(int new_spindle, bool new_mist, bool new_flood, bool new_isG21, bool new_isG91)
{
    mylock();
    spindle = new_spindle;
    mist = new_mist;
    flood = new_flood;
    isG21 = new_isG21;
    isG91 = new_isG91;
    myunlock();
}


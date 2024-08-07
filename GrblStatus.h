#pragma once
#ifndef GRLBSTASTUS_H
#define GRLBSTASTUS_H

#include <thread>
#include <mutex>
#include <atomic>

#include "GrblParser\GrblParserC.h"


// From FluidNC/FluidNC/src/Types.h with "enum class" changed to "enum"
// From FluidNC/FluidNC/src/Report.cpp translates enum to string
//
// System states. The state variable primarily tracks the individual functions
// to manage each without overlapping. It is also used as a messaging flag for
// critical events.
enum class State // : uint8_t
{
  Idle = 0,    // Must be zero.
  Alarm,       // In alarm state. Locks out all g-code processes. Allows settings access.
  CheckMode,   // G-code check mode. Locks out planner and motion only.
  Homing,      // Performing homing cycle
  Cycle,       // Cycle is running or motions are being executed.
  Hold,        // Active feed hold
  Jog,         // Jogging mode.
  SafetyDoor,  // Safety door is ajar. Feed holds and de-energizes system.
  Sleep,       // Sleep state.
  ConfigAlarm, // You can't do anything but fix your config file.
  Critical,    // You can't do anything but reset with CTRL-x or the reset button
  Undefined
};

class GrblStatus {
  private:
    State myState;
    int isMpos;
    int nAxis;
    double axis_Position[MAX_N_AXIS];
    double axis_Probe[MAX_N_AXIS];
    // ignore axis_WCO for now as it is not currently used
    double    axis_WCO[MAX_N_AXIS];
    bool isG91;
    bool isG21;
    uint32_t feedrate;
    int spindle;
    uint32_t spindle_speed;
    bool flood;
    bool mist;
    bool NewProbeFlag = 0;
    bool ProbeSuccessFlag;

    std::atomic_flag lock = ATOMIC_FLAG_INIT;
    std::mutex mtx;

    inline void GrblStatus::mylock();
    inline void GrblStatus::myunlock();

    public:
         GrblStatus();
         void SetState(State NewState);
         bool GetIsMpos();
         void SetIsMpos (bool myIsMpos);
         void ShowDro (const pos_t *new_axes, const pos_t *new_wcos, bool new_isMpos, size_t new_n_axis);
         void SpindleFeed (uint32_t new_feedrate, uint32_t new_spindle_speed);
         void SpindleCoolant(int spindle, bool flood, bool mist);
         void ShowProbe(const pos_t *new_axes, const bool new_probe_success, size_t new_n_axis);
         void ShowGcodeModes(int spindle, bool mist, bool flood, bool isG21, bool isG91);

};

#endif

#ifndef GRBLCODE_H
#define GRBLCODE_H

#include "GrblParserC.h"

#define MAX_STATUS_LEN 10

// use the Z axis, 3rd (index = 2) coordinate for probing
#define PROBE_AXIS 2
const char GRBLAxisLetters[] = {'X', 'Y', 'Z', 'A', 'B', 'C'};
#define PROBE_AXIS_CHAR GRBLAxisLetters[PROBE_AXIS]
#define CMD_DELAY 1000
const char CMD_SAFE_Z[] = "G53G0Z0";
const char CMD_MOVE_M_COORD[] = "G53G0";
const char CMD_GOTO_PROBE_XY[] = "G30G91X0Y0";
const char CMD_FAST_PROBE[] = "G53G38.2Z-200F300";
const char CMD_PROBE_LIFT[] = "$J=G91Z2F6000";
const char CMD_SLOW_PROBE[] = "G53G38.2Z-200F75";
const char CMD_RESET_Z[] = "G10L20P0Z";


extern bool is_report_type(char* , char** , const char* , const char*);
extern size_t parse_axes(char*, pos_t*);

void PrintGrblStatusMsg(struct GRBLSTATUS *GrblStatus);

// struct GRBLSTATUS
// {
//   uint8_t State;
//   int isMpos;
//   int nAxis;
//   double axis_Position[MAX_N_AXIS];
//   double axis_Probe[MAX_N_AXIS];
//   // ignore axis_WCO for now as it is not currently used
//   double    axis_WCO[MAX_N_AXIS];
//   bool isG91;
//   bool isG21;
//   uint32_t feedrate;
//   int spindle;
//   uint32_t spindle_speed;
//   bool flood;
//   bool mist;
//   bool ProbeSuccessFlag;
//   bool NewProbeFlag = 0;
// };

// // From FluidNC/FluidNC/src/Types.h with "enum class" changed to "enum"
// // From FluidNC/FluidNC/src/Report.cpp translates enum to string
// //
// // System states. The state variable primarily tracks the individual functions
// // to manage each without overlapping. It is also used as a messaging flag for
// // critical events.
// enum State : uint8_t
// {
//   Idle = 0,    // Must be zero.
//   Alarm,       // In alarm state. Locks out all g-code processes. Allows settings access.
//   CheckMode,   // G-code check mode. Locks out planner and motion only.
//   Homing,      // Performing homing cycle
//   Cycle,       // Cycle is running or motions are being executed.
//   Hold,        // Active feed hold
//   Jog,         // Jogging mode.
//   SafetyDoor,  // Safety door is ajar. Feed holds and de-energizes system.
//   Sleep,       // Sleep state.
//   ConfigAlarm, // You can't do anything but fix your config file.
//   Critical,    // You can't do anything but reset with CTRL-x or the reset button
//   Undefined
// };

#endif

// #include <Arduino.h>
#include <string.h>
// #include "GrblCode.h"
#include "..\Telnet Pendant.h"
#include "..\GrblStatus.h"

// Data parsed from <...> status reports
// void  show_limits(bool probe, const bool* limits, size_t n_axis) {};
void show_state(const char *state)
{

    // From FluidNC/FluidNC/src/Report.cpp, possible string states are"
    //    "Idle", "Run" , "Jog", "Home", " Alarm", "Check", "Door:x", "Sleep"

    // Parse GRBL state; set to undefined by default
    State myState = State::Undefined;

    if (strncmp(state, "Run", 3) == 0)
    {
        myState = State::Cycle;
    }

    if (strncmp(state, "Idle", 4) == 0)
    {
        myState = State::Idle;
    }
    if (strncmp(state, "Hold", 4) == 0)
    {
        myState = State::Hold;
    }
    if (strncmp(state, "Jog", 3) == 0)
    {
        myState = State::Jog;
    }

    // Record state to status
    myGrblStatus.SetState(myState);

#if SERIALDEBUG > 0
    // State state not handled above
    Serial.print("Grbl State ");
    Serial.print(state);
    Serial.println(" not handled");
#endif
};

void show_dro(const pos_t *axes, const pos_t *wcos, bool isMpos, bool *limits, size_t n_axis)
{
    myGrblStatus.ShowDro (axes, wcos, isMpos, n_axis);
}

void show_feed_spindle(uint32_t feedrate, uint32_t spindle_speed)
{
    myGrblStatus.SpindleFeed (feedrate, spindle_speed);
}

void show_spindle_coolant(int spindle, bool flood, bool mist)
{
    myGrblStatus.SpindleCoolant(spindle, flood, mist);
}

void show_probe(const pos_t *axes, const bool probe_success, size_t n_axis)
{
    myGrblStatus.ShowProbe(axes, probe_success, n_axis);
}

void end_status_report()
{
    // Send a newly allocated structure that is initialize with current status
    //rp2040.fifo.push_nb((uint32_t) new GRBLSTATUS(GrblStatus));
}

// [GC: messages
void show_gcode_modes(struct gcode_modes *modes)
{
    int spindle;
    bool mist, flood, isG21, isG91;
    
    // Spindle values
    if (!strcmp((modes->spindle), "Off"))
        spindle = 0;
    if (!strcmp((modes->spindle), "CW"))
        spindle = 1;
    if (!strcmp((modes->spindle), "CCW"))
        spindle = 2;

    mist=(!strcmp(modes->mist, "On"));
    flood=(!strcmp(modes->flood, "On"));

    // Inches or mm?
    if (!strcmp(modes->units, "In"))
    {
        isG21 = false;
    }
    if (!strcmp(modes->units, "mm"))
    {
        isG21 = true;
    }

    // Relative or absolute
    if (!strcmp(modes->distance, "Rel"))
    {
        isG91 = false;
    }
    if (!strcmp(modes->distance, "Abs"))
    {
        isG91 = true;
    }

    myGrblStatus.ShowGcodeModes(spindle, mist, flood, isG21, isG91);

    // Send a newly allocated structure that is initialize with current status
    //rp2040.fifo.push_nb((uint32_t) new GRBLSTATUS(GrblStatus));
}
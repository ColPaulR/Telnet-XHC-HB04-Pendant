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
    // update GrblStatus with new modes

    // Spindle values
    if (!strcmp((modes->spindle), "Off"))
        GrblStatus.spindle = 0;
    if (!strcmp((modes->spindle), "CW"))
        GrblStatus.spindle = 1;
    if (!strcmp((modes->spindle), "CCW"))
        GrblStatus.spindle = 2;

    GrblStatus.mist=(!strcmp(modes->mist, "On"));
    GrblStatus.flood=(!strcmp(modes->flood, "On"));

    // Inches or mm?
    if (!strcmp(modes->units, "In"))
    {
        GrblStatus.isG21 = false;
    }
    if (!strcmp(modes->units, "mm"))
    {
        GrblStatus.isG21 = true;
    }

    // Relative or absolute
    if (!strcmp(modes->distance, "Rel"))
    {
        GrblStatus.isG91 = false;
    }
    if (!strcmp(modes->distance, "Abs"))
    {
        GrblStatus.isG91 = true;
    }

    // Send a newly allocated structure that is initialize with current status
    //rp2040.fifo.push_nb((uint32_t) new GRBLSTATUS(GrblStatus));
}

// void  handle_other(char *field)
// // static void parse_probe_report(char *field)
// {
//     // The report wrapper, already removed, is [PRB:...]
//     // The body for [PRB:1095.000,105.000,-49.880,0.000:1] is, for example,
//     //   1095.000,105.000,-49.880,0.000:1

//     char *next;

//     size_t n_axis = 0;
//     pos_t axes[MAX_N_AXIS];

//     // Separate position from success
//     if (!split(field, &next, '|'))
//         // Return if split fails
//         return;

//     // Process positon
//     n_axis = parse_axes(field, axes);

//  for (int i = 0; i < n_axis; i++)
//     {
//         GrblStatus.axis_Probe[i] = axes[i];
//     }

//     // Process success flag
//     GrblStatus.ProbeFlag = false;
//     if (*next == '1')
//         GrblStatus.ProbeFlag = true;

//     // Send a newly allocated structure that is initialize with current status
//     rp2040.fifo.push_nb((uint32_t) new GRBLSTATUS(GrblStatus));
// }

// Telnet Pendant.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <mutex>


// TODO: Reference additional headers your program requires here.
// Create static structure to store current state
extern GRBLSTATUS GrblStatus;
extern std::mutex mGrblStatus;
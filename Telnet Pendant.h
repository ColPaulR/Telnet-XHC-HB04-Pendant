// Telnet Pendant.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <mutex>
#include "GrblStatus.h"

using namespace std;

// TODO: Reference additional headers your program requires here.
// Create static structure to store current state
extern GrblStatus myGrblStatus;
extern bool bQuit;

#define LOOPSLEEP 50
#define REPORTINGINTERVAL 250
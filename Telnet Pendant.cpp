﻿// Telnet Pendant.cpp : Defines the entry point for the application.
//

#include <thread>
#include "GrblParser\GrblCode.h"
#include "Telnet Pendant.h"
#include "Telnet.h"
#include "GrblStatus.h"

using namespace std;

// Create class to store and provide current state
GrblStatus myGrblStatus;
bool bQuit;

// Class for communicating with FluidNC
MyTelnet myTelnet;

void TelnetThread()
{
  char recvbuf[DEFAULT_BUFLEN+1];
  int iResult;
  int recvbuflen = DEFAULT_BUFLEN;

  do {
    iResult = myTelnet.Read(recvbuf,recvbuflen);

    if (iResult > 0) {
      // Send 1 character at a time to GRBLParser.
      for (int iLooper = 0; iLooper < iResult; iLooper++) {
        collect(recvbuf[iLooper]);
      }
      #if (GRBL_STATUS_ECHO)
        // Ensure buffer is null terminated to handle as string
        recvbuf[iResult] = 0;
        puts(recvbuf);
      #endif
    } else if (iResult == 0)
      printf("Connection closed\n");
    else
      printf("recv failed with error: %d\n", WSAGetLastError());
  } while (!bQuit);
  return;
}


int main(int argc, char **argv) {
  //char initbuf[] = "/n$RI="+"/n";
  int iResult;

  // Set flag to continue looping
  bQuit = false;

  // Initialize Winsock
  iResult = myTelnet.Initialize();
  if (iResult != 0) {
    printf("WSAStartup failed with error: %d\n", iResult);
    return false;
  }

  // Connect to server
  myTelnet.Connect(DEFAULT_SERVER, DEFAULT_PORT);

  iResult = myTelnet.Send("/n" REPORTINGINTERVALCMD);

  printf("Bytes Sent: %ld\n", iResult);

  // Spawn Telnet task thread
  thread taskTelnet(TelnetThread);

  // Test if taskTelent thread is still running
  // if() taskTelnet.joinable())
  while (!bQuit)
  {
    // Loop

    // Sleep
    Sleep(LOOPSLEEP);
  }
  
  // wait for thread to finish
  taskTelnet.join();
  
  return 0;
}

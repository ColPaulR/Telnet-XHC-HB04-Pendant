// Telnet Pendant.cpp : Defines the entry point for the application.
//


#include "GrblParser\GrblCode.h"
#include "Telnet Pendant.h"
#include "Telnet.h"
#include "GrblStatus.h"

using namespace std;



// Create static structure to store current state
GrblStatus myGrblStatus;

int main(int argc, char **argv) {
  char sendbuf[] = "/n?/n?/n";
  int iResult;
  MyTelnet myTelnet;

  // Initialize Winsock
  iResult =  MyTelnet.Initialize();
  if (iResult != 0) {
    printf("WSAStartup failed with error: %d\n", iResult);
    return false;
  }

  // Connect to server
  myTelnet.TelnetConnect(DEFAULT_SERVER, DEFAULT_PORT);

  iResult = myTelnet.TelnetSend(sendbuf);

  printf("Bytes Sent: %ld\n", iResult);

  // Do main loop
  
  do {
    myTelnet.TelnetTask();
  } while (1);

  return 0;
}

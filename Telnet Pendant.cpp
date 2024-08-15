// Telnet Pendant.cpp : Defines the entry point for the application.
//

#include <thread>
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
  iResult = myTelnet.Initialize();
  if (iResult != 0) {
    printf("WSAStartup failed with error: %d\n", iResult);
    return false;
  }

  // Connect to server
  myTelnet.Connect(DEFAULT_SERVER, DEFAULT_PORT);

  iResult = myTelnet.Send(sendbuf);

  printf("Bytes Sent: %ld\n", iResult);

  // Spawn Telnet task thread
  thread taskTelnet(myTelnet.Task);

  // wait for thread to finish
  taskTelnet.join();


  return 0;
}

// Telnet Pendant.cpp : Defines the entry point for the application.
//
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "GrblParser\GrblCode.h"
#include "Telnet Pendant.h"
#include "Telnet.h"
#include "GrblStatus.h"

using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define DEFAULT_PORT "23"
#define DEFAULT_SERVER "192.168.0.48"

// Create static structure to store current state
GrblStatus myGrblStatus;

int main(int argc, char **argv) {
  WSADATA wsaData;
  // SOCKET ConnectSocket = INVALID_SOCKET;
  char sendbuf[] = "/n?/n?/n";
  int iResult;
  MyTelnet myTelnet;

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed with error: %d\n", iResult);
    return 1;
  }

  // Initial here
  myTelnet.TelnetConnect(DEFAULT_SERVER, DEFAULT_PORT);

  iResult = myTelnet.TelnetSend(sendbuf);

  printf("Bytes Sent: %ld\n", iResult);

  // Do main loop
  
  do {
    myTelnet.TelnetTask();
  } while (1);

  // cleanup
  // closesocket(myTelnet.MySocket);
  WSACleanup();

  return 0;
}

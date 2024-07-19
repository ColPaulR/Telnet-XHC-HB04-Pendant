﻿// Telnet Pendant.cpp : Defines the entry point for the application.
//
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Telnet Pendant.h"
#include "Telnet.h"

using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "23"
#define DEFAULT_SERVER "192.168.0.48"

int main(int argc, char **argv) {
  WSADATA wsaData;
  SOCKET ConnectSocket = INVALID_SOCKET;
  char sendbuf[] = "/n?/n?/n";
  char recvbuf[DEFAULT_BUFLEN];
  int iResult;
  int recvbuflen = DEFAULT_BUFLEN;

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed with error: %d\n", iResult);
    return 1;
  }

  // Initial here
  ConnectSocket = TelnetConnect(DEFAULT_SERVER, DEFAULT_PORT);

  iResult = TelnetSend(ConnectSocket, sendbuf);

  printf("Bytes Sent: %ld\n", iResult);

  // Do main loop
  
  do {
    TelnetTask(ConnectSocket);
  } while (1);

  // cleanup
  closesocket(ConnectSocket);
  WSACleanup();

  return 0;
}

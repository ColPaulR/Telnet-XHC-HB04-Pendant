﻿// Telnet.cpp
//
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "GrblParser/GrblParserC.h"
#include "SerialDebug.h"
#include "Telnet.h"

bool MyTelnet::Connect(const char *szServer, const char *szPort) 
{
  struct addrinfo *result = NULL, *ptr = NULL, hints;
  int iResult;
  // SOCKET MySocket;

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  iResult = getaddrinfo(szServer, szPort, &hints, &result);
  if (iResult != 0) {
    printf("getaddrinfo failed with error: %d\n", iResult);
    WSACleanup();
    return false;
  }

  // Attempt to connect to an address until one succeeds
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    // Create a SOCKET for connecting to server
    MySocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (MySocket == INVALID_SOCKET) {
      printf("socket failed with error: %ld\n", WSAGetLastError());
      WSACleanup();
      return 1;
    }

    // Connect to server.
    iResult = connect(MySocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      closesocket(MySocket);
      MySocket = INVALID_SOCKET;
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  if (MySocket == INVALID_SOCKET) {
    printf("Unable to connect to server!\n");
    WSACleanup();
    exit(1);
  }

  return MySocket;
}

int MyTelnet::Read(char *szRead, int BuffLen)
{
  return recv(MySocket, szRead, BuffLen, 0);
}

int MyTelnet::Send(char *szSend) {
  int iResult;
  int iResult2;

  // Send an initial buffer
  iResult = send(MySocket, szSend, (int)strlen(szSend), 0);
  if (iResult == SOCKET_ERROR) {
    iResult2 = WSAGetLastError();
    switch (iResult2) {
    case WSAECONNRESET:
      // Need to reconnect
      printf("send failed with error: %d\n", iResult2);
      break;
    default:
      // Unhandled error. Print and quit
      printf("send failed with error: %d\n", iResult2);
      closesocket(MySocket);
      WSACleanup();
      exit(1);
      break;
    }
  }
  return (iResult);
}

// void MyTelnet::Task() {
//   char recvbuf[DEFAULT_BUFLEN+1];
//   int iResult;
//   int recvbuflen = DEFAULT_BUFLEN;

//   do {
//     iResult = recv(MySocket, recvbuf, recvbuflen, 0);
//     if (iResult > 0) {
//       // Send 1 character at a time to GRBLParser.
//       for (int iLooper = 0; iLooper < iResult; iLooper++) {
//         collect(recvbuf[iLooper]);
//       }
//       #if (GRBL_STATUS_ECHO)
//         // Ensure buffer is null terminated to handle as string
//         recvbuf[iResult] = 0;
//         puts(recvbuf);
//       #endif
//       // printf("Bytes received: %d\n\t%s\n", iResult, recvbuf);
//     } else if (iResult == 0)
//       printf("Connection closed\n");
//     else
//       printf("recv failed with error: %d\n", WSAGetLastError());
//   } while (1);
//   return;
// }

int MyTelnet::Initialize() {
  // Initialize Winsock
  return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

MyTelnet::~MyTelnet() {
  // cleanup
  closesocket(MySocket);
  WSACleanup();

}

  

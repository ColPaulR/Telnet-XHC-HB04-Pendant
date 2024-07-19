// Telnet Pendant.cpp : Defines the entry point for the application.
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
/*
SOCKET TelnetConnect(const char *szServer) {
  struct addrinfo *result = NULL, *ptr = NULL, hints;
  int iResult;
  SOCKET MySocket;

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  iResult = getaddrinfo(DEFAULT_SERVER, DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
    printf("getaddrinfo failed with error: %d\n", iResult);
    WSACleanup();
    return 1;
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

int TelnetSend(SOCKET ConnectSocket, char *szSend) {
  int iResult;
  int iResult2;

  // Send an initial buffer
  iResult = send(ConnectSocket, szSend, (int)strlen(szSend), 0);
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
      closesocket(ConnectSocket);
      WSACleanup();
      exit(1);
      break;
    }
  }
  return (iResult);
}
*/
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

  ConnectSocket = TelnetConnect(DEFAULT_SERVER);

  iResult = TelnetSend(ConnectSocket, sendbuf);

  printf("Bytes Sent: %ld\n", iResult);

  // shutdown the connection since no more data will be sent
  iResult = shutdown(ConnectSocket, SD_SEND);
  if (iResult == SOCKET_ERROR) {
    printf("shutdown failed with error: %d\n", WSAGetLastError());
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
  }

  // Receive until the peer closes the connection
  do {

    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
      recvbuf[iResult] = 0;
      printf("Bytes received: %d\n\t%s\n", iResult, recvbuf);
    } else if (iResult == 0)
      printf("Connection closed\n");
    else
      printf("recv failed with error: %d\n", WSAGetLastError());

  } while (iResult > 0);

  // cleanup
  closesocket(ConnectSocket);
  WSACleanup();

  return 0;
}

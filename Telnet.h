// Telnet.h
//

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "23"
#define DEFAULT_SERVER "192.168.0.48"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

class MyTelnet
{
    SOCKET MySocket = INVALID_SOCKET;
    WSADATA wsaData;
  
    public:
        bool TelnetConnect(const char *szServer, const char *szPort);
        int TelnetSend(char *szSend);
        void TelnetTask();
        int Initialize();
        ~MyTelnet();
};
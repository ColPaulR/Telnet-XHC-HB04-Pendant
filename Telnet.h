// Telnet.cpp
//
#include <winsock2.h>

SOCKET TelnetConnect(const char *szServer);
int TelnetSend(SOCKET ConnectSocket, char *szSend);

// Telnet.cpp
//
#include <winsock2.h>

extern SOCKET TelnetConnect(const char *szServer);
extern int TelnetSend(SOCKET ConnectSocket, char *szSend);

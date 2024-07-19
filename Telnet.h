// Telnet.h
//
extern SOCKET TelnetConnect(const char *szServer);
extern int TelnetSend(SOCKET ConnectSocket, char *szSend);

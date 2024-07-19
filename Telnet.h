// Telnet.h
//

extern SOCKET TelnetConnect(const char *szServer, const char *szPort);
extern int TelnetSend(SOCKET ConnectSocket, char *szSend);
extern void TelnetTask(SOCKET ConnectSocket);

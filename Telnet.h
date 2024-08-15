// Telnet.h
//

// extern SOCKET TelnetConnect(const char *szServer, const char *szPort);
// extern int TelnetSend(SOCKET ConnectSocket, char *szSend);
// extern void TelnetTask(SOCKET ConnectSocket);

#define DEFAULT_BUFLEN 512

Class MyTelnet
{
    public:
        SOCKET MySocket = INVALID_SOCKET;

        bool TelnetConnect(const char *szServer, const char *szPort);
        int TelnetSend(SOCKET ConnectSocket, char *szSend);
        void TelnetTask(SOCKET ConnectSocket);
}
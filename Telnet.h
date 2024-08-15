// Telnet.h
//

// extern SOCKET TelnetConnect(const char *szServer, const char *szPort);
// extern int TelnetSend(SOCKET ConnectSocket, char *szSend);
// extern void TelnetTask(SOCKET ConnectSocket);

#define DEFAULT_BUFLEN 512

class MyTelnet
{
    SOCKET MySocket = INVALID_SOCKET;

    public:

        bool TelnetConnect(const char *szServer, const char *szPort);
        int TelnetSend(char *szSend);
        void TelnetTask();
};
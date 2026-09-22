#ifndef UPPERTCPSERVER_H
#define UPPERTCPSERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <netinet/in.h>

class UpperTcpServer
{
public:
    UpperTcpServer(int dataPort, int ctrlPort);
    ~UpperTcpServer();

    void start();
    void stop();

private:
    int dataPort;
    int ctrlPort;
    int dataSocket;
    int ctrlSocket;
    std::vector<int> clientSockets;

    void setupSocket(int &socket, int port);
    void handleDataConnection();
    void handleCtrlConnection();
    void handleClientData(int clientSocket);
    void handleClientCtrl(int clientSocket);
};

#endif // UPPERTCPSERVER_H

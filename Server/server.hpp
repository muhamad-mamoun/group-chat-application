#pragma once

class Server
{
private:
    int fileDescriptor;
    unsigned short portNumber;
    void setPortNumber(unsigned short a_portNumber);
protected:
public:
    Server() = delete;
    Server(unsigned long a_portNumber, int a_maxAllowedClients);
    ~Server();
    void listenToNewClient(int* a_clientFileDescriptor);
    void sendMessage(int a_clientFileDescriptor, const char* a_messageContent, int a_messageLength);
    void receiveMessage(int a_clientFileDescriptor, char* a_messageBuffer, int a_bufferSize);
};

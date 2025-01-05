#pragma once

class Server
{
private:
    int fileDescriptor;
    unsigned short portNumber;
protected:
public:
    Server() = delete;
    Server(unsigned long a_portNumber);
    ~Server();
    void setPortNumber(unsigned short a_portNumber);
    void listenToClient(void);
    void sendMessage(const char* a_messageContent, int a_messageLength);
    void receiveMessage(char* a_messageBuffer);
};

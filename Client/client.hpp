#pragma once

class Client
{
private:
    char* name;
    char fileDescriptor;
    bool connectionStatus;
    unsigned int serverIPv4Address;
    unsigned short serverPortNumber;
    static bool isDigit(const char a_character);
protected:
public:
    Client() = delete;
    Client(const char* a_clientName, const char* a_serverIPv4Address, unsigned short a_serverPortNumber);
    ~Client();
    void setName(const char* a_clientName);
    void setServerIPv4Address(const char* a_serverIPv4Address);
    void setServerPortNumber(unsigned short a_serverPortNumber);
    void sendMessage(const char* a_messageContent, int a_messageLength);
    void receiveMessage(char* a_messageBuffer);
};

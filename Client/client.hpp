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
    void setServerSocketAddress(const char* a_serverSocketAddress);
    void setServerIPv4Address(const char* a_serverIPv4Address);
    void setServerPortNumber(const char* a_serverPortNumber);
    void setName(const char* a_clientName);
    void connectToServer(void);
protected:
public:
    Client() = delete;
    Client(const char* a_clientName, const char* a_serverSocketAddress);
    Client(const char* a_clientName, const char* a_serverIPv4Address, const char* a_serverPortNumber);
    ~Client();
    void sendMessage(const char* a_messageContent, int a_messageLength);
    void receiveMessage(char* a_messageBuffer);
};

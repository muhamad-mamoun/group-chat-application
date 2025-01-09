#include "client.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Client::Client(const char* a_clientName, const char* a_serverSocketAddress)
{
    this->name = nullptr;
    this->serverPortNumber = 0;
    this->serverIPv4Address = 0;
    this->setName(a_clientName);
    this->setServerSocketAddress(a_serverSocketAddress);

    this->fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(this->fileDescriptor < 0)
    {
        throw "Failed to create a socket!";
    }

    this->connectToServer();
}

Client::Client(const char* a_clientName, const char* a_serverIPv4Address, const char* a_serverPortNumber)
{
    this->name = nullptr;
    this->serverPortNumber = 0;
    this->serverIPv4Address = 0;
    this->setName(a_clientName);
    this->setServerIPv4Address(a_serverIPv4Address);
    this->setServerPortNumber(a_serverPortNumber);

    this->fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(this->fileDescriptor < 0)
    {
        throw "Failed to create a socket!";
    }

    this->connectToServer();
}

void Client::connectToServer(void)
{
    sockaddr_in* serverAddress = new sockaddr_in;
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_addr.s_addr = this->serverIPv4Address;
    serverAddress->sin_port = this->serverPortNumber;
    
    this->connectionStatus = (connect(this->fileDescriptor, (sockaddr*)serverAddress, sizeof(sockaddr_in)) == 0) ? true : false;
    if(!this->connectionStatus)
    {
        throw "Failed to connect to the server!";
    }

    delete serverAddress;
}

Client::~Client()
{
    shutdown(this->fileDescriptor, SHUT_RDWR);
    close(this->fileDescriptor);
    delete[] this->name;
}

void Client::setName(const char* a_clientName)
{
    if(this->name != nullptr) delete[] this->name;

    short clientNameLength = 0;
    while(*(a_clientName + clientNameLength) != '\0') clientNameLength++;

    clientNameLength++; /* For Null Character. */
    this->name = new char[clientNameLength];
    for(short characterIndex = 0; characterIndex < clientNameLength; characterIndex++)
    {
        *(this->name + characterIndex) = *(a_clientName + characterIndex);
    }
}

void Client::setServerSocketAddress(const char* a_serverSocketAddress)
{
    const char* serverIPv4Address = a_serverSocketAddress;
    const char* serverPortNumber = nullptr;
    int characterIndex = 0;

    for(; *(a_serverSocketAddress + characterIndex) != ':'; characterIndex++);
    *((char*)a_serverSocketAddress + characterIndex) = '\0'; /* Sorry C++ */
    serverPortNumber = (a_serverSocketAddress + characterIndex + 1);

    this->setServerIPv4Address(serverIPv4Address);
    this->setServerPortNumber(serverPortNumber);
}

void Client::setServerIPv4Address(const char* a_serverIPv4Address)
{
    unsigned char IPv4Segments[4] = {0}, segmentCounter = 0;
    char previousCharacter = '\0', currentCharacter = '\0';

    for(char characterIndex = 0; *(a_serverIPv4Address + characterIndex) != '\0'; characterIndex++)
    {
        previousCharacter = currentCharacter;
        currentCharacter = *(a_serverIPv4Address + characterIndex);

        if((currentCharacter == '.') && (previousCharacter == '.'))
        {
            throw "Invalid IPv4 address!";
        }
        
        else if((currentCharacter == '.') && (segmentCounter == 3))
        {
            throw "Invalid IPv4 address!";
        }

        else if(currentCharacter == '.')
        {
            segmentCounter++;
        }

        else if(!Client::isDigit(currentCharacter))
        {
            throw "Invalid IPv4 address!";
        }

        else if(((IPv4Segments[segmentCounter] * 10) + (currentCharacter - '0')) > 255UL)
        {
            throw "Invalid IPv4 address!";
        }

        else
        {
            IPv4Segments[segmentCounter] = ((IPv4Segments[segmentCounter] * 10) + (currentCharacter - '0'));
        }
    }

    this->serverIPv4Address = ((IPv4Segments[0] << 24) | (IPv4Segments[0] << 16) | (IPv4Segments[0] << 8) | IPv4Segments[0]);
}

void Client::setServerPortNumber(const char* a_serverPortNumber)
{
    for(char characterIndex = 0; *(a_serverPortNumber + characterIndex) != '\0'; characterIndex++)
    {
        char currentCharacter = *(a_serverPortNumber + characterIndex);

        if(!Client::isDigit(currentCharacter))
        {
            throw "Invalid port number!";
        }

        else if(((this->serverPortNumber * 10) + (currentCharacter - '0')) > 65535UL)
        {
            throw "Invalid port number!";
        }

        else
        {
            this->serverPortNumber = ((this->serverPortNumber * 10) + (currentCharacter - '0'));
        }
    }

    this->serverPortNumber = ((this->serverPortNumber >> 8) | (this->serverPortNumber << 8));
}

void Client::sendMessage(const char* a_messageContent, int a_messageLength)
{
    send(this->fileDescriptor, a_messageContent, a_messageLength, 0);
}

void Client::receiveMessage(char* a_messageBuffer, int a_bufferSize)
{
    recv(this->fileDescriptor, a_messageBuffer, a_bufferSize, 0);
}

bool Client::getConnectionStatus(void)
{
    return this->connectionStatus;
}

bool Client::isDigit(const char a_character)
{
    return ((a_character >= '0') && (a_character <= '9'));
}

#include <sys/socket.h>
#include <netinet/in.h>
#include "client.hpp"

Client::Client(const char* a_clientName, const char* a_serverIPv4Address, unsigned short a_serverPortNumber)
{
    this->name = nullptr;
    this->setName(a_clientName);
    this->setServerIPv4Address(a_serverIPv4Address);
    this->setServerPortNumber(a_serverPortNumber);

    this->fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(this->fileDescriptor < 0)
    {
        throw "Failed to create a socket!";
    }

    sockaddr_in* serverAddress = new sockaddr_in;
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_addr.s_addr = this->serverIPv4Address;
    serverAddress->sin_port = this->serverPortNumber;
    
    this->connectionStatus = (connect(this->fileDescriptor, (sockaddr*)serverAddress, sizeof(sockaddr_in)) == 0) ? true : false;
    if(!this->connectionStatus)
    {
        throw "Failed to connect to the server!";
    }
}

Client::~Client()
{
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

void Client::setServerPortNumber(unsigned short a_serverPortNumber)
{
    this->serverPortNumber = ((a_serverPortNumber >> 8) | (a_serverPortNumber << 8));
}

void Client::sendMessage(const char* a_messageContent, int a_messageLength)
{
    send(this->fileDescriptor, a_messageContent, a_messageLength, 0);
}

void Client::receiveMessage(char* a_messageBuffer)
{
    recv(this->fileDescriptor, a_messageBuffer, sizeof(a_messageBuffer), 0);
}

bool Client::isDigit(const char a_character)
{
    return ((a_character >= '0') && (a_character <= '9'));
}

#include "server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Server::Server(unsigned long a_portNumber, int a_maxAllowedClients)
{
    this->setPortNumber(a_portNumber);
    this->fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if(this->fileDescriptor < 0)
    {
        throw "Failed to create a socket!";
    }

    sockaddr_in* serverAddress = new sockaddr_in;
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_addr.s_addr = INADDR_ANY;
    serverAddress->sin_port = this->portNumber;

    if(bind(this->fileDescriptor, (sockaddr*)serverAddress, sizeof(sockaddr_in)))
    {
        throw "Failed to bound the port!";
    }

    if(listen(this->fileDescriptor, a_maxAllowedClients) < 0)
    {
        throw "Failed to prepare the server to listen!";
    }

    delete serverAddress;
}

Server::~Server()
{
    shutdown(this->fileDescriptor, SHUT_RDWR);
    close(this->fileDescriptor);
}

void Server::setPortNumber(unsigned short a_portNumber)
{
    this->portNumber = ((a_portNumber >> 8) | (a_portNumber << 8));
}

void Server::listenToNewClient(int* a_clientFileDescriptor)
{
    sockaddr_in clientAddress;
    unsigned int clientAddressSize = 0;
    *a_clientFileDescriptor = accept(this->fileDescriptor, (sockaddr*)(&clientAddress), &clientAddressSize);
}

void Server::sendMessage(int a_clientFileDescriptor, const char* a_messageContent, int a_messageLength)
{
    send(a_clientFileDescriptor, a_messageContent, a_messageLength, 0);
}

void Server::receiveMessage(int a_clientFileDescriptor, char* a_messageBuffer, int a_bufferSize)
{
    recv(a_clientFileDescriptor, a_messageBuffer, a_bufferSize, 0);
}

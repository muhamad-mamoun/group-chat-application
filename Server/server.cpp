#include "server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Server::Server(unsigned long a_portNumber)
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

void Server::listenToClient(void)
{
    if(!listen(this->fileDescriptor, 2)) /* 1 is the number of client to be queued, I will change it later. */
    {
        unsigned int clientAddressSize = 0;
        sockaddr_in* clientAddress = new sockaddr_in;
        this->clientFileDescriptor = accept(this->fileDescriptor, (sockaddr*)clientAddress, &clientAddressSize);
        
        if(clientFileDescriptor < 0)
        {
            throw "Failed to connect to a client!";
        }
    }
}

void Server::sendMessage(const char* a_messageContent, int a_messageLength)
{
    send(this->clientFileDescriptor, a_messageContent, a_messageLength, 0);
}

void Server::receiveMessage(char* a_messageBuffer)
{
    recv(this->clientFileDescriptor, a_messageBuffer, 1024, 0);
}

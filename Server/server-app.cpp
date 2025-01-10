#include "../Utils/console.hpp"
#include "server.hpp"
#include <iostream>
#include <cstring>
#include <thread>

#define SERVER_MAX_ALLOWED_CLIENT 10UL

void getUserInput(void);
void listenToNewClient(Server* a_myServer);
void receiveFromClient(Server* a_myServer, int a_clientIndex);
void floodToClients(Server* a_myServer, int a_clientFileDescriptor, char* a_messageBuffer);

struct clientInfo
{
    char name[20];
    bool activeClient;
    int fileDescriptor;
    char messageBuffer[1024];
    std::thread* receiveMessageThread;
}serverClients[SERVER_MAX_ALLOWED_CLIENT];

unsigned short currentClientsCounter = 0;
bool activeSession = true;

int main(void)
{
    Server* myServer = nullptr;

    try
    {
        myServer = new Server(3232, SERVER_MAX_ALLOWED_CLIENT);

        std::thread listenToNewClientThread(listenToNewClient, myServer);
        std::cout << "\nThe server is created successfully and running." << std::endl;
        std::cout << "Press \"exit\" to terminate the session." << std::endl << std::endl;
        std::cout << "Listenning..." << std::endl;
        listenToNewClientThread.detach();
        getUserInput();
    }
    catch(const char* a_exceptionMessage) {ConsUtils::displayError(a_exceptionMessage);}

    delete myServer;
    return 0;
}

void getUserInput(void)
{
    char userInput[1024] = {'\0'};

    while(activeSession == true)
    {
        std::cin >> userInput;
        if(!strcmp(userInput, "exit")) activeSession = false;
    }
}

void listenToNewClient(Server* a_myServer)
{
    while(activeSession == true)
    {
        if(currentClientsCounter < SERVER_MAX_ALLOWED_CLIENT)
        {
            a_myServer->listenToNewClient(&serverClients[currentClientsCounter].fileDescriptor);
            serverClients[currentClientsCounter].receiveMessageThread = new std::thread(receiveFromClient, a_myServer, currentClientsCounter);
            serverClients[currentClientsCounter].receiveMessageThread->detach();
            currentClientsCounter++;
        }
    }
}

void receiveFromClient(Server* a_myServer, int a_clientIndex)
{
    serverClients[a_clientIndex].activeClient = true;
    
    while(serverClients[a_clientIndex].activeClient == true)
    {
        std::fill(serverClients[a_clientIndex].messageBuffer, (serverClients[a_clientIndex].messageBuffer + 1024), '\0');
        a_myServer->receiveMessage(serverClients[a_clientIndex].fileDescriptor, serverClients[a_clientIndex].messageBuffer, sizeof(serverClients[a_clientIndex].messageBuffer));
        if(!strcmp(serverClients[a_clientIndex].messageBuffer, "\0")) serverClients[a_clientIndex].activeClient = false;
        else
        {
            floodToClients(a_myServer, serverClients[a_clientIndex].fileDescriptor, serverClients[a_clientIndex].messageBuffer);
            std::cout << serverClients[a_clientIndex].messageBuffer << std::endl;
        }
    }

    delete serverClients[a_clientIndex].receiveMessageThread;
}

void floodToClients(Server* a_myServer, int a_clientFileDescriptor, char* a_messageBuffer)
{
    for(int clientIndex = 0; clientIndex <= currentClientsCounter; clientIndex++)
    {
        if((serverClients[clientIndex].fileDescriptor != a_clientFileDescriptor) && (serverClients[clientIndex].activeClient == true))
        {
            a_myServer->sendMessage(serverClients[clientIndex].fileDescriptor, a_messageBuffer, strlen(a_messageBuffer));
        }
    }
}

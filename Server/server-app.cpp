/*
=======================================================================================================================
Author       : Mamoun
Module       : Server-side Application
File Name    : server-app.cpp
Date Created : Jan 10, 2025
Description  : Source file for the server-side application.
=======================================================================================================================
*/


/*=====================================================================================================================
                                               < Includes >
=====================================================================================================================*/

#include "../Utils/console.hpp"
#include "server.hpp"
#include <iostream>
#include <cstring>
#include <thread>

/*=====================================================================================================================
                                < Definitions and Static Configurations >
=====================================================================================================================*/

#define SERVER_LISTENING_PORT 3232   /* The port that will be bound by the server to listen on it. */ 
#define SERVER_MAX_ALLOWED_CLIENT 10UL     /* Maximum allowed clients to be handled by the server. */
#define SERVER_MESSAGE_BUFFER_SIZE 1024UL           /* The size of all the server message buffers. */

/*=====================================================================================================================
                                         < Functions Prototypes >
=====================================================================================================================*/

void getUserInput(void);
void listenToNewClient(Server* a_myServer);
void receiveFromClient(Server* a_myServer, int a_clientIndex);
void floodToClients(Server* a_myServer, int a_clientFileDescriptor, char* a_messageBuffer);

/*=====================================================================================================================
                                       < Shared Global Variables >
=====================================================================================================================*/

struct clientInfo
{
    char name[20];
    bool activeClient;
    int fileDescriptor;
    char messageBuffer[SERVER_MESSAGE_BUFFER_SIZE];
    std::thread* receiveMessageThread;
}serverClients[SERVER_MAX_ALLOWED_CLIENT];                      /* Array hosts all server clients. */

unsigned short currentClientsCounter = 0;             /* The number of the current active clients. */
bool activeSession = true;                           /* Flag indicates the current session status. */

/*=====================================================================================================================
                                          < Functions Definitions >
=====================================================================================================================*/

/*=====================================================================================================================
 * [Function Name] : main
 * [Description]   : The entry point for the server-side application.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns int.
 ====================================================================================================================*/
int main(void)
{
    Server* myServer = nullptr;

    try
    {
        /* Create the server with a specific port and specific number of allowed clients. */
        myServer = new Server(SERVER_LISTENING_PORT, SERVER_MAX_ALLOWED_CLIENT);

        /* Create a thread to listen to new clients. */
        std::thread listenToNewClientThread(listenToNewClient, myServer);
        std::cout << "\nThe server created successfully and is currently running." << std::endl;
        std::cout << "Press \"exit\" to terminate the session." << std::endl << std::endl;
        std::cout << "Listening..." << std::endl;
        listenToNewClientThread.detach();
        getUserInput();     /* Read the user input to terminate the session when he need. */
    }
    catch(const char* a_exceptionMessage) {ConsUtils::displayError(a_exceptionMessage);}

    delete myServer;
    return 0;
}

/*=====================================================================================================================
 * [Function Name] : getUserInput
 * [Description]   : Read the user input to terminate the session when he enter "exit".
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void getUserInput(void)
{
    char userInput[SERVER_MESSAGE_BUFFER_SIZE] = {'\0'};

    while(activeSession == true)
    {
        std::cin >> userInput;
        if(!strcmp(userInput, "exit")) activeSession = false;
    }
}

/*=====================================================================================================================
 * [Function Name] : listenToNewClient
 * [Description]   : Listen to clients for new connections.
 * [Arguments]     : <a_myServer>      -> Pointer to the created server object.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void listenToNewClient(Server* a_myServer)
{
    while(activeSession == true)
    {
        if(currentClientsCounter < SERVER_MAX_ALLOWED_CLIENT)
        {
            a_myServer->listenToNewClient(&serverClients[currentClientsCounter].fileDescriptor);

            /* If a new client found, create a new separated thread to receive messages from him. */
            serverClients[currentClientsCounter].receiveMessageThread = new std::thread(receiveFromClient, a_myServer, currentClientsCounter);
            serverClients[currentClientsCounter].receiveMessageThread->detach();
            currentClientsCounter++;
        }
    }
}

/*=====================================================================================================================
 * [Function Name] : receiveFromClient
 * [Description]   : Receive messages from a specific client.
 * [Arguments]     : <a_myServer>      -> Pointer to the created server object.
 *                   <a_clientIndex>   -> The index of the client in the clients array.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void receiveFromClient(Server* a_myServer, int a_clientIndex)
{
    serverClients[a_clientIndex].activeClient = true;
    
    while(serverClients[a_clientIndex].activeClient == true)
    {
        /* Clear the buffer for safe message handling. */
        std::fill(serverClients[a_clientIndex].messageBuffer, (serverClients[a_clientIndex].messageBuffer + SERVER_MESSAGE_BUFFER_SIZE), '\0');
        a_myServer->receiveMessage(serverClients[a_clientIndex].fileDescriptor, serverClients[a_clientIndex].messageBuffer, sizeof(serverClients[a_clientIndex].messageBuffer));

        /* If the termination message came from the user, change the status of this client. */
        if(!strcmp(serverClients[a_clientIndex].messageBuffer, "\0")) serverClients[a_clientIndex].activeClient = false;
        else
        {
            /* Broadcast the message to all the other active clients. */
            floodToClients(a_myServer, serverClients[a_clientIndex].fileDescriptor, serverClients[a_clientIndex].messageBuffer);
            std::cout << serverClients[a_clientIndex].messageBuffer << std::endl;
        }
    }

    delete serverClients[a_clientIndex].receiveMessageThread;
}

/*=====================================================================================================================
 * [Function Name] : floodToClients
 * [Description]   : Broadcast the message to all the other active clients.
 * [Arguments]     : <a_myServer>             -> Pointer to the created server object.
 *                   <a_clientFileDescriptor> -> The file descriptor for the sender client.
 *                   <a_messageBuffer>        -> The buffer that contains the message.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void floodToClients(Server* a_myServer, int a_clientFileDescriptor, char* a_messageBuffer)
{
    for(int clientIndex = 0; clientIndex <= currentClientsCounter; clientIndex++)
    {
        /* If this client is active client and not the sender, send the message to him. */
        if((serverClients[clientIndex].fileDescriptor != a_clientFileDescriptor) && (serverClients[clientIndex].activeClient == true))
        {
            a_myServer->sendMessage(serverClients[clientIndex].fileDescriptor, a_messageBuffer, strlen(a_messageBuffer));
        }
    }
}

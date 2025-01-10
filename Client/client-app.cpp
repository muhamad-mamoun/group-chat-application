/*
=======================================================================================================================
Author       : Mamoun
Module       : Client-side Application
File Name    : client-app.cpp
Date Created : Jan 10, 2025
Description  : Source file for the client-side application.
=======================================================================================================================
*/


/*=====================================================================================================================
                                               < Includes >
=====================================================================================================================*/

#include "../Utils/console.hpp"
#include "../Utils/time.hpp"
#include "client.hpp"
#include <iostream>
#include <cstring>
#include <thread>

/*=====================================================================================================================
                                < Definitions and Static Configurations >
=====================================================================================================================*/

#define CLIENT_MESSAGE_BUFFER_SIZE 1024UL           /* The size of all the client message buffers. */

/*=====================================================================================================================
                                         < Functions Prototypes >
=====================================================================================================================*/

void parseArguments(int a_argCounter, char** a_argValues);
void getServerIPv4Address(void);
void getServerPort(void);
void getClientName(void);
void startSession(Client* a_myClient);
void sendMessage(Client* a_myClient);
void createMessageFrame(char* a_messageFrame, int a_frameSize, const char* a_messageContent);
void receiveMessage(Client* a_myClient);

/*=====================================================================================================================
                                       < Shared Global Variables >
=====================================================================================================================*/

char* serverSocketAddress = nullptr;
char* serverIPv4Address = nullptr;
char* serverPortNumber = nullptr;
char* clientName = nullptr;

/*=====================================================================================================================
                                          < Functions Definitions >
=====================================================================================================================*/

/*=====================================================================================================================
 * [Function Name] : main
 * [Description]   : The entry point for the server-side application.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns int.
 ====================================================================================================================*/
int main(int argc, char** argv)
{
    Client* myClient = nullptr;

    try {parseArguments(argc, argv);} /* If the user entered arguments, parse it. */
    catch(const char* a_exceptionMessage) {ConsUtils::displayError(a_exceptionMessage);};

    /* If there any missed arguments, read it from the user. */
    if((serverSocketAddress == nullptr) && (serverIPv4Address == nullptr)) getServerIPv4Address();
    if((serverSocketAddress == nullptr) && (serverPortNumber == nullptr)) getServerPort();
    if(clientName == nullptr) getClientName();

    std::cout << "\nWelcome, " << clientName << "!" << std::endl;

    try
    {
        /* Create a client to connect to a specific IPv4 on a specific port. */
        if(serverSocketAddress) myClient = new Client(serverSocketAddress);
        else myClient = new Client(serverIPv4Address, serverPortNumber);
        std::cout << "Connected successfully." << std::endl << std::endl;
        startSession(myClient);
    }
    catch(const char* a_exceptionMessage) {ConsUtils::displayError(a_exceptionMessage);}

    if(argc == 1)
    {
        delete[] serverIPv4Address;
        delete[] serverPortNumber;
        delete[] clientName;
    }

    delete myClient;
    return 0;
}

/*=====================================================================================================================
 * [Function Name] : parseArguments
 * [Description]   : Parse the user entered arguments.
 * [Arguments]     : <a_argCounter>    -> The number of the entered arguments.
 *                   <a_argValues>     -> Pointer to the entered arguments.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void parseArguments(int a_argCounter, char** a_argValues)
{
    while(a_argCounter != 1) /* Ignore the 1st argument [Program Name]. */
    {
        if(!std::strcmp(*(a_argValues + a_argCounter - 2), "-s"))
        {
            serverSocketAddress = *(a_argValues + a_argCounter - 1);
            a_argCounter -= 2;
        }

        else if(!std::strcmp(*(a_argValues + a_argCounter - 2), "-i"))
        {
            serverIPv4Address = *(a_argValues + a_argCounter - 1);
            a_argCounter -= 2;
        }

        else if(!std::strcmp(*(a_argValues + a_argCounter - 2), "-p"))
        {
            serverPortNumber = *(a_argValues + a_argCounter - 1);
            a_argCounter -= 2;
        }

        else if(!std::strcmp(*(a_argValues + a_argCounter - 2), "-n"))
        {
            clientName = *(a_argValues + a_argCounter - 1);
            a_argCounter -= 2;
        }

        else
        {
            throw "Inavalid arguments list!";
        }
    }
}

/*=====================================================================================================================
 * [Function Name] : getServerIPv4Address
 * [Description]   : Read the server IPv4 address from the user.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void getServerIPv4Address(void)
{
    serverIPv4Address = new char[16]();
    std::cout << "Enter Server IPv4: ";
    std::cin >> serverIPv4Address;
}

/*=====================================================================================================================
 * [Function Name] : floodToClients
 * [Description]   : Read the server port number from the user.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void getServerPort(void)
{
    serverPortNumber = new char[6]();
    std::cout << "Enter Server Port: ";
    std::cin >> serverPortNumber;
}

/*=====================================================================================================================
 * [Function Name] : floodToClients
 * [Description]   : Read the client name from the user.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void getClientName(void)
{
    clientName = new char[50]();
    std::cout << "Enter your name: ";
    std::cin >> clientName;
}

/*=====================================================================================================================
 * [Function Name] : startSession
 * [Description]   : Start the client-server session [sending/receiving].
 * [Arguments]     : <a_myClient>             -> Pointer to the created client object.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void startSession(Client* a_myClient)
{
    /* Create two separated threads to handle the sending/receiving operations. */
    std::thread sendingThread(sendMessage, a_myClient);
    std::thread receivingThread(receiveMessage, a_myClient);

    receivingThread.detach();
    sendingThread.join();
}

/*=====================================================================================================================
 * [Function Name] : sendMessage
 * [Description]   : Send a message to the server.
 * [Arguments]     : <a_myClient>             -> Pointer to the created client object.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void sendMessage(Client* a_myClient)
{
    char messageFrame[CLIENT_MESSAGE_BUFFER_SIZE] = {'\0'};
    char messageContent[CLIENT_MESSAGE_BUFFER_SIZE] = {'\0'};

    while(a_myClient->getConnectionStatus() == true)
    {
        std::cout << "\r>_ ";
        std::cin.getline(messageContent, CLIENT_MESSAGE_BUFFER_SIZE);

        /* If the user entered the client termination message, terminate the session. */
        if(!strcmp(messageContent, "goodbye")) a_myClient->setConnectionStatus(false);
        else if(strlen(messageContent)) 
        {
            createMessageFrame(messageFrame, sizeof(messageFrame), messageContent);
            a_myClient->sendMessage(messageFrame, strlen(messageFrame));
        }
    }

    /* Terminate the client session. */
    std::cout << "\nGoodbye, " << clientName << "." << std::endl;
    createMessageFrame(messageFrame, CLIENT_MESSAGE_BUFFER_SIZE, "Left the group");
    a_myClient->sendMessage(messageFrame, strlen(messageFrame));
    a_myClient->sendMessage("\0", sizeof("\0"));
}

/*=====================================================================================================================
 * [Function Name] : createMessageFrame
 * [Description]   : Prepare the message frame to be sent [current-time]~[client-name]: <message>.
 * [Arguments]     : <a_messageFrame>    -> Pointer to the frame.
 *                   <a_frameSize>       -> Frame size.
 *                   <a_messageContent>  -> The buffer that contains the message.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void createMessageFrame(char* a_messageFrame, int a_frameSize, const char* a_messageContent)
{
    char currentTime[25] = {'\0'};
    ConsUtils::getTime(currentTime, sizeof(currentTime));
    snprintf(a_messageFrame, a_frameSize, "[%s]~[%s]: %s", currentTime, clientName, a_messageContent);
}

/*=====================================================================================================================
 * [Function Name] : receiveMessage
 * [Description]   : Receive message from the server.
 * [Arguments]     : <a_myClient>             -> Pointer to the created client object.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void receiveMessage(Client* a_myClient)
{
    char messageFrame[CLIENT_MESSAGE_BUFFER_SIZE] = {'\0'};
    
    while(a_myClient->getConnectionStatus() == true)
    {
        std::fill(messageFrame, (messageFrame + CLIENT_MESSAGE_BUFFER_SIZE), '\0');
        a_myClient->receiveMessage(messageFrame, sizeof(messageFrame));
        if(strlen(messageFrame)) std::cout << "\r" << messageFrame << "\n\r>_ " << std::flush;
    }
}

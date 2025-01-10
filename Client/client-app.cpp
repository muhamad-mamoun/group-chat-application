#include "../Utils/console.hpp"
#include "../Utils/time.hpp"
#include "client.hpp"
#include <iostream>
#include <cstring>
#include <thread>

void parseArguments(int a_argCounter, char** a_argValues);
void getServerIPv4Address(void);
void getServerPort(void);
void getClientName(void);
void startSession(Client* a_myClient);
void sendMessage(Client* a_myClient);
void createMessageFrame(char* a_messageFrame, int a_frameSize, const char* a_messageContent);
void receiveMessage(Client* a_myClient);

char* serverSocketAddress = nullptr;
char* serverIPv4Address = nullptr;
char* serverPortNumber = nullptr;
char* clientName = nullptr;

int main(int argc, char** argv)
{
    Client* myClient = nullptr;

    try {parseArguments(argc, argv);}
    catch(const char* a_exceptionMessage) {ConsUtils::displayError(a_exceptionMessage);};

    if((serverSocketAddress == nullptr) && (serverIPv4Address == nullptr)) getServerIPv4Address();
    if((serverSocketAddress == nullptr) && (serverPortNumber == nullptr)) getServerPort();
    if(clientName == nullptr) getClientName();

    std::cout << "\nWelcome, " << clientName << "!" << std::endl;
    std::cout << "Connected successfully." << std::endl << std::endl;

    try
    {
        if(serverSocketAddress) myClient = new Client(serverSocketAddress);
        else myClient = new Client(serverIPv4Address, serverPortNumber);
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

void getServerIPv4Address(void)
{
    serverIPv4Address = new char[16]();
    std::cout << "Enter Server IPv4: ";
    std::cin >> serverIPv4Address;
}

void getServerPort(void)
{
    serverPortNumber = new char[6]();
    std::cout << "Enter Server Port: ";
    std::cin >> serverPortNumber;
}

void getClientName(void)
{
    clientName = new char[50]();
    std::cout << "Enter your name: ";
    std::cin >> clientName;
}

void startSession(Client* a_myClient)
{
    std::thread sendingThread(sendMessage, a_myClient);
    std::thread receivingThread(receiveMessage, a_myClient);

    receivingThread.detach();
    sendingThread.join();
}

void sendMessage(Client* a_myClient)
{
    char messageFrame[1024] = {'\0'};
    char messageContent[1024] = {'\0'};

    while(a_myClient->getConnectionStatus() == true)
    {
        std::cout << "\r>_ ";
        std::cin.getline(messageContent, 1024);
        if(!strcmp(messageContent, "goodbye")) a_myClient->setConnectionStatus(false);
        else if(strlen(messageContent)) 
        {
            createMessageFrame(messageFrame, sizeof(messageFrame), messageContent);
            a_myClient->sendMessage(messageFrame, strlen(messageFrame));
        }
    }

    std::cout << "\nGoodbye, " << clientName << "." << std::endl;
    createMessageFrame(messageFrame, 1024, "Left the group");
    a_myClient->sendMessage(messageFrame, strlen(messageFrame));
    a_myClient->sendMessage("\0", sizeof("\0"));
}

void createMessageFrame(char* a_messageFrame, int a_frameSize, const char* a_messageContent)
{
    char currentTime[25] = {'\0'};
    ConsUtils::getTime(currentTime, sizeof(currentTime));
    snprintf(a_messageFrame, a_frameSize, "[%s]~[%s]: %s", currentTime, clientName, a_messageContent);
}

void receiveMessage(Client* a_myClient)
{
    char messageFrame[1024] = {'\0'};
    
    while(a_myClient->getConnectionStatus() == true)
    {
        std::fill(messageFrame, (messageFrame + 1024), '\0');
        a_myClient->receiveMessage(messageFrame, sizeof(messageFrame));
        if(strlen(messageFrame)) std::cout << "\r" << messageFrame << "\n\r>_ " << std::flush;
    }
}

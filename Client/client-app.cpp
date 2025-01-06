#include "../Utils/console.hpp"
#include "client.hpp"
#include <iostream>
#include <cstring>
#include <chrono>

char* serverSocketAddress = nullptr;
char* serverIPv4Address = nullptr;
char* serverPortNumber = nullptr;
char* clientName = nullptr;

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

void startSession(Client* a_myClient)
{
    char* messageBuffer = new char[1024]();

    do
    {
        std::fill(messageBuffer, (messageBuffer + 1023), '\0');

        std::cout << ">_ ";
        std::cin.getline(messageBuffer, 1024);
        a_myClient->sendMessage(messageBuffer, strlen(messageBuffer));

    } while(strcmp(messageBuffer, "exit"));
}

int main(int argc, char** argv)
{
    Client* myClient = nullptr;

    try {parseArguments(argc, argv);}
    catch(const char* a_exceptionMessage) {ConsUtils::displayError(a_exceptionMessage);};

    if((serverIPv4Address == nullptr) && (serverSocketAddress == nullptr)) getServerIPv4Address();
    if((serverPortNumber == nullptr) && (serverSocketAddress == nullptr)) getServerPort();
    if(clientName == nullptr) getClientName();

    std::cout << "Welcome, " << clientName << "!" << std::endl;

    try
    {
        if(serverSocketAddress) myClient = new Client(clientName, serverSocketAddress);
        else myClient = new Client(clientName, serverIPv4Address, serverPortNumber);
        startSession(myClient);
    }
    catch(const char* a_exceptionMessage) {ConsUtils::displayError(a_exceptionMessage);}

    if(argc == 1)
    {
        delete[] serverIPv4Address;
        delete[] serverPortNumber;
        delete[] clientName;
    }

    return 0;
}

#include "../Utils/console.hpp"
#include "server.hpp"
#include <iostream>
#include <cstring>

int main(void)
{
    Server* myServer = nullptr;
    char* messageBuffer = new char[1024]();

    try
    {
        myServer = new Server(3232);
        myServer->listenToClient();
    }
    catch(const char* a_exceptionMessage) {ConsUtils::displayError(a_exceptionMessage);}

    do
    {
        std::fill(messageBuffer, (messageBuffer + 1023), '\0');
        strcpy(messageBuffer, "[Mamoun]: ");
        myServer->receiveMessage(messageBuffer + 10);
        myServer->sendMessage(messageBuffer, strlen(messageBuffer));
        if(strlen(messageBuffer)) std::cout << messageBuffer << std::endl;
    } while(strcmp(messageBuffer, "exit"));

    delete[] messageBuffer;
    delete myServer;
    return 0;
}

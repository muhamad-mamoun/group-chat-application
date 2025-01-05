#include "server.hpp"
#include <iostream>

int main(void)
{
    try
    {
        Server myServer(3232);
        myServer.listenToClient();
    }
    catch(const char* a_exceptionMessage) {std::cout << "\r[ERROR]: " << a_exceptionMessage << std::endl;}
    
    return 0;
}
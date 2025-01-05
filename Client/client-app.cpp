#include "client.hpp"
#include <iostream>
#include <chrono>

void displayTime(void)
{
    std::time_t currentTime = std::chrono::high_resolution_clock::to_time_t(std::chrono::high_resolution_clock::now());
    
    std::cout << "[";
    std::cout << (std::localtime(&currentTime)->tm_year + 1900) << "-";
    std::cout << (std::localtime(&currentTime)->tm_mon + 1) << "-";
    std::cout << std::localtime(&currentTime)->tm_mday << " ";
    std::cout << std::localtime(&currentTime)->tm_hour << ":";
    std::cout << std::localtime(&currentTime)->tm_min << ":";
    std::cout << std::localtime(&currentTime)->tm_sec << "] ";
}

void getClientName(char** a_clientName)
{
    *a_clientName = new char[50];
    std::cout << "Enter your name: ";
    std::cin >> *a_clientName;
}

int main(int argc, char** argv)
{
    char* clientName = nullptr;
    
    if(argc == 2) clientName = argv[1];
    else getClientName(&clientName);
    
    std::cout << "Welcome, " << clientName << "!" << std::endl;

    try {Client myClient(clientName, "127.0.0.1", 3232);}
    catch(const char* a_exceptionMessage)
    {
        std::cout << "\r";
        displayTime();
        std::cout << "[ERROR]: " << a_exceptionMessage << std::endl;
    }

    if(argc != 2) delete[] clientName;
    return 0;
}

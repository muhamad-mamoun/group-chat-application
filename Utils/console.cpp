#include "console.hpp"
#include <iostream>
#include <chrono>

void ConsUtils::displayError(const char* a_errorMessage)
{
    std::cout << "\r";
    ConsUtils::changeConsoleColor(CONSOLE_COLOR_RED);
    ConsUtils::displayTime();
    std::cout << "[ERROR]: " << a_errorMessage << std::endl;
    ConsUtils::changeConsoleColor(CONSOLE_COLOR_WHITE);
}

void ConsUtils::displayTime(void)
{
    std::time_t currentTime = std::chrono::high_resolution_clock::to_time_t(std::chrono::high_resolution_clock::now());
    
    std::cout << "[";
    std::cout << std::localtime(&currentTime)->tm_year + 1900 << "-";
    std::cout << std::localtime(&currentTime)->tm_mon + 1 << "-";
    std::cout << std::localtime(&currentTime)->tm_mday << " ";
    std::cout << std::localtime(&currentTime)->tm_hour << ":";
    std::cout << std::localtime(&currentTime)->tm_min << ":";
    std::cout << std::localtime(&currentTime)->tm_sec << "] ";
}

void ConsUtils::changeConsoleColor(ConsUtils::consoleColor a_color)
{
    std::cout << "\033[" << a_color << "m";
}

#include "console.hpp"
#include "time.hpp"
#include <iostream>

void ConsUtils::displayError(const char* a_errorMessage)
{
    char timeBuffer[25] = {'\0'};
    ConsUtils::getTime(timeBuffer, sizeof(timeBuffer));

    ConsUtils::changeConsoleColor(CONSOLE_COLOR_RED);
    std::cout << "\r[" << timeBuffer << "]~[ERROR]: " << a_errorMessage << std::endl;
    ConsUtils::changeConsoleColor(CONSOLE_COLOR_WHITE);
}

void ConsUtils::changeConsoleColor(ConsUtils::consoleColor a_color)
{
    std::cout << "\033[" << a_color << "m";
}

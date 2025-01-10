#pragma once

namespace ConsUtils
{
    enum consoleColor
    {
        CONSOLE_COLOR_WHITE = 0,
        CONSOLE_COLOR_RED = 31,
    };

    void changeConsoleColor(ConsUtils::consoleColor a_color);
    void displayError(const char* a_errorMessage);
    void displayTime(void);
};
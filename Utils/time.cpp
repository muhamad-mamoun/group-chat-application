#include "time.hpp"
#include <iostream>
#include <chrono>

void ConsUtils::getTime(char* a_buffer, int a_bufferSize)
{
    std::time_t currentTime = std::chrono::high_resolution_clock::to_time_t(std::chrono::high_resolution_clock::now());
    tm* timeBroken = std::localtime(&currentTime);

    snprintf(a_buffer, 1024, "[%d-%d-%d %d:%d:%d]", \
    (timeBroken->tm_year + 1900), (timeBroken->tm_mon + 1), \
    timeBroken->tm_mday, timeBroken->tm_hour, timeBroken->tm_min, timeBroken->tm_sec);
}

#pragma once

#include <iostream>
#include <chrono>

int getCurrentTimestamp()
{
    const auto p1 = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
}

std::chrono::high_resolution_clock::time_point startClock()
{
    return std::chrono::high_resolution_clock::now();
}

double measureTime(std::chrono::high_resolution_clock::time_point startTime)
{
    auto endTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(endTime - startTime).count();
}
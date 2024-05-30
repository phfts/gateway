#include <chrono>
#include <iostream>

#include "sliding_window_log.h"

void SlidingWindowLog::clean(std::string id, int timestamp)
{
    while (!timestampsPerIp[id].empty() && (timestamp - timestampsPerIp[id][0] > timeInterval))
    {
        // TODO: O(N^2). Bad because can contain bursts. Use sorted set from redis
        timestampsPerIp[id].erase(timestampsPerIp[id].begin());
    }
}

std::chrono::high_resolution_clock::time_point SlidingWindowLog::startClock()
{
    return std::chrono::high_resolution_clock::now();
}

void SlidingWindowLog::finishClockAndLog(std::chrono::high_resolution_clock::time_point startTime)
{
    auto endTime = std::chrono::high_resolution_clock::now();
    double elapsed_time_ms = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    std::cout << "Process time was " << elapsed_time_ms << "ms" << std::endl;
}

bool SlidingWindowLog::checkGlobally(std::string id, int timestamp)
{
    auto startTime = startClock();
    if (timestampsPerIp[id].empty())
    {
        timestampsPerIp[id].push_back(timestamp);
        finishClockAndLog(startTime);

        return true;
    }

    clean(id, timestamp);
    int firstTimestamp = timestampsPerIp[id][0];
    if (timestampsPerIp[id].size() < maxRequests)
    {
        timestampsPerIp[id].push_back(timestamp);
        finishClockAndLog(startTime);
        return true;
    }

    finishClockAndLog(startTime);
    return false;
}

#include <chrono>
#include <iostream>

#include "sliding_window_log.h"

SlidingWindowLog::SlidingWindowLog(int timeInterval, int maxRequests)
    : timeInterval(timeInterval), maxRequests(maxRequests)
{
    char * _redisHost = getenv("REDIS_HOST");
    const std::string host = _redisHost == NULL ? std::string("tcp://127.0.0.1:6379") : std::string(_redisHost);
    redis = std::make_shared<Redis>(host);
}

bool SlidingWindowLog::checkGlobally(std::string id, int timestamp)
{
    redis->zincrby(id, 1, std::to_string(timestamp));
    redis->zrem(id, std::initializer_list<int>{0, timestamp - timeInterval});

    std::vector<std::pair<std::string, double>> zset_result;
    redis->zrangebyscore(id, UnboundedInterval<double>{}, std::back_inserter(zset_result));

    double sum = 0;
    for (auto x : zset_result)
    {
        sum += x.second;
    }

    return sum <= maxRequests;
}

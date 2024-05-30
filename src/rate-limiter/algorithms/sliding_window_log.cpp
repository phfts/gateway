#include <chrono>
#include <iostream>

#include "sliding_window_log.h"

SlidingWindowLog::SlidingWindowLog(int timeInterval, int maxRequests)
    : timeInterval(timeInterval), maxRequests(maxRequests)
{
    redis = std::make_shared<Redis>("tcp://127.0.0.1:6379");
}

bool SlidingWindowLog::checkGlobally(std::string id, int timestamp)
{
    RedLockMutex mtx(redis, "rate_limiter_lock_user_" + id);
    RedLock<RedLockMutex> lock(mtx, std::defer_lock);
    
    try
    {
        lock.try_lock(std::chrono::seconds(10));
        redis->zincrby(id, 1, std::to_string(timestamp));
        redis->zrem(id, std::initializer_list<int>{0, timestamp - timeInterval});

        std::vector<std::pair<std::string, double>> zset_result;
        redis->zrangebyscore(id, UnboundedInterval<double>{}, std::back_inserter(zset_result));

        double sum = 0;
        for (auto x : zset_result)
        {
            sum += x.second;
        }

        lock.unlock();
        return sum <= maxRequests;
    }
    catch (const std::exception &e)
    {
        lock.unlock();
        std::cerr << e.what() << '\n';
    }
}


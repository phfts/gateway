#pragma once

#include <string>
#include <vector>
#include <map>
#include "./algorithms/sliding_window_log.cpp"

class RateLimiter
{
private:
    bool _hasGlobalRateLimit;
    std::string timeUnit;
    int timeInterval;
    int maxRequests;
    std::vector<std::string> identificationMethods;
    SlidingWindowLog *slidingWindowLog; // TODO: fix to use Dependency Injection

    void validateTimeUnit(std::string _timeUnit);
    void validateIdentificationMethod(std::string idMethod);

public:
    RateLimiter(std::string timeUnit, int timeInterval, int maxRequests, std::vector<std::string> identificationMethods);
    RateLimiter(std::string file);
    bool hasGlobalRateLimit();
    void setHasGlobalLimit(bool _hasGlobalRateLimit);
    void setMaxRequests(int _maxRequests);
    void setIdentificationMethods(std::vector<std::string> idMethods);
    void setTimeInterval(int _timeInterval);
    void setTimeUnit(std::string _timeUnit);
    int getRateLimitIntervalInSeconds();
    bool checkGlobally(std::string ip);
};
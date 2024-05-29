#pragma once

#include <vector>
#include <string>
#include "../utils/string.cpp"
#include "../utils/time.cpp"
#include "rate_limiter.h"
#include "../config/yaml_loader.cpp"

void RateLimiter::validateTimeUnit(std::string _timeUnit)
{
    // TODO: allow more time units
    if (_timeUnit != "minute")
        throw std::runtime_error("Only minute time unit is allowed for now");
}

void RateLimiter::validateIdentificationMethod(std::string idMethod)
{
    // TODO: allow other id methods
    if (idMethod != "IP")
        throw std::runtime_error("Only IP identification method is allowed for now");
}

bool RateLimiter::hasGlobalRateLimit()
{
    return _hasGlobalRateLimit;
}

void RateLimiter::setHasGlobalLimit(bool _hasGlobalRateLimit)
{
    _hasGlobalRateLimit = _hasGlobalRateLimit;
}

void RateLimiter::setMaxRequests(int _maxRequests)
{
    maxRequests = _maxRequests;
}

void RateLimiter::setIdentificationMethods(std::vector<std::string> idMethods)
{
    for (int i = 0; i < idMethods.size(); i++)
    {
        validateIdentificationMethod(idMethods[i]);
        identificationMethods.push_back(idMethods[i]);
    }
}

void RateLimiter::setTimeInterval(int _timeInterval)
{
    timeInterval = _timeInterval;
}

void RateLimiter::setTimeUnit(std::string _timeUnit)
{
    validateTimeUnit(_timeUnit);
    timeUnit = _timeUnit;
}

int RateLimiter::getRateLimitIntervalInSeconds()
{
    return 60 * timeInterval;
}

bool RateLimiter::checkGlobally(std::string ip)
{
    int timestamp = getCurrentTimestamp();
    if (timestampsPerIp[ip].empty())
    {
        timestampsPerIp[ip].push_back(timestamp);
        return true;
    }
    else
    {
        int firstTimestamp = timestampsPerIp[ip][0];
        if (timestamp - firstTimestamp > this->getRateLimitIntervalInSeconds())
        {
            timestampsPerIp[ip].push_back(timestamp);
            while (!timestampsPerIp[ip].empty() && (timestamp -  timestampsPerIp[ip][0] > this->getRateLimitIntervalInSeconds()))
            {
                // TODO: O(N^2). Bad because can contain bursts. Use sorted set from redis
                timestampsPerIp[ip].erase(timestampsPerIp[ip].begin());
            }
            
            return true;
        }
        else
        {
            timestampsPerIp[ip].push_back(timestamp);
            bool ans = timestampsPerIp[ip].size() <= maxRequests;
            return ans;
        }
    }
}

std::vector<std::string> toArrayOfStrings(WsjcppYamlCursor values)
{
    std::vector<std::string> ans;
    for (int i = 0; i < values.size(); i++)
    {
        ans.push_back(values[i]);
    }
    return ans;
}

RateLimiter::RateLimiter(std::string timeUnit, int timeInterval, int maxRequests, std::vector<std::string> identificationMethods)
: timeUnit(timeUnit), timeInterval(timeInterval), maxRequests(maxRequests), identificationMethods(identificationMethods)
{
}

RateLimiter::RateLimiter(std::string file)
{
    WsjcppYaml yaml = loadYaml(file);
    if (!yaml["global-rate-limit"].isNull())
    {
        setHasGlobalLimit(true);
        setTimeUnit(yaml["global-rate-limit"]["time_unit"].valStr());
        setTimeInterval(yaml["global-rate-limit"]["time_interval"].valInt());
        setMaxRequests(yaml["global-rate-limit"]["max_requests"].valInt());
        setIdentificationMethods(toArrayOfStrings(yaml["global-rate-limit"]["identification_methods"]));
    }
    else
    {
        setHasGlobalLimit(false);
    }
}

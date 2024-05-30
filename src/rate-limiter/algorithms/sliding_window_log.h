#include <string>
#include <map>
#include <vector>
#include <sw/redis++/redis++.h>
#include <sw/redis++/patterns/redlock.h>

using namespace sw::redis;

class SlidingWindowLog
{
private:
    int timeInterval;
    int maxRequests;
    std::shared_ptr<Redis> redis;
public:
    SlidingWindowLog(int timeInterval, int maxRequests);
    bool checkGlobally(std::string id, int timestamp);
};
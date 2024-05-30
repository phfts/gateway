#include <string>
#include <map>
#include <vector>

class SlidingWindowLog
{
private:
    std::map<std::string, std::vector<int>> timestampsPerIp;
    int timeInterval;
    int maxRequests;
    void clean(std::string id, int timestamp);

    std::chrono::high_resolution_clock::time_point startClock();
    void finishClockAndLog(std::chrono::high_resolution_clock::time_point startTime);

public:
    SlidingWindowLog(int timeInterval, int maxRequests) : timeInterval(timeInterval), maxRequests(maxRequests) {}
    bool checkGlobally(std::string id, int timestamp);
};
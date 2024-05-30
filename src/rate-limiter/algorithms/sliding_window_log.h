#include <string>
#include <map>
#include <vector>

class SlidingWindowLog
{
private:
    std::map<std::string, std::vector<int>> timestampsPerIp;
    int timeInterval;
    int maxRequests;

    void clean(std::string id, int timestamp)
    {
        while (!timestampsPerIp[id].empty() && (timestamp - timestampsPerIp[id][0] > timeInterval))
        {
            // TODO: O(N^2). Bad because can contain bursts. Use sorted set from redis
            timestampsPerIp[id].erase(timestampsPerIp[id].begin());
        }
    }

public:
    SlidingWindowLog(int timeInterval, int maxRequests) : timeInterval(timeInterval), maxRequests(maxRequests) {}
    bool checkGlobally(std::string id, int timestamp)
    {
        if (timestampsPerIp[id].empty())
        {
            timestampsPerIp[id].push_back(timestamp);
            return true;
        }
        
        clean(id, timestamp);
        int firstTimestamp = timestampsPerIp[id][0];
        if (timestampsPerIp[id].size() < maxRequests)
        {
            timestampsPerIp[id].push_back(timestamp);
            return true;
        }

        return false;
    }
};
#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "../utils/string.cpp"

class Route
{
private:
    std::string sanitizePath(std::string path)
    {
        int n = path.length();
        if (n == 0) 
            return path;
        
        std::stringstream ss;
        ss << (char)(path[0]);
        for (int i=1; i<n;i++)
        {
            if (path[i-1] != '/' || path[i] != '/')
                ss << path[i];
        }
        return ss.str();
    }

public:
    std::string path;
    std::string targetHost;
    std::string targetPath;
    bool stripPath;

    std::string resolveRequestPath(std::string requestPath)
    {
        if (!stripPath)
            return requestPath;

        if (beginsWith(requestPath, path))
        {
            return sanitizePath(targetPath + requestPath.substr(path.size(), requestPath.size() - path.size()));
        }
        
        return sanitizePath(targetPath + requestPath);
    }
};

class Router
{
private:
    std::vector<Route*> routes;

public:
    Router(std::string file);
    std::vector<Route*> getRoutes();
};
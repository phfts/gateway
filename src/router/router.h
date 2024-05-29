#pragma once

#include <string>
#include <vector>

struct Route
{
    std::string verb;
    std::string path;
    std::string targetHost;
    std::string targetPath;
};

class Router
{
private:
    std::vector<Route*> routes;

public:
    Router(std::string file);
    std::vector<Route*> getRoutes();
};
#pragma once

#include "router.h"
#include "../config/yaml_loader.cpp"

Router::Router(std::string file)
{
    WsjcppYaml yaml = loadYaml(file);

    if (!yaml["routes"].isNull())
    {
        for (int i=0; i<yaml["routes"].size(); i++)
        {   
            auto route = new Route();
            route->path = yaml["routes"][i]["path"].valStr();
            route->targetHost = yaml["routes"][i]["target_host"].valStr();
            route->targetPath = yaml["routes"][i]["target_path"].valStr();
            route->stripPath = yaml["routes"][i]["strip_path"].valBool();
            routes.push_back(route);
        }
    }
    else
    {
        throw std::runtime_error("Invalid config file " + file + ". Property routes cant be null");
    }
}

std::vector<Route*> Router::getRoutes()
{
    return routes;
}

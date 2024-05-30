#pragma once

#include "./web_server.h"
#include "./handlers/handlers.cpp"

WebServer::WebServer(std::vector<Route*> &routes, RateLimiter& rateLimiter)
{
    svr = new httplib::Server();
    for (auto &route : routes)
    {
        std::vector<std::string> patterns = {std::string(route->path) + "/.*", route->path};
        for (auto &pattern : patterns)
        {
            svr->Get(pattern, [route, &rateLimiter](const httplib::Request &req, httplib::Response &res)
            {
                getHandler(req, res, route, rateLimiter);
            });

            svr->Post(pattern, [route, &rateLimiter](const httplib::Request &req, httplib::Response &res)
            {
                postHandler(req, res, route, rateLimiter);
            });
        }   
    }
}

void WebServer::start() {
    std::cout << "Server started " << std::endl;
    svr->listen("0.0.0.0", 8080);
}

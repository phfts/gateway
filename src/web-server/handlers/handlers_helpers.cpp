#pragma once

#include "../../../lib/cpp-httplib/httplib.h"
#include "../../router/router.h"

void setupResponse(httplib::Response &res, std::string &body, std::string contentType, int code)
{
    res.set_content(body, contentType);
    res.status = code;
}

httplib::Headers buildHeaders()
{
    return httplib::Headers{
        {"Access-Control-Allow-Origin", "*"}, // TODO: use cors as config
    };
}

std::string paramsToString(httplib::Params params)
{
    std::stringstream ss;
    for (auto &param : params)
    {
        ss << " { " << param.first << " : " << param.second << " } ";
    }
    return ss.str();
}

void logRedirection(std::string method, std::string &newPath, Route *route, const httplib::Request &req)
{
    std::cout << "Redirected " << method << " to " << newPath << " of host " << route->targetHost << " with params: " << paramsToString(req.params) << std::endl;
}


bool refererIsSameAsRoute(const httplib::Request &req, Route *route)
{
    std::string host = std::string(req.get_header_value("Host"));
    std::string referer = std::string(req.get_header_value("Referer"));
    std::string hostWithPath = host + std::string(route->path);
    return (std::string("http://") + hostWithPath == referer) || (std::string("https://") + hostWithPath == referer);
}
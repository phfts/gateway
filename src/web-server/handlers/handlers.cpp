#pragma once

#include "../../../lib/cpp-httplib/httplib.h"
#include "../../rate-limiter/rate_limiter.h"
#include "../../router/router.h"
#include "./middlewares.cpp"
#include "handlers_helpers.cpp"

// TODO: think about headers to redirect
void getHandler(const httplib::Request &req, httplib::Response &res, Route *route, RateLimiter &rateLimiter)
{
    if (!checkUserIsAllowed(rateLimiter, req, res))
        return;

    httplib::Client cli(route->targetHost);
    auto newPath = route->resolveRequestPath(req.path);
    auto newRes = cli.Get(newPath, req.params, buildHeaders());
    logRedirection("GET", newPath, route, req);
    setupResponse(res, newRes->body, req.get_header_value("content-type"), newRes->status);
}

void postHandler(const httplib::Request &req, httplib::Response &res, Route *route, RateLimiter &rateLimiter)
{
    if (!checkUserIsAllowed(rateLimiter, req, res))
        return;

    httplib::Client cli(route->targetHost);
    auto newPath = route->resolveRequestPath(req.path);
    auto newRes = cli.Post(newPath, req.headers, req.body, req.get_header_value("content-type"));
    logRedirection("POST", newPath, route, req);
    setupResponse(res, newRes->body, req.get_header_value("content-type"), newRes->status);
}

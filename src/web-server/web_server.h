#pragma once

#include "../../lib/cpp-httplib/httplib.h"
#include "../rate-limiter/rate_limiter.h"
#include "../router/router.h"

class WebServer
{
private:
    httplib::Server *svr;

public:
    WebServer(std::vector<Route*> &routes, RateLimiter& rateLimiter);
    void start();
};
#pragma once

#include "../../../lib/cpp-httplib/httplib.h"
#include "../../rate-limiter/rate_limiter.h"

bool checkUserIsAllowed(RateLimiter &rateLimiter, const httplib::Request &req, httplib::Response &res)
{
    if (!rateLimiter.checkGlobally(req.remote_addr))
    {
        res.status = 429;
        res.set_content("Rate limited", "text/plain");
        return false;
    }
    return true;
}

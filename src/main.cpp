#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>

#include "../lib/cpp-httplib/httplib.h"
#include "rate-limiter/rate_limiter.cpp"
#include "router/router.cpp"
#include "web-server/web_server.cpp"

using namespace std;


int main(int argc, char const *argv[])
{
    auto rateLimiter = RateLimiter("routes.yml");
    auto router = Router("routes.yml");
    std::vector<Route*> routes = router.getRoutes();
    
    auto server = WebServer(routes, rateLimiter);
    server.start();

    return 0;
}

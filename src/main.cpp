#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../lib/cpp-httplib/httplib.h"
#include "rate-limiter/rate_limiter.cpp"
#include "router/router.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
    auto rateLimiter = RateLimiter("routes.yml");
    auto router = Router("routes.yml");

    httplib::Server svr;

    // TODO: only allow GET verbs
    for (auto &route : router.getRoutes())
    {
        if (route->verb == "GET")
        {
            svr.Get(route->path, [route, &rateLimiter](const httplib::Request &req, httplib::Response &res)
            {
                httplib::Client cli(route->targetHost);
                std::string ip = req.remote_addr;

                if (rateLimiter.checkGlobally(ip))
                {
                    auto newRes = cli.Get(route->targetPath);
                    res.set_content(newRes->body, req.get_header_value("content-type"));
                    res.status = newRes->status;
                }
                else
                {
                    res.status = 429;
                    res.set_content("Rate limited", "text/plain");
                } 
            });
        }
    }

    std::cout << "Server started " << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}

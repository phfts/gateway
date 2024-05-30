#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../lib/cpp-httplib/httplib.h"
#include "rate-limiter/rate_limiter.cpp"
#include "router/router.cpp"
#include "main.h"

using namespace std;

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


int main(int argc, char const *argv[])
{
    auto rateLimiter = RateLimiter("routes.yml");
    auto router = Router("routes.yml");

    httplib::Server svr;

    // TODO: deal with files
    // TODO: handle paths better (stripping)
    for (auto &route : router.getRoutes())
    {
        svr.Get(route->path, [route, &rateLimiter](const httplib::Request &req, httplib::Response &res)
        {
            if (!checkUserIsAllowed(rateLimiter, req, res))
                return;

            httplib::Client cli(route->targetHost);
            auto newRes = cli.Get(route->targetPath);
            setupResponse(res, newRes, req);
        });

        svr.Post(route->path, [route, &rateLimiter](const httplib::Request &req, httplib::Response &res)
        {
            if (!checkUserIsAllowed(rateLimiter, req, res))
                return;

            httplib::Client cli(route->targetHost);
            auto newRes = cli.Post(route->targetPath, req.headers, req.body, req.get_header_value("content-type"));
            setupResponse(res, newRes, req);
        });

        svr.Patch(route->path, [route, &rateLimiter](const httplib::Request &req, httplib::Response &res)
        {
            if (!checkUserIsAllowed(rateLimiter, req, res))
                return;

            httplib::Client cli(route->targetHost);
            auto newRes = cli.Patch(route->targetPath, req.headers, req.body, req.get_header_value("content-type"));
            setupResponse(res, newRes, req);
        });


        svr.Put(route->path, [route, &rateLimiter](const httplib::Request &req, httplib::Response &res)
        {
            if (!checkUserIsAllowed(rateLimiter, req, res))
                return;

            httplib::Client cli(route->targetHost);
            auto newRes = cli.Put(route->targetPath, req.headers, req.body, req.get_header_value("content-type"));
            setupResponse(res, newRes, req);
        });  

        svr.Delete(route->path, [route, &rateLimiter](const httplib::Request &req, httplib::Response &res)
        {
            if (!checkUserIsAllowed(rateLimiter, req, res))
                return;

            httplib::Client cli(route->targetHost);
            auto newRes = cli.Delete(route->targetPath, req.headers);
            setupResponse(res, newRes, req);
        });        
    }

    std::cout << "Server started " << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}



void setupResponse(httplib::Response &res, httplib::Result &newRes, const httplib::Request &req)
{
    res.set_content(newRes->body, req.get_header_value("content-type"));
    res.status = newRes->status;
}

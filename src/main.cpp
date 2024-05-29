#include "../lib/cpp-httplib/httplib.h"
#include "rate-limiter/rate_limiter.cpp"

using namespace std;


int main(int argc, char const *argv[])
{
    auto rateLimiter = RateLimiter("routes.yml");

    // httplib::Server svr;

    // int count = 0;
    // svr.Get("/hi", [&count](const httplib::Request &, httplib::Response &res) {
    //     res.set_content("Response number " + to_string(++count) + "\n", "text/plain");
    // });

    // svr.listen("0.0.0.0", 8080);
    return 0;
}

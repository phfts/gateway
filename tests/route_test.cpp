#include <gtest/gtest.h>
#include "../src/router/router.cpp"

TEST(Route, resolveRequestPathWithStripTrue) {
    auto route = Route();
    route.path = "/hi";
    route.targetHost = "https://www.google.com";
    route.targetPath = "/";
    route.stripPath = true;

  EXPECT_EQ("/", route.resolveRequestPath("/hi"));
  EXPECT_EQ("/", route.resolveRequestPath("/hi/"));
  EXPECT_EQ("/123", route.resolveRequestPath("/hi/123"));
  EXPECT_EQ("/123/", route.resolveRequestPath("/hi/123/"));
}

TEST(Route, resolveRequestPathWithStripFalse) {
    auto route = Route();
    route.path = "/hi";
    route.targetHost = "https://www.google.com";
    route.targetPath = "/";
    route.stripPath = false;

  EXPECT_EQ("/hi", route.resolveRequestPath("/hi"));
  EXPECT_EQ("/hi/", route.resolveRequestPath("/hi/"));
  EXPECT_EQ("/hi/123", route.resolveRequestPath("/hi/123"));
}

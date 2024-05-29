#include <gtest/gtest.h>
#include "../src/rate-limiter/rate_limiter.cpp"

// Tests that the Foo::Bar() method does Abc.
TEST(FooTest, MethodBarDoesAbc) {
  auto rateLimit = RateLimiter("minute", 60, 5, {"IP"});
  
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  EXPECT_EQ(false, rateLimit.checkGlobally("1.2.3.4"));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
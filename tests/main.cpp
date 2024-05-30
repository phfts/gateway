#include <gtest/gtest.h>
#include <sw/redis++/redis++.h>

#include "../src/rate-limiter/rate_limiter.cpp"
#include "../src/utils/time.cpp"


TEST(RateLimiter, checkGlobally) {
  auto redis = new Redis("tcp://127.0.0.1:6379");
  redis->flushall();

  auto rateLimit = RateLimiter("minute", 60, 5, {"IP"});
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.4"));
  
  EXPECT_EQ(false, rateLimit.checkGlobally("1.2.3.4"));

  EXPECT_EQ(true, rateLimit.checkGlobally("1.2.3.5"));
}


TEST(RateLimiter, checkGloballyLoad) {
  auto redis = new Redis("tcp://127.0.0.1:6379");
  redis->flushall();
  int N = 10e3;

  auto rateLimit = RateLimiter("minute", 60, 3, {"IP"});
  auto startTime = startClock();

  for (int i=0; i < N; i++)
  {
    EXPECT_EQ(i < 3, rateLimit.checkGlobally("1.2.3.4"));
  }

  double time = measureTime(startTime);
  std::cout << "Processed " << N << " requests in " << time << " ms" << std::endl;
  EXPECT_EQ(true, time < 3e3);
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
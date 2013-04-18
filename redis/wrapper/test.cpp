/**
 * test.cpp
 * More info on http://thisthread.blogspot.com/2013/04/redis-client-testing.html
 */

#include "gtest/gtest.h"
#include "UniqueRedis.h"
#include "SharedRedis.h"

TEST(TestConnectShared, BadPort)
{
    TTR::SharedRedis redis(1234);
    ASSERT_FALSE(redis.isConnected());
}

TEST(TestConnectShared, Vanilla)
{
    TTR::SharedRedis redis;
    ASSERT_TRUE(redis.isConnected());
}

TEST(TestCopyShared, Copy)
{
    TTR::SharedRedis redis;
    ASSERT_TRUE(redis.isConnected());
    TTR::SharedRedis r2 = redis;
    ASSERT_TRUE(r2.isConnected());
}

TEST(TestConnectUnique, Vanilla)
{
    TTR::UniqueRedis& redis = TTR::UniqueRedis::instance();
    ASSERT_TRUE(redis.isConnected());
}

/**
 * Tester.h
 * more info on http://thisthread.blogspot.com/2013/04/redis-client-testing.html
 */

#pragma once
#include "gtest/gtest.h"

class Tester
{
public:
    Tester(int argc, char** argv) { testing::InitGoogleTest(&argc, argv); }

    int run() { return RUN_ALL_TESTS(); }
};

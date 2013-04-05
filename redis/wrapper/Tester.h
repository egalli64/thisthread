#pragma once
#include "gtest/gtest.h"

class Tester
{
public:
    Tester(int argc, char** argv) { testing::InitGoogleTest(&argc, argv); }

    int run() { return RUN_ALL_TESTS(); }
};


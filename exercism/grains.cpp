/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Grains https://exercism.org/tracks/cpp/exercises/grains/

    Calculate the number of grains of wheat on a chessboard given that the number on each square doubles.
    - how many grains were on a given square, and
    - the total number of grains on the chessboard
 */
#include <stdint.h>
#include "grains.h"

namespace grains
{
    unsigned long long square(int cell)
    {   // parentesys are just to avoid readability warning
        return 1ULL << (cell - 1);
    }

    unsigned long long total()
    {
        return UINT64_MAX;
    }
}

// ---
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("square_1")
{
    REQUIRE(1ULL == grains::square(1));
}

TEST_CASE("square_2")
{
    REQUIRE(2ULL == grains::square(2));
}

TEST_CASE("square_3")
{
    REQUIRE(4ULL == grains::square(3));
}

TEST_CASE("square_4")
{
    REQUIRE(8ULL == grains::square(4));
}

TEST_CASE("square_16")
{
    REQUIRE(32768ULL == grains::square(16));
}

TEST_CASE("square_32")
{
    REQUIRE(2147483648ULL == grains::square(32));
}

TEST_CASE("square_64")
{
    REQUIRE(9223372036854775808ULL == grains::square(64));
}

TEST_CASE("total")
{
    REQUIRE(18446744073709551615ULL == grains::total());
}

/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Leap https://exercism.org/tracks/cpp/exercises/leap/
 */
#include "leap.h"

namespace leap
{
    bool is_leap_year(int year)
    {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }
} // namespace leap

// ---
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("smoke test")
{
    CHECK(leap::is_leap_year(2000));
    CHECK(!leap::is_leap_year(2100));
}
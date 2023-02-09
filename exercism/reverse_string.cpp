/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Reverse String https://exercism.org/tracks/cpp/exercises/reverse-string/
 */
#include "reverse_string.h"

namespace reverse_string
{
    std::string reverse_string(std::string const &input)
    {
        return {input.rbegin(), input.rend()};
    }
}

// ---
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("smoke test")
{
    CHECK("" == reverse_string::reverse_string(""));
    CHECK("tobor" == reverse_string::reverse_string("robot"));
}
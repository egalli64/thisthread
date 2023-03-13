/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Two Fer https://exercism.org/tracks/cpp/exercises/two_fer/

    Given a string, use it as a filler for "One for {}, one for me."
    - if no string is passed, use "you" as default
 */
#include "two_fer.h"

namespace two_fer
{
    std::string two_fer(const std::string& s)
    {
        return "One for " + (s.empty() ? "you" : s) + ", one for me.";
    }
}

// ---
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("no_name_given")
{
    const std::string actual = two_fer::two_fer();
    const std::string expected = "One for you, one for me.";
    REQUIRE(actual == expected);
}

TEST_CASE("a_name_given")
{
    const std::string name = "Alice";
    const std::string actual = two_fer::two_fer(name);
    const std::string expected = "One for Alice, one for me.";
    REQUIRE(actual == expected);
}

TEST_CASE("another_name_given")
{
    const std::string name = "Bob";
    const std::string actual = two_fer::two_fer(name);
    const std::string expected = "One for Bob, one for me.";
    REQUIRE(actual == expected);
}
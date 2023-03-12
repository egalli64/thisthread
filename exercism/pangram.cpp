/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Pangram https://exercism.org/tracks/cpp/exercises/pangram/

    Given a string, return true if it is a pangram
    - all letters in [a..z] should be present (low or upper case)
 */
#include <cctype>
#include "pangram.h"

namespace pangram
{
    bool is_pangram(const std::string &s)
    {
        bool flags[26]{};
        for (const char cur : s)
            if (std::isalpha(cur))
                flags[std::tolower(cur) - 'a'] = true;

        for (bool flag : flags)
            if (!flag)
                return false;
        return true;
    }
}

// ---
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("sentence_empty")
{
    REQUIRE(!pangram::is_pangram(""));
}

TEST_CASE("pangram_with_only_lower_case")
{
    REQUIRE(pangram::is_pangram("the quick brown fox jumps over the lazy dog"));
}

TEST_CASE("missing_character_x")
{
    REQUIRE(!pangram::is_pangram("a quick movement of the enemy will jeopardize five gunboats"));
}

TEST_CASE("another_missing_x")
{
    REQUIRE(!pangram::is_pangram("the quick brown fish jumps over the lazy dog"));
}

TEST_CASE("pangram_with_underscores")
{
    REQUIRE(pangram::is_pangram("the_quick_brown_fox_jumps_over_the_lazy_dog"));
}

TEST_CASE("pangram_with_numbers")
{
    REQUIRE(pangram::is_pangram("the 1 quick brown fox jumps over the 2 lazy dogs"));
}

TEST_CASE("missing_letters_replaced_with_numbers")
{
    REQUIRE(!pangram::is_pangram("7h3 qu1ck brown fox jumps ov3r 7h3 lazy dog"));
}

TEST_CASE("pangram_with_mixed_case_and_punctuation")
{
    REQUIRE(pangram::is_pangram("\"Five quacking Zephyrs jolt my wax bed.\""));
}

TEST_CASE("upper_and_lower_should_not_be_counted_separately")
{
    REQUIRE(!pangram::is_pangram("the quick brown fox jumps over with lazy FX"));
}

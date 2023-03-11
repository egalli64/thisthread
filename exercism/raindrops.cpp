/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Raindrops https://exercism.org/tracks/cpp/exercises/raindrops/

    Given an integer, generate a string by conditional concatenation of
    - "Pling" if input has factor 3
    - "Plang" ... 5
    - "Plong" ... 7
    - In none of the above, return the integer converted in string
 */
#include <string>
#include "raindrops.h"

namespace raindrops
{
    std::string convert(int input)
    {
        std::string buffer;

        if (input % 3 == 0)
        {
            buffer.append("Pling");
        }
        if (input % 5 == 0)
        {
            buffer.append("Plang");
        }
        if (input % 7 == 0)
        {
            buffer.append("Plong");
        }
        return buffer.length() == 0 ? std::to_string(input) : buffer;
    }
}

// ---
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("the_sound_for_1_is_1")
{
    REQUIRE("1" == raindrops::convert(1));
}

TEST_CASE("the_sound_for_3_is_pling")
{
    REQUIRE("Pling" == raindrops::convert(3));
}

TEST_CASE("the_sound_for_5_is_plang")
{
    REQUIRE("Plang" == raindrops::convert(5));
}

TEST_CASE("the_sound_for_7_is_plong")
{
    REQUIRE("Plong" == raindrops::convert(7));
}

TEST_CASE("the_sound_for_6_is_pling_as_it_has_a_factor_3")
{
    REQUIRE("Pling" == raindrops::convert(6));
}

TEST_CASE("2_to_the_power_3_does_not_make_a_raindrop_sound_as_3_is_the_exponent_not_the_base")
{
    REQUIRE("8" == raindrops::convert(8));
}

TEST_CASE("the_sound_for_9_is_pling_as_it_has_a_factor_3")
{
    REQUIRE("Pling" == raindrops::convert(9));
}

TEST_CASE("the_sound_for_10_is_plang_as_it_has_a_factor_5")
{
    REQUIRE("Plang" == raindrops::convert(10));
}

TEST_CASE("the_sound_for_14_is_plong_as_it_has_a_factor_of_7")
{
    REQUIRE("Plong" == raindrops::convert(14));
}

TEST_CASE("the_sound_for_15_is_plingplang_as_it_has_factors_3_and_5")
{
    REQUIRE("PlingPlang" == raindrops::convert(15));
}

TEST_CASE("the_sound_for_21_is_plingplong_as_it_has_factors_3_and_7")
{
    REQUIRE("PlingPlong" == raindrops::convert(21));
}

TEST_CASE("the_sound_for_25_is_plang_as_it_has_a_factor_5")
{
    REQUIRE("Plang" == raindrops::convert(25));
}

TEST_CASE("the_sound_for_27_is_pling_as_it_has_a_factor_3")
{
    REQUIRE("Pling" == raindrops::convert(27));
}

TEST_CASE("the_sound_for_35_is_plangplong_as_it_has_factors_5_and_7")
{
    REQUIRE("PlangPlong" == raindrops::convert(35));
}

TEST_CASE("the_sound_for_49_is_plong_as_it_has_a_factor_7")
{
    REQUIRE("Plong" == raindrops::convert(49));
}

TEST_CASE("the_sound_for_52_is_52")
{
    REQUIRE("52" == raindrops::convert(52));
}

TEST_CASE("the_sound_for_105_is_plingplangplong_as_it_has_factors_3_5_and_7")
{
    REQUIRE("PlingPlangPlong" == raindrops::convert(105));
}

TEST_CASE("the_sound_for_3125_is_plang_as_it_has_a_factor_5")
{
    REQUIRE("Plang" == raindrops::convert(3125));
}
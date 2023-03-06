/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Space Age https://exercism.org/tracks/cpp/exercises/space-age/

    Given an age in seconds, calculate how old someone would be on the planets in the solar system
 */
#include "space_age.h"

// ---
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("age_in_seconds")
{
    const space_age::space_age age(1'000'000);

    REQUIRE(age.seconds() == 1'000'000);
}

namespace
{
    const double accuracy = 0.005;
}

TEST_CASE("age_in_earth_years")
{
    const space_age::space_age age(1'000'000'000);

    // 'Approx' is a helper from the test suite for comparing floating point

    // numbers.

    REQUIRE(age.on_earth() == doctest::Approx(31.69).epsilon(accuracy));
}

TEST_CASE("age_in_mercury_years")
{
    const space_age::space_age age(2'134'835'688);

    REQUIRE(age.on_earth() == doctest::Approx(67.65).epsilon(accuracy));
    REQUIRE(age.on_mercury() == doctest::Approx(280.88).epsilon(accuracy));
}

TEST_CASE("age_in_venus_years")
{
    const space_age::space_age age(189'839'836);

    REQUIRE(age.on_earth() == doctest::Approx(6.02).epsilon(accuracy));
    REQUIRE(age.on_venus() == doctest::Approx(9.78).epsilon(accuracy));
}

TEST_CASE("age_in_mars_years")
{
    const space_age::space_age age(2'329'871'239);

    REQUIRE(age.on_earth() == doctest::Approx(73.83).epsilon(accuracy));
    REQUIRE(age.on_mars() == doctest::Approx(39.25).epsilon(accuracy));
}

TEST_CASE("age_in_jupiter_years")
{
    const space_age::space_age age(901'876'382);

    REQUIRE(age.on_earth() == doctest::Approx(28.58).epsilon(accuracy));
    REQUIRE(age.on_jupiter() == doctest::Approx(2.41).epsilon(accuracy));
}

TEST_CASE("age_in_saturn_years")
{
    const space_age::space_age age(3'000'000'000);

    REQUIRE(age.on_earth() == doctest::Approx(95.06).epsilon(accuracy));
    REQUIRE(age.on_saturn() == doctest::Approx(3.23).epsilon(accuracy));
}

TEST_CASE("age_in_uranus_years")
{
    const space_age::space_age age(3'210'123'456);

    REQUIRE(age.on_earth() == doctest::Approx(101.72).epsilon(accuracy));
    REQUIRE(age.on_uranus() == doctest::Approx(1.21).epsilon(accuracy));
}

TEST_CASE("age_in_neptune_year")
{
    const space_age::space_age age(8'210'123'456);

    REQUIRE(age.on_earth() == doctest::Approx(260.16).epsilon(accuracy));
    REQUIRE(age.on_neptune() == doctest::Approx(1.58).epsilon(accuracy));
}
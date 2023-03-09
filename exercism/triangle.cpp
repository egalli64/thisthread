/*
    Exercism C++ track
    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism
    Triangle https://exercism.org/tracks/cpp/exercises/triangle/

    Determine if a triangle is equilateral, isosceles, or scalene.

    By the Triangle Inequality:
        all sides have to be of length > 0
        the lengths of any two sides must be greater than or equal to the length of the third side
 */
#include <stdexcept>
#include "triangle.h"

namespace triangle
{
    flavor kind(double a, double b, double c)
    {
        if (a <= 0 || b <= 0 || c <= 0 || a > b + c || b > a + c || c > a + b)
        {
            throw std::domain_error("Illegal input data");
        }
        if (a == b && a == c && b == c)
        {
            return flavor::equilateral;
        }
        if (a == b || a == c || b == c)
        {
            return flavor::isosceles;
        }

        return flavor::scalene;
    }
}

// ---
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("equilateral_triangles_have_equal_sides")
{
    REQUIRE(triangle::flavor::equilateral == triangle::kind(2, 2, 2));
}

TEST_CASE("larger_equilateral_triangles_also_have_equal_sides")
{
    REQUIRE(triangle::flavor::equilateral == triangle::kind(10, 10, 10));
}

TEST_CASE("isosceles_triangles_have_last_two_sides_equal")
{
    REQUIRE(triangle::flavor::isosceles == triangle::kind(3, 4, 4));
}

TEST_CASE("isosceles_triangles_have_first_and_last_sides_equal")
{
    REQUIRE(triangle::flavor::isosceles == triangle::kind(4, 3, 4));
}

TEST_CASE("isosceles_triangles_have_first_two_sides_equal")
{
    REQUIRE(triangle::flavor::isosceles == triangle::kind(4, 4, 3));
}

TEST_CASE("isosceles_triangles_have_in_fact_exactly_two_sides_equal")
{
    REQUIRE(triangle::flavor::isosceles == triangle::kind(10, 10, 2));
}

TEST_CASE("scalene_triangles_have_no_equal_sides")
{
    REQUIRE(triangle::flavor::scalene == triangle::kind(3, 4, 5));
}

TEST_CASE("scalene_triangles_have_no_equal_sides_at_a_larger_scale_too")
{
    REQUIRE(triangle::flavor::scalene == triangle::kind(10, 11, 12));
}

TEST_CASE("scalene_triangles_have_no_equal_sides_in_descending_order_either")
{
    REQUIRE(triangle::flavor::scalene == triangle::kind(5, 4, 2));
}

TEST_CASE("very_small_triangles_are_legal")
{
    REQUIRE(triangle::flavor::scalene == triangle::kind(0.4, 0.6, 0.3));
}

TEST_CASE("triangles_with_no_size_are_illegal")
{
    REQUIRE_THROWS_AS(triangle::kind(0, 0, 0), std::domain_error);
}

TEST_CASE("triangles_with_negative_sides_are_illegal")
{
    REQUIRE_THROWS_AS(triangle::kind(3, 4, -5), std::domain_error);
}

TEST_CASE("triangles_violating_triangle_inequality_are_illegal")
{
    REQUIRE_THROWS_AS(triangle::kind(1, 1, 3), std::domain_error);
}

TEST_CASE("larger_triangles_violating_triangle_inequality_are_illegal")
{
    REQUIRE_THROWS_AS(triangle::kind(7, 3, 2), std::domain_error);
}

TEST_CASE("double_and_integer_arguments")
{
    REQUIRE(triangle::flavor::scalene == triangle::kind(5.5, 4, 2));
}

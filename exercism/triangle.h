/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Triangle https://exercism.org/tracks/cpp/exercises/triangle/
 */
#pragma once

namespace triangle
{
    enum class flavor
    {
        equilateral,
        isosceles,
        scalene
    };

    flavor kind(double a, double b, double c);
}
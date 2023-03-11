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
#pragma once
#include <string>

namespace raindrops
{
    std::string convert(int input);
}

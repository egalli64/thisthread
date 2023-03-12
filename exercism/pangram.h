/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Pangram https://exercism.org/tracks/cpp/exercises/pangram/

    Given a string, return true if it is a pangram
    - all letters in [a..z] should be present (low or upper case)
 */
#pragma once
#include <string>

namespace pangram
{
    bool is_pangram(const std::string& s);
}

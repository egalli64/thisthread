/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Two Fer https://exercism.org/tracks/cpp/exercises/two_fer/

    Given a string, use it as a filler for "One for {}, one for me."
    - if no string is passed, use "you" as default
 */
#pragma once
#include <string>

namespace two_fer
{
    std::string two_fer(const std::string& s = "");
}

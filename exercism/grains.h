/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Grains https://exercism.org/tracks/cpp/exercises/grains/

    Calculate the number of grains of wheat on a chessboard given that the number on each square doubles.
    - how many grains were on a given square, and
    - the total number of grains on the chessboard
 */
#pragma once

namespace grains
{
    unsigned long long square(int cell);

    unsigned long long total();
}

/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 4 - Operators and enumerated types

    My repository: https://github.com/egalli64/thisthread

    2.1.7 Enumerated types

    Operators and enum
 */
#include <iostream>

enum weekday
{
    SUN,
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT
};

weekday operator+(weekday day, int days)
{
    int next = (static_cast<int>(day) + days) % 7;
    return static_cast<weekday>(next);
}

std::ostream &operator<<(std::ostream &os, weekday day)
{
    switch (day)
    {
    case SUN:
        return os << "SUNDAY";
    case MON:
        return os << "MONDAY";
    case TUE:
        return os << "TUESDAY";
    case WED:
        return os << "WEDNESDAY";
    case THU:
        return os << "THURSDAY";
    case FRI:
        return os << "FRIDAY";
    case SAT:
        return os << "SATURDAY";
    default:
        return os << "Somewhere inside the depths of time...";
    }
}

int main()
{
    weekday d = SAT;

    d = d + 9;
    std::cout << d << '\n';
}

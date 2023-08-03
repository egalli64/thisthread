/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    1.1.2 Defining class hierarchy

    Private vs public subclasses
 */
#include "1_1_1.hpp"
#include <iostream>

class SubHas : Super
{
};

class SubIs : public Super
{
};

int main()
{
    SubHas sh;
    // won't compile: inaccessible
    // sh.put(100);

    SubIs si;
    si.put(100);
    si.put(si.get() + 1);
    std::cout << si.get() << '\n';
}

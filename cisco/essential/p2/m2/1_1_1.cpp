/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    1.1.1 Defining class hierarchy

    A base class
 */
#include "1_1_1.hpp"
#include <iostream>

int main()
{
    Super object;

    object.put(100);
    object.put(object.get() + 1);
    std::cout << object.get() << '\n';
}

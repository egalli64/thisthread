/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 3 - Exceptions

    My repository: https://github.com/egalli64/thisthread

    1.1.1 To err is human

    A (maybe) crashing code with Arithmetic exception (divide by zero)
 */
#include <cstdlib>
#include <iostream>

int main()
{
    srand(43);
    for (int i = 0; i < 10; ++i)
    {
        int a = rand() % 10;
        int b = rand() % 10;
        std::cout << a << " / " << b << " is ... " << std::flush;
        std::cout << a / b << '\n';
    }
}

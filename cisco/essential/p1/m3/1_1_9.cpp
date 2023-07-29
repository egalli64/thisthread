/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus

    My repository: https://github.com/egalli64/thisthread

    1.1.9 Pointers: another kind of data in the C++ language

    sizeof for basic types
 */
#include <iostream>

int main()
{
    std::cout << "This computing environment uses:\n";
    std::cout << sizeof(char) << " byte for chars\n";
    std::cout << sizeof(short int) << " bytes for shorts\n";
    std::cout << sizeof(int) << " bytes for ints\n";
    std::cout << sizeof(long int) << " bytes for longs\n";
    std::cout << sizeof(float) << " bytes for floats\n";
    std::cout << sizeof(double) << " bytes for doubles\n";
    std::cout << sizeof(bool) << " byte for bools\n";
    std::cout << sizeof(void *) << " bytes for pointers\n";
}

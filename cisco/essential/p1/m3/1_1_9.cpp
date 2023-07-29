/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus

    My repository: https://github.com/egalli64/thisthread

    1.1.9 Pointers: another kind of data in the C++ language

    sizeof for basic types
 */
#include <iostream>

using namespace std;

int main()
{
    cout << "This computing environment uses:\n";
    cout << sizeof(char) << " byte for chars\n";
    cout << sizeof(short int) << " bytes for shorts\n";
    cout << sizeof(int) << " bytes for ints\n";
    cout << sizeof(long int) << " bytes for longs\n";
    cout << sizeof(float) << " bytes for floats\n";
    cout << sizeof(double) << " bytes for doubles\n";
    cout << sizeof(bool) << " byte for bools\n";
    cout << sizeof(void *) << " bytes for pointers\n";
}
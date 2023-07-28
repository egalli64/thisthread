/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus

    My repository: https://github.com/egalli64/thisthread

    2.1.5 More types and when we use them

    floating point precision
 */
#include <iomanip>
#include <iostream>

int main()
{
    double a = 0.1;
    double b = 0.2;
    double c = 0.3;

    std::cout << a + b << " == " << c << '\n';
    std::cout << std::fixed << std::setprecision(17) << a + b << " == " << c << '\n';
}

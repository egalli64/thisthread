/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 1, module 1 - Introduction to computer programming, variables, comments, basic I/O operations, flow control (if)

    My repository: https://github.com/egalli64/thisthread

    1.7.1.11 Connecting with the real world: input and output

    cin & cout
 */
#include <iostream>

int main()
{
    int value;

    std::cout << "Give me a number and I will square it!\n";
    std::cin >> value;

    int square = value * value;
    std::cout << "You've given " << value << '\n';
    std::cout << "The squared value is " << square << '\n';
}
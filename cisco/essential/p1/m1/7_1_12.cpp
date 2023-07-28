/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus

    My repository: https://github.com/egalli64/thisthread

    1.7.1.12 Connecting with the real world: input and output

    cin & cout /2
 */
#include <cmath>
#include <iostream>

int main()
{
    double value;

    std::cout << "Give me a number and I will find its square root:\n";
    std::cin >> value;
    if (value >= 0.0)
    {
        double result = std::sqrt(value);
        std::cout << "You have given: " << value << '\n';
        std::cout << "The square root is: " << result << '\n';
    }
    else
    {
        std::cout << "I don't know how to extract square root from " << value << '\n';
    }
}
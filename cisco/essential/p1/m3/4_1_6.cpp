/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus

    My repository: https://github.com/egalli64/thisthread

    4.1.6 Declaring and defining functions

    an example
 */
#include <iostream>

float f_to_c(float temp)
{
    return ((temp - 32.0) * 5.0) / 9.0;
}

void tester(float temp)
{
    std::cout << "Fahrenheit " << temp << " corresponds to " << f_to_c(temp) << " Centigrade\n";
}

int main()
{
    tester(32.0);
    tester(212.0);
    tester(451.0);
}
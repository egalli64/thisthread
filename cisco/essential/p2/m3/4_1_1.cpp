/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 3 - Exceptions

    My repository: https://github.com/egalli64/thisthread

    4.1.1 Catching exceptions

    catch all
 */
#include <iostream>
#include <stdexcept>

void function(int i)
{
    switch (i)
    {
    case 0:
        throw std::out_of_range("0");
    case 1:
        throw std::overflow_error("1");
    case 2:
        throw std::domain_error("2");
    }
}

int main()
{
    for (int i = 0; i < 3; i++)
    {
        try
        {
            function(i);
        }
        catch (...)
        {
            std::cout << "Exception caught!\n";
        }
    }
}

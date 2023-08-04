/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 3 - Exceptions

    My repository: https://github.com/egalli64/thisthread

    4.1.2 Catching exceptions

    multi-catch
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
    case 3:
        throw std::exception();
    }
}

int main()
{
    for (int i = 0; i < 4; i++)
    {
        try
        {
            function(i);
        }
        catch (std::out_of_range &ex)
        {
            std::cout << "Out of range: " << ex.what() << '\n';
        }
        catch (std::overflow_error &ex)
        {
            std::cout << "Overflow: " << ex.what() << '\n';
        }
        catch (std::domain_error &ex)
        {
            std::cout << "Domain: " << ex.what() << '\n';
        }
        catch (std::exception &ex)
        {
            std::cout << "Exception: " << ex.what() << '\n';
        }
    }
}

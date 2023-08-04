/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 3 - Exceptions

    My repository: https://github.com/egalli64/thisthread

    4.1.2 Catching exceptions

    rethrow
 */
#include <iostream>
#include <stdexcept>

void function(int i)
{
    switch (i)
    {
    case 0:
        throw std::domain_error("0");
    case 1:
        throw std::logic_error("1");
    case 2:
        throw std::exception();
    case 3:
        throw std::range_error("2");
    case 4:
        throw "so bad";
    }
}

void broker(int i)
{
    try
    {
        function(i);
    }
    catch (...)
    {
        std::cout << "Broker swept problems under the carpet\n";
        throw;
    }
}

int main()
{
    for (int i = 0; i < 5; i++)
    {
        try
        {
            broker(i);
        }
        catch (std::logic_error &le)
        {
            std::cout << "Logic error: " << le.what() << '\n';
        }
        catch (std::runtime_error &re)
        {
            std::cout << "Runtime error: " << re.what() << '\n';
        }
        catch (std::exception &ex)
        {
            std::cout << "Exception: " << ex.what() << '\n';
        }
        catch (...)
        {
            std::cout << "Something bad happened" << '\n';
        }
    }
}

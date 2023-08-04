/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 3 - Exceptions

    My repository: https://github.com/egalli64/thisthread

    2.1.5 Throw statement in detail

    noexcept
 */
#include <iostream>
#include <string>

class MyException
{
  public:
    std::string msg;
    MyException(const std::string &msg) : msg(msg)
    {
    }
};

void thrower() noexcept(false)
{
    std::cout << "Thrower\n";
    throw MyException("object");
}

void putter() noexcept // or: noexcept(true)
{
    std::cout << "Putter\n";
}

int main()
{
    try
    {
        putter();
        thrower();
    }
    catch (MyException &exc)
    {
        std::cout << "Caught!\n";
    }
}

/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    5.1.11 Various supplements

    Copy ctor and composition
 */
#include <iostream>

class A
{
  public:
    A(A &src)
    {
        std::cout << "A copy ctor\n";
    }

    A()
    {
        std::cout << "Default A ctor\n";
    }

    void do_it()
    {
        std::cout << "A is doing something\n";
    }
};

class B
{
  public:
    B(B &src)
    {
        std::cout << "B copy ctor\n";
    }

    B()
    {
        std::cout << "Default B ctor\n";
    }

    void do_it()
    {
        std::cout << "B is doing something\n";
    }
};

class Compo
{
  public:
    Compo(Compo &other) : f1(other.f1), f2(other.f2)
    {
        std::cout << "Compo copy ctor\n";
    }

    Compo()
    {
        std::cout << "Default Compo ctor\n";
    };

    A f1;
    B f2;
};

int main()
{
    Compo co1;

    std::cout << "Calling copy ctor for Compo ...\n";
    Compo co2 = co1;

    co2.f1.do_it();
    co2.f2.do_it();
}
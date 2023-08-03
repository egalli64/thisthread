/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    2.1.2 Classes, inheritance and type compatibility

    A non-polimorphic base class
 */
#pragma once
#include <iostream>

class Pet
{
  protected:
    std::string name;

  public:
    Pet(const std::string& name)
    {
        this->name = name;
    }
    void run()
    {
        std::cout << name << ": I'm running\n";
    }
};

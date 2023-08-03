/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    1.1.1 Defining class hierarchy

    A base class
 */
#pragma once

class Super
{
  private:
    int storage;

  public:
    void put(int val)
    {
        storage = val;
    }
    int get()
    {
        return storage;
    }
};

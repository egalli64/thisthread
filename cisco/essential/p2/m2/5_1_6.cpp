/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    5.1.6 Various supplements

    No copy ctor
 */
#include <iostream>

class Dummy
{
  private:
    // old-style disallowing of copy ctor, relying on private visibility
    // Dummy(Dummy &)
    // {
    // }
  public:
    Dummy(int value)
    {
    }
    
    // copy ctor explicitely deleted from interface
    Dummy(const Dummy &) = delete;
};

// !! WARNING - Can't create a copy of Dummy !!
void do_something(Dummy ob)
{
    std::cout << "I'm here!\n";
}

int main()
{
    Dummy o1(123);
    // won't compile, use of deleted function
    // Dummy o2 = o1;

    // won't compile, use of deleted function
    // do_something(o1);
}

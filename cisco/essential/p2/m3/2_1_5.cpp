/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 3 - Exceptions

    My repository: https://github.com/egalli64/thisthread

    2.1.5 Throw statement in detail

    Life cycle of a thrown object
 */
#include <iostream>

class MyException
{
  private:
    std::string msg;

  public:
    MyException(const std::string &msg) : msg(msg)
    {
        std::cout << "Ctor for [" << msg << "]\n";
    }

    ~MyException()
    {
        std::cout << "Dtor for [" << msg << "]\n";
    }

    void hello()
    {
        std::cout << "Hello [" << msg << "]\n";
    }
};

void do_calculations(int i)
{
    if (i == 0)
        throw MyException("exception 1");
    MyException object("object");
    if (i == 1)
        throw MyException("exception 2");
    object.hello();
    if (i == 2)
        throw MyException("exception 3");
}

int main()
{
    for (int i = 0; i < 3; i++)
    {
        try
        {
            std::cout << "-------\n";
            do_calculations(i);
        }
        catch (MyException &exc)
        {
            std::cout << "Caught!\n";
            exc.hello();
        }
    }
}

/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    1.1.4 Defining class hierarchy

    Access protected in class
 */
#include <iostream>

class Super
{
  protected:
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

class Sub : public Super
{
  public:
    void print()
    {
        std::cout << "storage = " << storage << '\n';
    }
};

int main()
{
    Sub object;

    object.put(100);
    object.put(object.get() + 1);
    object.print();
}

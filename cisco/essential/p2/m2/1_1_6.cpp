/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    1.1.6 Defining class hierarchy

    Multiple Inheritance
 */
#include <iostream>

class SuperA
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

class SuperB
{
  protected:
    int safe;

  public:
    void insert(int val)
    {
        safe = val;
    }
    int takeout()
    {
        return safe;
    }
};

class Sub : public SuperA, public SuperB
{
  public:
    void print()
    {
        std::cout << "storage = " << storage << '\n';
        std::cout << "safe    = " << safe << '\n';
    }
};

int main()
{
    Sub object;

    object.put(1);
    object.insert(2);
    object.put(object.get() + object.takeout());
    object.insert(object.get() + object.takeout());
    object.print();
}
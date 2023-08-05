/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 4 - Operators and enumerated types

    My repository: https://github.com/egalli64/thisthread

    1.1.5 Overloading operators – the basics

    Operators as function members
 */
#include <iostream>

class Stack
{
  private:
    int *store;
    int cur;

  public:
    Stack(int size = 100)
    {
        store = new int[size];
        cur = 0;
    }

    ~Stack()
    {
        delete[] store;
    }

    void push(int value)
    {
        store[cur++] = value;
    }

    int pop()
    {
        return store[--cur];
    }

    Stack &operator<<(int v)
    {
        push(v);
        return *this;
    }

    Stack &operator>>(int &v)
    {
        v = pop();
        return *this;
    }
};

int main()
{
    Stack stack;

    int i = 2;
    stack << 1 << 2 * i;

    int j;
    stack >> j >> i;
    std::cout << j << ' ' << i << '\n';
}

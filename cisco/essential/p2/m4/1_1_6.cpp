/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 4 - Operators and enumerated types

    My repository: https://github.com/egalli64/thisthread

    1.1.6 Overloading operators – the basics

    Operators as free functions
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
};

Stack &operator<<(Stack &s, int v)
{
    s.push(v);
    return s;
}

Stack &operator>>(Stack &s, int &v)
{
    v = s.pop();
    return s;
}

int main()
{
    Stack stack;

    int i = 2;
    stack << 1 << 2 * i;

    int j;
    stack >> j >> i;

    std::cout << j << ' ' << i << '\n';
}
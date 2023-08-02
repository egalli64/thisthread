/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 1 - Object-oriented programming essentials (classes, objects, inheritance)

    My repository: https://github.com/egalli64/thisthread

    2.1.12 A stack: a view from two different perspectives

    an OO stack (non polymorphic/private derivation) hierarchy
 */
#include <iostream>

class Stack
{
  private:
    static constexpr int max_size = 100;
    int data[max_size];
    int stack_pointer;

  public:
    Stack()
    {
        stack_pointer = 0;
    }

    void push(int value)
    {
        data[stack_pointer] = value;
        stack_pointer += 1;
    }

    int pop()
    {
        stack_pointer -= 1;
        return data[stack_pointer];
    }
};

class AddingStack : Stack
{
  private:
    int sum;

  public:
    AddingStack() : Stack(), sum(0)
    {
    }

    void push(int value)
    {
        sum += value;
        Stack::push(value);
    }

    int pop()
    {
        int val = Stack::pop();
        sum -= val;
        return val;
    }

    int get_sum()
    {
        return sum;
    }
};

int main()
{
    AddingStack stack;

    for (int i = 1; i < 10; i++)
    {
        stack.push(i);
    }

    std::cout << "Current stack total: " << stack.get_sum() << '\n';
    for (int i = 1; i < 10; i++)
    {
        stack.pop();
    }

    std::cout << "Current stack total: "  << stack.get_sum() << '\n';
}

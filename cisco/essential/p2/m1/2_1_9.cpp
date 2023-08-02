/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 1 - Object-oriented programming essentials (classes, objects, inheritance)

    My repository: https://github.com/egalli64/thisthread

    2.1.9 A stack: a view from two different perspectives

    an OO stack
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
        std::cout << "Pushing " << value << " in position " << stack_pointer << '\n';
        data[stack_pointer] = value;
        stack_pointer += 1;
    }

    int pop()
    {
        stack_pointer -= 1;
        return data[stack_pointer];
    }
};

int main()
{
    Stack stack;
    stack.push(3);
    stack.push(2);
    stack.push(1);
    std::cout << "Pop: " << stack.pop() << '\n';
    std::cout << "Pop: " << stack.pop() << '\n';
    std::cout << "Pop: " << stack.pop() << '\n';
}

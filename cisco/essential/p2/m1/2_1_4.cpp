/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 1 - Object-oriented programming essentials (classes, objects, inheritance)

    My repository: https://github.com/egalli64/thisthread

    2.1.4 A stack: a view from two different perspectives

    a procedural stack
 */
#include <iostream>

namespace
{
constexpr int max_size = 100;
int stack[max_size];
int stack_pointer = 0;
} // namespace

void push(int value)
{
    std::cout << "Pushing " << value << " in position " << stack_pointer << '\n';
    stack[stack_pointer] = value;
    stack_pointer += 1;
}

int pop()
{
    stack_pointer -= 1;
    return stack[stack_pointer];
}

int main()
{
    push(3);
    push(2);
    push(1);
    std::cout << "Pop: " << pop() << '\n';
    std::cout << "Pop: " << pop() << '\n';
    std::cout << "Pop: " << pop() << '\n';
}

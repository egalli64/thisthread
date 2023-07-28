/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus

    My repository: https://github.com/egalli64/thisthread

     1.7.1.4 Connecting with the real world: input and output

     I/O manipulator
 */
#include <iostream>

int main()
{
    int value = 255;
    std::cout << "A value: " << value << '\n';
    std::cout << "Hex format: " << std::hex << value << '\n';
    std::cout << "Hex format is still valid: " << value << '\n';
    std::cout << "Switch back to decimal: " << std::dec << value << '\n';
    std::cout << "Oct format: " << std::oct << value << '\n';
}
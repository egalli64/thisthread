/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus

    My repository: https://github.com/egalli64/thisthread

    1.3.1 Integer values, integer variables and comments
    1. Numbers and how computers see them
 */
#include <iostream>

int main()
{
    // since C++17 we can use ' to improve numbers readability
    std::cout << "Eleven millions ... " << 11'111'111 << '\n';

    // octal representation
    std::cout << "Octal 123 is decimal 83: " << 0123 << '\n';

    // octal representation
    std::cout << "Hexadecimal 123 is decimal 291: " << 0x123 << '\n';

    // binary representation
    std::cout << "Binary 1111 is decimal 15: " << 0b1111 << '\n';
}

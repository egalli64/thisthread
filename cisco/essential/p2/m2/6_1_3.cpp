/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    6.1.3 The const keyword

    const and pointers
 */
#include <iostream>

int main()
{
    int data[]{1, 2, 3, 4, 5};

    int *const cpi = data + 2;
    std::cout << "The third element of data, " << data[2] << ", is pointed by cpi, " << *cpi << '\n';
    *cpi += 10;
    // cpi += 1;
    std::cout << "cpi is a constant pointer to int, can't change, but its pointed can: " << *cpi << '\n';

    const int * pic = data + 2;
    std::cout << "The third element of data, " << data[2] << ", is pointed by pic, " << *pic << '\n';
    // *pic += 10;
    pic += 1;
    std::cout << "pic is a pointer to constant int, it can change, but its pointed can't: " << *pic << '\n';

    
    const int * const cpic = data + 2;
    std::cout << "The third element of data, " << data[2] << ", is pointed by cpic, " << *cpic << '\n';
    // *cpic += 10;
    // cpic += 1;
    std::cout << "cpic is a constant pointer to constant int, it can't change, as its pointed: " << *cpic << '\n';
}
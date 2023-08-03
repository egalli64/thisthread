/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    2.1.2 Classes, inheritance and type compatibility

    A non-polimorphic hierarchy
 */
#include "2_1_2.hpp"
#include <iostream>
#include <string>

class Dog : public Pet
{
  public:
    Dog(const std::string &name) : Pet(name){};
    void make_sound()
    {
        std::cout << name << ": Woof! Woof!\n";
    }
};

class Cat : public Pet
{
  public:
    Cat(const std::string &name) : Pet(name){};
    void make_sound()
    {
        std::cout << name << ": Meow! Meow!\n";
    }
};

int main()
{
    Pet a_pet("A pet");
    Cat a_cat("Tom");
    Dog a_dog("Spike");

    a_pet.run();
    a_dog.run();
    a_dog.make_sound();
    a_cat.run();
    a_cat.make_sound();
}

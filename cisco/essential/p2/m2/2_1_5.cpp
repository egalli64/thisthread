/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    2.1.5 Classes, inheritance and type compatibility

    Upcast and downcast
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
    Pet *a_pet1 = new Cat("Tom");
    Pet *a_pet2 = new Dog("Spike");

    a_pet1->run();
    static_cast<Cat *>(a_pet1)->make_sound();
    a_pet2->run();
    static_cast<Dog *>(a_pet2)->make_sound();
}

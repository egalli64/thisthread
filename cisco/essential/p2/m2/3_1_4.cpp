/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    3.1.4 Polymorphism and virtual methods

    A polymorphic hierarchy
 */
#include <iostream>
#include <string>

class Pet
{
  protected:
    std::string name;

  public:
    Pet(const std::string& name)
    {
        this->name = name;
        // !!! WARNING - calling a virtual method on a (possibly) partially constructed object !!!
        make_sound();
    }
    virtual void make_sound()
    {
        std::cout << name << " the Pet says: Shh! Shh!\n";
    }
};

class Cat : public Pet
{
  public:
    Cat(const std::string& n) : Pet(n)
    {
    }
    void make_sound()
    {
        std::cout << name << " the Cat says: Meow! Meow!\n";
    }
};

class Dog : public Pet
{
  public:
    Dog(const std::string& n) : Pet(n)
    {
    }
    void make_sound()
    {
        std::cout << name << " the Dog says: Woof! Woof!\n";
    }
};

int main()
{
    Cat *a_cat = new Cat("Kitty");
    Dog *a_dog = new Dog("Doggie");

    a_cat->make_sound();
    a_dog->make_sound();
}

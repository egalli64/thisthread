/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    4.1.2 Objects as parameters and dynamic casting

    Passing an object as a function parameter
 */
#include <iostream>

class Pet
{
  protected:
    std::string name;

  public:
    void name_me(const std::string &name)
    {
        this->name = name;
    }
    void make_sound()
    {
        std::cout << name << " says: no comments\n";
    }
};

void pet_by_pointer(const std::string &name, Pet *pet)
{
    pet->name_me(name);
    pet->make_sound();
}

void pet_by_reference(const std::string &name, Pet &pet)
{
    pet.name_me(name);
    pet.make_sound();
}

// !! WARNING do we really want a _copy_ of pet in here? !!
void pet_by_value(const std::string &name, Pet pet)
{
    pet.name_me(name);
    pet.make_sound();
}

int main()
{
    Pet pet;

    pet.name_me("no_name");
    pet_by_value("Alpha", pet);
    pet.make_sound();
    pet_by_pointer("Beta", &pet);
    pet.make_sound();
    pet_by_reference("Gamma", pet);
    pet.make_sound();
}

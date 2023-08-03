/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    4.1.7 Objects as parameters and dynamic casting

    dynamic_casting for references
 */
#include <iostream>

class Pet
{
  protected:
    std::string name;

  public:
    Pet(std::string name) : name(name)
    {
    }
    virtual void make_sound()
    {
        std::cout << name << " is silent ._.\n";
    }
};

class Dog : public Pet
{
  public:
    Dog(std::string name) : Pet(name)
    {
    }
    void make_sound()
    {
        std::cout << name << " says: Woof!\n";
    }
};

class GermanShepherd : public Dog
{
  public:
    GermanShepherd(std::string name) : Dog(name)
    {
    }
    void make_sound()
    {
        std::cout << name << " says: Wuff!\n";
    }
    void laufen()
    {
        std::cout << name << " runs (shepherd)!\n";
    }
};

class MastinEspanol : public Dog
{
  public:
    MastinEspanol(std::string name) : Dog(name)
    {
    }
    void make_sound()
    {
        std::cout << name << " says: Guau!\n";
    }
    void ejecutar()
    {
        std::cout << name << " runs (mastin)!\n";
    }
};

void play_with_pet(Pet &pet)
{
    pet.make_sound();
    try
    {
        dynamic_cast<GermanShepherd &>(pet).laufen();
    }
    catch (std::bad_cast &ex)
    {
        std::cout << "Can't downcast to German Shepherd\n";
    }

    try
    {
        dynamic_cast<MastinEspanol &>(pet).ejecutar();
    }
    catch (std::bad_cast &ex)
    {
        std::cout << "Can't downcast to Mastin Espanol\n";
    }
}

int main()
{
    Pet pet("creature");
    Dog dog("Dog");
    GermanShepherd shepherd("Hund");
    MastinEspanol mastin("Perro");

    play_with_pet(pet);
    play_with_pet(dog);
    play_with_pet(shepherd);
    play_with_pet(mastin);
}
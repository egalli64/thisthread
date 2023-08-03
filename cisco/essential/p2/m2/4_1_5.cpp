/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    4.1.5 Objects as parameters and dynamic casting

    dynamic_casting
 */
#include <iostream>

class Pet
{
  protected:
    std::string name;

  public:
    Pet(const std::string &name) : name(name)
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
    Dog(const std::string& name) : Pet(name)
    {
    }

    void make_sound() override
    {
        std::cout << name << " says: Woof!\n";
    }
};

class GermanShepherd : public Dog
{
  public:
    GermanShepherd(const std::string& name) : Dog(name)
    {
    }

    void make_sound() override
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
    MastinEspanol(const std::string& name) : Dog(name)
    {
    }

    void make_sound() override
    {
        std::cout << name << " says: Guau!\n";
    }

    void ejecutar()
    {
        std::cout << name << " runs (mastin)!\n";
    }
};

void play_with_pet(Pet *pet)
{
    pet->make_sound();
    GermanShepherd *shepherd = dynamic_cast<GermanShepherd *>(pet);
    if (shepherd != nullptr)
        shepherd->laufen();
    MastinEspanol *mastin = dynamic_cast<MastinEspanol *>(pet);
    if (mastin != nullptr)
        mastin->ejecutar();
}

int main()
{
    Pet *pet = new Pet("creature");
    Pet *dog = new Dog("Dog");
    Pet *shepherd = new GermanShepherd("Hund");
    Pet *mastin = new MastinEspanol("Perro");

    play_with_pet(pet);
    play_with_pet(dog);
    play_with_pet(shepherd);
    play_with_pet(mastin);
}

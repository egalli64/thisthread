/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus
    Part 2, module 2 - Inheritance

    My repository: https://github.com/egalli64/thisthread

    5.1.4 Various supplements

    Copy ctor
 */
#include <iostream>

class Class
{
    int *data;

  public:
    Class(int value)
    {
        data = new int;
        *data = value;
        std::cout << "Ctor " << value << " to " << data << '\n';
    }

    Class(const Class &other)
    {
        data = new int;
        *data = *(other.data);
        std::cout << "Copy ctor " << *data << " to " << data << '\n';
    }

    Class &operator=(const Class &other)
    {
        data = other.data;
        std::cout << "Assignment in " << data << '\n';
        return *this;
    }

    void increment()
    {
        (*data)++;
    }

    int value() const
    {
        return *data;
    }

    ~Class()
    {
        std::cout << "Deleting " << data << '\n';
        delete data;
    }
};

int main()
{
    Class o1(123);
    Class o2 = o1;
    Class o3(o2);

    o1.increment();

    std::cout << o1.value() << '\n';
    std::cout << o2.value() << '\n';
    std::cout << o3.value() << '\n';
}

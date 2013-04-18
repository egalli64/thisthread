/**
 * main4test.cpp
 * More info on http://thisthread.blogspot.com/2013/04/redis-client-testing.html
 */

#include <iostream>
#include "Tester.h"

int main(int argc, char* argv[])
{
    Tester(argc, argv).run();

    std::cout << "Prerequiste: Redis server should run on localhost, default port" << std::endl;
}

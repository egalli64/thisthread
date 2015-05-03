/*
 * ceKnightMoves.cpp
 *
 *  Created on: May 3, 2015
 *      Author: manny
 *  https://www.codeeval.com/open_challenges/180/
 */

#include <string>

namespace
{

std::string solution(const std::string& input)
{
    if (input.size() != 2 || input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8')
        return {};

    std::string result;

    auto push_back = [&result] (char x, char y) {
        result.push_back(x);
        result.push_back(y);
        result.push_back(' ');
    };

    auto add = [&](char x, int step)
    {
        if(input[1] > '0' + step)
            push_back(x, input[1] - step);
        if(input[1] < '9' - step)
            push_back(x, input[1] + step);
    };

    char x = input[0];
    if (x > 'b')
        add(x - 2, 1);
    if (x > 'a')
        add(x - 1, 2);
    if (x < 'h')
        add(x + 1, 2);
    if (x < 'g')
        add(x + 2, 1);

    return result;
}

}

#include <gtest/gtest.h>

TEST(Ce18, Given1)
{
    std::string input { "g2" };
    std::string expct { "e1 e3 f4 h4 " };
    ASSERT_EQ(expct, solution(input));
}

TEST(Ce18, Given2)
{
    std::string input { "a1" };
    std::string expct { "b3 c2 " };
    ASSERT_EQ(expct, solution(input));
}

TEST(Ce18, Given3)
{
    std::string input { "d6" };
    std::string expct { "b5 b7 c4 c8 e4 e8 f5 f7 " };
    ASSERT_EQ(expct, solution(input));
}

TEST(Ce18, Given4)
{
    std::string input { "e5" };
    std::string expct { "c4 c6 d3 d7 f3 f7 g4 g6 " };
    ASSERT_EQ(expct, solution(input));
}

TEST(Ce18, Given5)
{
    std::string input { "b1" };
    std::string expct { "a3 c3 d2 " };
    ASSERT_EQ(expct, solution(input));
}

/*
 #include <iostream>
 #include <fstream>

 int main(int argc, char *argv[])
 {
 std::ifstream stream(argv[1]);
 std::string line;
 while (std::getline(stream, line))
 {
 std::cout << solution(line) << std::endl;
 }
 }
 */

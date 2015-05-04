/*
 * ceDetails.cpp
 *
 *  Created on: May 4, 2015
 *      Author: manny
 *  https://www.codeeval.com/open_challenges/183/
 */

#include <string>
#include <vector>
#include <iostream>

namespace
{

std::vector<std::string> split(const std::string& input, char sep)
{
    std::vector<std::string> tokens;
    std::size_t beg = 0, end = 0;
    while ((end = input.find(sep, beg)) != std::string::npos)
    {
        tokens.push_back(input.substr(beg, end - beg));
        beg = end + 1;
    }
    tokens.push_back(input.substr(beg));

    return tokens;
}

int solution(const std::string& input)
{
    std::vector<std::string> tokens = split(input, ',');
    int dist = input.size() - 2;
    for (std::string s : tokens)
    {
        int gap = s.find('Y') - s.rfind('X') - 1;
        if (gap < dist)
            dist = gap;
    }

    return dist;
}

}

#include <gtest/gtest.h>

TEST(Ce183, Given1)
{
    std::string input { "XX.YY,XXX.Y,X..YY,XX..Y" };
    ASSERT_EQ(1, solution(input));
}

TEST(Ce183, Given2)
{
    std::string input { "XXX.YYYY,X...Y..Y,XX..YYYY,X.....YY,XX....YY" };
    ASSERT_EQ(1, solution(input));
}

TEST(Ce183, Given3)
{
    std::string input { "XX...YY,X....YY,XX..YYY,X..YYYY" };
    ASSERT_EQ(2, solution(input));
}

TEST(Ce183, Given4)
{
    std::string input { "XXYY,X..Y,XX.Y" };
    ASSERT_EQ(0, solution(input));
}

/*
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::ifstream stream(argv[1]);
    std::string line;
    while (std::getline(stream, line)) {
        std::cout << solution(line) << std::endl;
    }
}
 */

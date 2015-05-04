/*
 * ceMaxRangeSum.cpp
 *
 *  Created on: May 4, 2015
 *      Author: manny
 *  https://www.codeeval.com/open_challenges/186/
 */

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <numeric>

namespace
{

typedef std::vector<int> Values;

int maxRangeSum(const Values& values, unsigned days)
{
    if(values.size() < days)
        return 0;

    int tentative = std::accumulate(values.begin(), values.begin() + days, 0);
    int result = tentative > 0 ? tentative : 0;

    for(std::size_t i = 0; i < values.size() - days; ++i)
    {
        tentative -= values[i];
        tentative += values[i + days];
        if(tentative > result)
            result = tentative;
    }

    return result;
}

int solution(const std::string& input)
{
    std::istringstream iss(input);
    unsigned days;
    iss >> days;
    iss.ignore(1);

    int value;
    Values values;
    while(iss >> value)
        values.push_back(value);

    return maxRangeSum(values, days);
}

}

#include <gtest/gtest.h>

TEST(Ce186, Given1)
{
    std::string input { "5;7 -3 -10 4 2 8 -2 4 -5 -2" };
    ASSERT_EQ(16, solution(input));
}

TEST(Ce186, Given2)
{
    std::string input { "6;-4 3 -10 5 3 -7 -3 7 -6 3" };
    ASSERT_EQ(0, solution(input));
}

TEST(Ce186, Given3)
{
    std::string input { "3;-7 0 -45 34 -24 7" };
    ASSERT_EQ(17, solution(input));
}

/*
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::ifstream stream(argv[1]);
    std::string line;
    while (std::getline(stream, line))
        std::cout << solution(line) << std::endl;
}
 */

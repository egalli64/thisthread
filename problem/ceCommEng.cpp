/*
 * ceCommEng.cpp
 *
 * My C++11 solution to the CodeEval Commuting Engineer problem
 * Written and tested on GCC 4.8 - currently won't work on CodeEval
 *
 * More info: http://thisthread.blogspot.com/2013/10/commuting-engineer-codeeval-problem.html
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

using Position = std::pair<double, double>;
using Location = std::pair<int, Position>;
using Locations = std::vector<Location>;

Location parse(const std::string& input)
{
    int nr = std::stoi(input);

    std::string::size_type bracket = input.find('(');
    if(bracket == std::string::npos)
        return {};

    std::string::size_type comma = input.find(',', bracket);
    if(comma == std::string::npos)
        return {};

    double lon = std::stod(input.substr(bracket + 1));
    double lat = std::stod(input.substr(comma + 1));
    return { nr, {lon, lat}};
}

double distance(const Position& beg, const Position& end)
{
    return std::sqrt(std::pow(beg.first - end.first, 2) + pow(beg.second - end.second, 2));
}

Locations::iterator findClosest(Position& beg, Locations& others)
{
    double closest = std::numeric_limits<double>::max();

    Locations::iterator pos = others.end();
    for(Locations::iterator it = others.begin(); it != others.end(); ++it)
    {
        double current = distance(beg, it->second);
        if(current < closest)
        {
            closest = current;
            pos = it;
        }
    }

    return pos;
}

std::vector<int> getPath(const Locations& input)
{
    if(input.empty())
        return {};

    Locations locations(input);
    std::vector<int> results;
    Locations::iterator current = locations.begin();
    do {
        Position curPos = current->second;
        results.push_back(current->first);
        locations.erase(current);

        current = findClosest(curPos, locations);

    } while(!locations.empty());

    return results;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Missing input file name in input" << std::endl;
        return 1;
    }

    std::ifstream ifs(argv[1]);
    std::string input;
    Locations locations;
    while(std::getline(ifs, input))
    {
        if(!input.empty())
            locations.push_back(parse(input));
    }

    std::vector<int> results = getPath(locations);
    std::for_each(results.begin(), results.end(), [](int result){
        std::cout << result << ' ';
    });
    std::cout << std::endl;

    return 0;
}

#include <gtest/gtest.h>

TEST(CommEng, Parse1)
{
    std::string input("1 | CodeEval 1355 Market St, SF (37.7768016, -122.4169151)");
    Location loc = parse(input);

    EXPECT_EQ(1, loc.first);
    EXPECT_DOUBLE_EQ(37.7768016, loc.second.first);
    EXPECT_DOUBLE_EQ(-122.4169151, loc.second.second);
}

TEST(CommEng, Test0)
{
    Locations locations;
    std::vector<int> results = getPath(locations);

    ASSERT_TRUE(results.empty());
}

TEST(CommEng, Test1)
{
    Locations locations {
        parse("1 | CodeEval 1355 Market St, SF (37.7768016, -122.4169151)")
    };

    std::vector<int> results = getPath(locations);

    ASSERT_EQ(1, results.size());
    ASSERT_EQ(1, results[0]);
}

TEST(CommEng, Test2)
{
    Locations locations {
        parse("1 | CodeEval 1355 Market St, SF (37.7768016, -122.4169151)"),
        parse("2 | Yelp 706 Mission St, SF (37.7860105, -122.4025377)")
    };

    std::vector<int> results = getPath(locations);

    ASSERT_EQ(2, results.size());
    ASSERT_EQ(1, results[0]);
    ASSERT_EQ(2, results[1]);
}

TEST(CommEng, Test3)
{
    Locations locations {
        parse("1 | CodeEval 1355 Market St, SF (37.7768016, -122.4169151)"),
        parse("2 | Yelp 706 Mission St, SF (37.7860105, -122.4025377)"),
        parse("3 | Square 110 5th St, SF (37.7821494, -122.4058960)")
    };

    std::vector<int> results = getPath(locations);

    ASSERT_EQ(3, results.size());
    ASSERT_EQ(1, results[0]);
    ASSERT_EQ(3, results[1]);
    ASSERT_EQ(2, results[2]);
}

TEST(CommEng, Full)
{
    Locations locations {
        parse("1 | CodeEval 1355 Market St, SF (37.7768016, -122.4169151)"),
        parse("2 | Yelp 706 Mission St, SF (37.7860105, -122.4025377)"),
        parse("3 | Square 110 5th St, SF (37.7821494, -122.4058960)"),
        parse("4 | Airbnb 99 Rhode Island St, SF (37.7689269, -122.4029053)"),
        parse("5 | Dropbox 185 Berry St, SF (37.7768800, -122.3911496)"),
        parse("6 | Zynga 699 8th St, SF (37.7706628, -122.4040139)"),
        parse("7 | Mashery 717 Market St, SF (37.7870361, -122.4039444)"),
        parse("8 | Flurry 3060 3rd St, SF (37.7507903, -122.3877184)"),
        parse("9 | New Relic 188 Spear St, SF (37.7914417, -122.3927229)"),
        parse("10 | Glassdoor 1 Harbor Drive, Sausalito (37.8672841, -122.5010216)")
    };

    ASSERT_EQ(10, locations.size());

    std::vector<int> results = getPath(locations);
    ASSERT_EQ(10, results.size());

    std::for_each(results.begin(), results.end(), [](int result){
        std::cout << result << ' ';
    });
    std::cout << std::endl;

    ASSERT_EQ(1, results[0]);
    ASSERT_EQ(3, results[1]);
    ASSERT_EQ(2, results[2]);
    ASSERT_EQ(7, results[3]);
    ASSERT_EQ(9, results[4]);
    ASSERT_EQ(5, results[5]);
    ASSERT_EQ(4, results[6]);
    ASSERT_EQ(6, results[7]);
    ASSERT_EQ(8, results[8]);
    ASSERT_EQ(10, results[9]);
}

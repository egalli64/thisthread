/*
 * cj101aa.cpp
 *
 * http://code.google.com/codejam/contest/544101/dashboard#s=p0
 * Code Jam Round 1A 2010 Problem A. Rotate
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <stdexcept>

namespace
{
const char* results[] { "Neither", "Red", "Blue", "Both", "Unexpected" };
enum Results { Neither, Red, Blue, Both, Unexpected };
const int maxSize = 50;
}

using Board = std::array<std::array<char, maxSize>, maxSize>;
using Input = std::vector<std::string>;

class Rotator
{
private:
    Board board_ {};
    const unsigned size_;
    const unsigned len_;
    bool blue_ = false;
    bool red_ = false;

    bool consistency(const Input& input);

    bool checkRight(unsigned row, unsigned col);
    bool checkDown(unsigned row, unsigned col);
    bool checkRightUp(unsigned row, unsigned col);
    bool checkRightDown(unsigned row, unsigned col);

public:
    Rotator(unsigned size, unsigned len, const Input& input);
    std::string winner();
};

bool Rotator::consistency(const Input& input)
{
    if(size_ > maxSize || input.size() != size_)
        return false;

    for(auto it = input.begin(); it != input.end(); ++it)
        if(it->size() != size_)
            return false;

    return true;
}

bool Rotator::checkRight(unsigned row, unsigned col)
{
    if(col + len_ > size_)
        return false;

    for(unsigned k = 1; k < len_; ++k)
        if(board_[row][col] != board_[row][col + k])
            return false;

    board_[row][col] == 'R' ? red_ = true : blue_ = true;
    return true;
}

bool Rotator::checkDown(unsigned row, unsigned col)
{
    if(row + len_ > size_)
        return false;

    for(unsigned k = 1; k < len_; ++k)
        if(board_[row][col] != board_[row + k][col])
            return false;

    board_[row][col] == 'R' ? red_ = true : blue_ = true;
    return true;
}

bool Rotator::checkRightUp(unsigned row, unsigned col)
{
    if(row < len_ -1 || col + len_ > size_)
        return false;

    for(unsigned k = 1; k < len_; ++k)
        if(board_[row][col] != board_[row - k][col + k])
            return false;

    board_[row][col] == 'R' ? red_ = true : blue_ = true;
    return true;
}

bool Rotator::checkRightDown(unsigned row, unsigned col)
{
    if(row + len_ > size_ || col + len_ > size_)
        return false;

    for(unsigned k = 1; k < len_; ++k)
        if(board_[row][col] != board_[row + k][col + k])
            return false;

    board_[row][col] == 'R' ? red_ = true : blue_ = true;
    return true;
}

Rotator::Rotator(unsigned size, unsigned len, const Input& input) : size_(size), len_(len)
{
    if(!consistency(input))
        throw std::logic_error("Bad input");

    for(unsigned y = 0; y < size_; ++y)
    {
        unsigned iBoard = size_ - 1;
        for(unsigned x = 0; x < size_; ++x)
        {
            unsigned iInput = size_ - 1 - x;
            if(input[y][iInput] != '.')
                board_[y][iBoard--] = input[y][iInput];
        }
    }
}

std::string Rotator::winner()
{
    for(unsigned row = 0; row < size_ && (!blue_ || !red_); ++row)
    {
        for(unsigned col = 0; col < size_ && (!blue_ || !red_); ++col)
        {
            if((board_[row][col] == 'R' && !red_) || (board_[row][col] == 'B' && !blue_))
            {
                if(checkRight(row, col))
                    continue;
                if(checkDown(row, col))
                    continue;
                if(checkRightUp(row, col))
                    continue;
                checkRightDown(row, col);
            }
        }
    }

    return results[blue_*2 + red_];
}


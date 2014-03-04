/**
 * ita363.cpp
 *
 * Feb 28, 2014
 * egalli
 */

#include <vector>
#include <algorithm>

typedef std::vector<int> Vector;

// recursive top-down
unsigned cutRodRecursive(const Vector& price, unsigned size)
{
  unsigned result = 0;
  for(unsigned i = 0; i < size; ++i)
    result = std::max(result, price[i] + cutRodRecursive(price, size - (i+1)));

  return result;
}

// memoized top-down
unsigned memoCutRod(const Vector& price, unsigned size, Vector& memo)
{
  if(memo[size] >= 0)
    return memo[size];

  unsigned result = 0;
  for(unsigned i = 0; i < size; ++i)
    result = std::max(result, price[i] + memoCutRod(price, size - (i+1), memo));

  return memo[size] = result;
}

unsigned cutRodMemoized(const Vector& price, unsigned size)
{
  Vector memo(size + 1, -1);
  memo[0] = 0; // dummy

  return memoCutRod(price, size, memo);
}

// memoized bottom-up
unsigned cutRod(const Vector& price, unsigned size)
{
  Vector memo(size + 1);
  for(unsigned i = 1; i <= size; ++i)
  {
    int value = -1;
    for(unsigned j = 0; j < i; ++j)
      value = std::max(value, price[j] + memo[i-j-1]);
    memo[i] = value;
  }

  return memo.back();
}

#include <gtest/gtest.h>

TEST(CutRod, Simple)
{
  Vector price { 1, 5, 8, 9, 10, 17, 17, 20, 24, 30 };

  // best cut: 1 5 8 10 13 17 18 22 25 30
  ASSERT_EQ(30, cutRodRecursive(price, 10));
  ASSERT_EQ(30, cutRodMemoized(price, 10));
  ASSERT_EQ(30, cutRod(price, 10));
  ASSERT_EQ(25, cutRod(price, 9));
  ASSERT_EQ(18, cutRod(price, 7));
  ASSERT_EQ(10, cutRod(price, 4));
}

/**
 * info: http://thisthread.blogspot.com/2014/02/maximum-subarray-by-divide-and-conquer.html
 *       http://thisthread.blogspot.com/2014/02/maximum-subarray-by-kadane.html
 */

#include <vector>
#include <array>
#include <limits>

typedef std::array<int, 3> Info;
typedef std::vector<int> Vector;

Info maxCrossing(const Vector& data, int left, int middle, int right)
{
  int sum = 0;

  int maxLeft = middle;
  int leftSum = std::numeric_limits<int>::min();
  for(int i = middle; i >= left; --i)
  {
    sum += data[i];
    if(sum > leftSum)
    {
      leftSum = sum;
      maxLeft = i;
    }
  }

  sum = 0;
  int maxRight = middle + 1;
  int rightSum = std::numeric_limits<int>::min();
  for(int i = middle + 1; i <= right; ++i)
  {
    sum += data[i];
    if(sum > rightSum)
    {
      rightSum = sum;
      maxRight = i;
    }
  }

  return {{ maxLeft, maxRight, leftSum + rightSum }};
}

Info max(const Info& a, const Info& b, const Info& c)
{
  if(a[2] > b[2] && a[2] > c[2])
    return a;
  return b[2] > c[2] ? b : c;
}

Info maxSubArray(const Vector& data, int left, int right)
{
  if(left == right)
    return {{ left, right, data[left] }};

  int middle = (left + right) / 2;

  Info subLeft = maxSubArray(data, left, middle);
  Info subRight = maxSubArray(data, middle + 1, right);
  Info crossing = maxCrossing(data, left, middle, right);

  return max(subLeft, subRight, crossing);
}

int maxSubAr(const Vector& data)
{
  int sum = 0;
  int sumTmp = 0;

  for(unsigned i = 0; i < data.size(); ++i)
  {
    if(int value = sumTmp + data[i] > 0)
      sumTmp = value;
    else
      sumTmp = 0;

    if(sumTmp > sum)
      sum = sumTmp;
  }

  return sum;
}

Info maxSubArray(const Vector& data)
{
  int left = 0;
  int right = 0;
  int sum = 0;

  int leftTmp = 0;
  int sumTmp = 0;

  for(unsigned i = 0; i < data.size(); ++i)
  {
    int value = sumTmp + data[i];
    if(value > 0)
    {
      if(sumTmp == 0)
        leftTmp = i;
      sumTmp = value;
    }
    else
      sumTmp = 0;

    if(sumTmp > sum)
    {
      left = leftTmp;
      right = i;
      sum = sumTmp;
    }
  }

  return {{ left, right, sum }};
}

#include <gtest/gtest.h>

TEST(MaxCrossing, Simple)
{
  Vector input { 2, 3, 4, 5, 7 };

  unsigned last = input.size() - 1;
  Info sub = maxCrossing(input, 0, last / 2, last);
  EXPECT_EQ(0, sub[0]);
  EXPECT_EQ(last, sub[1]);
  EXPECT_EQ(21, sub[2]);
}

TEST(MaxCrossing, Simple2)
{
  Vector input {-2, -5, 6, -2, -3, 1, 5, -6};

  unsigned last = input.size() - 1;
  Info sub = maxCrossing(input, 0, last / 2, last);
  EXPECT_EQ(2, sub[0]);
  EXPECT_EQ(6, sub[1]);
  EXPECT_EQ(7, sub[2]);
}

TEST(MaxSub, Simple)
{
  Vector input { 2, 3, 4, 5, 7 };

  unsigned last = input.size() - 1;
  Info sub = maxSubArray(input, 0, last);
  EXPECT_EQ(0, sub[0]);
  EXPECT_EQ(last, sub[1]);
  EXPECT_EQ(21, sub[2]);
}

TEST(MaxSub, Simple2)
{
  Vector input {-2, -5, 6, -2, -3, 1, 5, -6};

  Info sub = maxSubArray(input, 0, input.size() - 1);
  EXPECT_EQ(2, sub[0]);
  EXPECT_EQ(6, sub[1]);
  EXPECT_EQ(7, sub[2]);
}

TEST(MaxSub, Negative)
{
  Vector input {-2, -5, -2, -3, -6};

  Info sub = maxSubArray(input);
  EXPECT_EQ(0, sub[0]);
  EXPECT_EQ(0, sub[1]);
  EXPECT_EQ(0, sub[2]);
}

TEST(MaxSubLin, Simple)
{
  Vector input { 2, 3, 4, 5, 7 };

  unsigned last = input.size() - 1;
  Info sub = maxSubArray(input);
  EXPECT_EQ(0, sub[0]);
  EXPECT_EQ(last, sub[1]);
  EXPECT_EQ(21, sub[2]);
}

TEST(MaxSubLin, Simple2)
{
  Vector input {-2, -5, 6, -2, -3, 1, 5, -6};

  Info sub = maxSubArray(input);
  EXPECT_EQ(2, sub[0]);
  EXPECT_EQ(6, sub[1]);
  EXPECT_EQ(7, sub[2]);
}

TEST(MaxSubX, Simple)
{
  EXPECT_EQ(21, maxSubAr({ 2, 3, 4, 5, 7 }));
}

TEST(MaxSubX, Simple2)
{
  EXPECT_EQ(7, maxSubAr({-2, -5, 6, -2, -3, 1, 5, -6}));
}

TEST(MaxSubX, SimpleSecond)
{
  EXPECT_EQ(7, maxSubAr({3, -5, 6, 1, -3}));
}

TEST(MaxSubX, AnotherSecond)
{
  EXPECT_EQ(12, maxSubAr({3, -1, 2, -6, 12}));
}


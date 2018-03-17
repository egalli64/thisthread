/**
 * Given an integer n, return the sum of the integers from 1 to n (inclusive)
 *
 * author: Manny egalli64@gmail.com
 * info: http://thisthread.blogspot.com/
 *		 https://stepik.org/lesson/26055/step/2?unit=8087
 * 1. Introduction and review - the fuss of C++
 */
#include "gtest/gtest.h"

namespace
{
	int addNumsNaive(int n)
	{
		int result = 0;
		for (int i = 0; i <= n; i++)
			result += i;
		return result;
	}

	int addNums(int n)
	{
		return n * (n + 1) / 2;
	}
}

TEST(Test141Naive, Provided)
{
	EXPECT_EQ(55, addNumsNaive(10));
}

TEST(Test141Naive, Eleven)
{
	EXPECT_EQ(66, addNumsNaive(11));
}

TEST(Test141, Provided)
{
	EXPECT_EQ(55, addNums(10));
}

TEST(Test141, Eleven)
{
	EXPECT_EQ(66, addNums(11));
}

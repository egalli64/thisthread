/**
 * Given an integer n, create a new vector, add all of the numbers from 1 to n (inclusive) to the vector,
 * and then return the vector
 *
 * author: Manny egalli64@gmail.com
 * info: http://thisthread.blogspot.com/
 *		https://stepik.org/lesson/26055/step/11
 * 1. Introduction and review - the fuss of C++
 */
#include "gtest/gtest.h"
#include <iostream>
#include <numeric>

namespace
{
	std::vector<int> createVectorA(int n) {
		std::vector<int> result;
		result.reserve(n);
		for (int i = 1; i <= n; ++i)
			result.push_back(i);

		return result;
	}

	std::vector<int> createVectorB(int n) {
		std::vector<int> result(n);
		std::iota(result.begin(), result.end(), 1);
		return result;
	}

}

TEST(Test142A, Provided)
{
	std::vector<int> v = createVectorA(10);
	EXPECT_EQ(10, v.size());
	for(size_t i = 0; i < v.size(); ++i)
		EXPECT_EQ(i+1, v[i]);
}

TEST(Test142B, Provided)
{
	std::vector<int> v = createVectorB(10);
	EXPECT_EQ(10, v.size());
	for (size_t i = 0; i < v.size(); ++i)
		EXPECT_EQ(i + 1, v[i]);
}


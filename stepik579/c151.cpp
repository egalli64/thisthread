/**
 * 1. Introduction and review - random numbers
 *
 * author: Manny egalli64@gmail.com
 * info: http://thisthread.blogspot.com/
 *		 https://stepik.org/lesson/Random-Numbers-31402/step/7
 */
#include "gtest/gtest.h"
#include <numeric>
#include <cmath>
#include <random>

namespace
{
	unsigned int flipCoin()
	{
		return rand() % 2;
	}

	unsigned int rollDie(unsigned int k)
	{
		return 1 + rand() % k;
	}

	/**
	 * biased coin that outputs 1 with probability p and 0 with probability 1–p
	 */
	unsigned int flipCoin(float p)
	{
		return static_cast<float>(rand()) / RAND_MAX < p ? 1 : 0;
	}
}

TEST(Test151, flipCoinBiased)
{
	std::vector<bool> results;
	results.reserve(10000);
	for (int i = 0; i < 10000; ++i)
	{
		results.push_back(flipCoin(0.7F));
	}

	int total = std::accumulate(results.begin(), results.end(), 0);

	EXPECT_TRUE(std::abs(7000 - total) < 50);
}



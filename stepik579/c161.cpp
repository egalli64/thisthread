/**
 * 1. Introduction and review - bit by bit
 *
 * author: Manny egalli64@gmail.com
 * info: http://thisthread.blogspot.com/
 *		 https://stepik.org/lesson/27076/step/12
 */
#include "gtest/gtest.h"

namespace
{
	// 7 | (~125 << 3)
	uint8_t ex5()
	{
		uint8_t a = 7;
		uint8_t b = 125;
		b = ~b;
		b <<= 3;
		return a | b;
	}
}

TEST(Test161, exercise5)
{
	EXPECT_EQ(23, ex5());
}

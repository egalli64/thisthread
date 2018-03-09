/**
 * ASIO adding new asynchronous wait on timer with a free function
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-passing-parameters-to-handler.html
 * Based on: Timer.3 - Binding arguments to a handler
 *           http://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/tutorial/tuttimer3.html
 *
 * expires_at() - no param - is deprecated, use expiry() instead
 */
#include <iostream>
#include <functional>
#include <boost/asio/system_timer.hpp>
#include "basic_skills.h"

namespace
{
	void print(ba::system_timer* pTimer, int* pCount) {
		if (*pCount < 5)
		{
			std::cout << (*pCount)++ << ' ';
			pTimer->expires_at(pTimer->expiry() + sc::milliseconds(500));
			pTimer->async_wait(std::bind(print, pTimer, pCount));
		}
	}
}

void timer3(ba::io_context& io)
{
	std::cout << "3) Starting ... " << std::flush;

	int count = 0;
	ba::system_timer timer(io, sc::milliseconds(500));

	timer.async_wait(std::bind(print, &timer, &count));
	io.run();

	std::cout << "final count is " << count << std::endl;
}

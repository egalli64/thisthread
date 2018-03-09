/**
 * ASIO adding new asynchronous wait on timer with a lambda
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-passing-parameters-to-handler.html
 * Based on: Timer.3 - Binding arguments to a handler
 *           http://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/tutorial/tuttimer3.html
 *
 * expires_at() deprecated, use expiry() instead
 */
#include <iostream>
#include <functional>
#include <boost/asio/system_timer.hpp>
#include "basic_skills.h"

void timer3a(ba::io_context& io)
{
	std::cout << "3a) Starting ... " << std::flush;

	int count = 0;
	ba::system_timer timer(io, sc::milliseconds(500));

	std::function<void(const bs::error_code&)> print = [&](const bs::error_code&) {
		if (count < 5)
		{
			std::cout << count++ << ' ';
			timer.expires_at(timer.expiry() + sc::milliseconds(500));
			timer.async_wait(print);
		}
	};

	timer.async_wait(print);
	io.run();

	std::cout << "final count is " << count << std::endl;
}

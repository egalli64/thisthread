/**
 * ASIO system_timer on std chrono to asynch wait for a sec
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-asynchronous-wait-on-timer.html
 * Based on: Timer.2 - Using a timer asynchronously
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tuttimer2.html
 */

#include <iostream>
#include <boost/asio/system_timer.hpp>
#include "basic_skills.h"

namespace
{
	void hello(const bs::error_code& ec)
	{
		std::cout << "delayed hello [" << ec.value() << "] " << std::flush;
	}
}

void timer2(ba::io_context& io)
{
	std::cout << "2) Starting ... " << std::flush;

	ba::system_timer timer{ io, sc::seconds(1) };
	timer.async_wait(hello);
	std::cout << "hello " << std::flush;

	io.run();
	std::cout << "done" << std::endl;
}

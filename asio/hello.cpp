/**
 * ASIO system_timer on std chrono to synch wait for a sec
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-basic-skills.html
 * Based on: Timer.1 - Using a timer synchronously
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tuttimer1.html
 */
#include <iostream>
#include <boost/asio/system_timer.hpp>
#include "basic_skills.h"

void timer1(ba::io_context& io)
{
	std::cout << "1) Starting ... " << std::flush;

	ba::system_timer timer{ io, sc::seconds(1) };
	timer.wait();

	std::cout << "done!" << std::endl;
}

/**
 * main for ASIO Basic Skills examples
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-basic-skills.html
 * Based on: Boost ASIO Tutorial "Basic Skills"
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial.html
 */
#include <iostream>
#include "basic_skills.h"

int main()
{
	std::cout << "Starting ASIO Basic Skills suite" << std::endl;
	ba::io_context io;

	timer1(io);
	assert(!io.stopped());

	timer2(io);
	assert(io.stopped());

	io.reset();
	timer2a(io);
	assert(io.stopped());

	io.reset();
	timer3(io);
	assert(io.stopped());

	io.reset();
	timer3a(io);
	assert(io.stopped());

	io.reset();
	timer4(io);
	assert(io.stopped());

	io.reset();
	timer5(io);
	assert(io.stopped());

	std::cout << "ASIO Basic Skills suite completed" << std::endl;
}

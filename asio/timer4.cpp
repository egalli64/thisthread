/**
 * ASIO adding new asynchronous wait on timer through class
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-using-member-function-as.html
 * Based on: Timer.4 - Using a member function as a handler
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tuttimer4.html
 */
#include <iostream>
#include <functional>
#include <boost/asio/system_timer.hpp>
#include "basic_skills.h"

namespace
{
	class Printer
	{
	private:
		ba::system_timer timer_;
		int count_;
	public:
		Printer(ba::io_context& io) : timer_(io, sc::milliseconds(500)), count_(0)
		{
			timer_.async_wait(std::bind(&Printer::print, this));
		}

		~Printer()
		{
			std::cout << "final count is " << count_ << std::endl;
		}

		void print()
		{
			if (count_ < 5)
			{
				std::cout << count_++ << ' ';
				timer_.expires_at(timer_.expiry() + sc::milliseconds(500));
				timer_.async_wait(std::bind(&Printer::print, this));
			}
		}
	};
}

void timer4(ba::io_context& io)
{
	std::cout << "4) Starting ... " << std::flush;
	Printer printer(io);
	io.run();
}
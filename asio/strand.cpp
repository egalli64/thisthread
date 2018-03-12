/**
 * ASIO strand to synchronise
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-strand-example.html
 * Based on: Timer.5 - Synchronising handlers in multithreaded programs
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tuttimer5.html
 */
#include <iostream>
#include <functional>
#include <thread>
#include <boost/asio/io_context_strand.hpp>
#include "basic_skills.h"
#include <boost/asio.hpp>

namespace
{
	class Printer
	{
	private:
		ba::io_context::strand strand_;
		int count_;

		void print(const char* msg)
		{
			std::cout << '{' << std::this_thread::get_id() << "} " << msg << ": " << count_ << " " << std::flush;
		}

		void print1()
		{
			if (count_ > 0)
			{
				print("one");
				--count_;

				strand_.post(std::bind(&Printer::print1, this));
			}
		}

		void print2()
		{
			if (count_ > 0)
			{
				print("two");
				--count_;
				strand_.post(std::bind(&Printer::print2, this));
			}
		}

	public:
		Printer(ba::io_context& io, int count) : strand_(io), count_(count)
		{
			strand_.post(std::bind(&Printer::print1, this));
			strand_.post(std::bind(&Printer::print2, this));
		}

		~Printer()
		{
			print("dtor");
			std::cout << std::endl;
		}
	};
}

void timer5(ba::io_context& io)
{
	std::cout << "5) Starting ... " << std::flush;

	Printer printer(io, 10);

	// boost bind approach
	//std::thread thread(boost::bind(&ba::io_context::run, &io));

	// std bind approach
	// error C2672: 'std::bind': no matching overloaded function found
	// error C2783: 'std::_Binder<std::_Unforced,_Fx,_Types...> std::bind(_Fx &&,_Types &&...)': could not deduce template argument for '_Fx'
	//std::thread thread(std::bind(&ba::io_context::run, &io));

	// std bind patched approach
	//auto run = static_cast<ba::io_context::count_type(ba::io_service::*)()>(&ba::io_context::run);
	//std::thread thread(std::bind(run, &io));

	// lambda approach
	std::thread thread([&io] { io.run(); });

	io.run();
	thread.join();
}

/**
 * ASIO system_timer on std chrono to asynch wait for a sec while executing another task
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-asynchronous-wait-on-timer.html
 * Based on: Timer.2 - Using a timer asynchronously
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tuttimer2.html
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <boost/asio/system_timer.hpp>

#include "basic_skills.h"

namespace
{
	class MyJob
	{
	private:
		MyJob(const MyJob&) = delete;
		const MyJob& operator=(const MyJob&) = delete;

		std::mutex mx_;
		std::atomic<bool> expired_;
	public:
		MyJob() : expired_(false) {}

		void log(const char* message)
		{
			std::unique_lock<std::mutex> lock(mx_);
			std::cout << message << std::flush;
		}

		void timeout()
		{
			expired_ = true;
			log("Timeout!\n");
		}

		void operator()()
		{
			for (int i = 0; !expired_; ++i)
			{
				std::ostringstream os;
				os << '[' << i << ']';

				log(os.str().c_str());
				std::this_thread::sleep_for(sc::milliseconds(300));
			}
		}
	};
}

void timer2a(ba::io_context& io)
{
	std::cout << "2a) Starting ... " << std::flush;

	ba::system_timer timer(io, sc::seconds(1));

	MyJob job;
	timer.async_wait([&job](const bs::error_code&) { job.timeout(); });
	std::thread thread(std::ref(job));

	io.run();
	thread.join();
}

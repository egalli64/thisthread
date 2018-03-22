/**
 * Boost ASIO UDP asynchronous server
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-udp-asynchronous-server.html
 * Based on: Daytime.6 - An asynchronous UDP daytime server
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tutdaytime6.html
 */
#include <iostream>
#include <memory>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/write.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
using ba::ip::udp;

namespace
{
	const int HELLO_PORT = 50'013;
	const std::string MSG("Async UDP hello from ASIO ");

	class Server
	{
	private:
		udp::socket socket_;
		udp::endpoint endpoint_;
		uint16_t counter_ = 0;

		void start()
		{
			std::array<char, 0> buffer;
			socket_.async_receive_from(ba::buffer(buffer), endpoint_,
				std::bind(&Server::recvHandler, this, std::placeholders::_1));
			std::cout << "Server ready" << std::endl;
		}

		void recvHandler(const bs::error_code& error)
		{
			if (!error)
			{
				std::shared_ptr<std::string> data(new std::string(MSG + std::to_string(++counter_)));

				socket_.async_send_to(ba::buffer(*data), endpoint_,
					std::bind(&Server::sendHandler, this, data, std::placeholders::_1, std::placeholders::_2));
				start();
			}
		}

		void sendHandler(std::shared_ptr<std::string> data, const bs::error_code& error, std::size_t size)
		{
			if (!error)
			{
				std::cout << size << " byte sent from [" << *data << "]" << std::endl;
			}
		}
	public:
		Server(ba::io_context& io) : socket_(io, udp::endpoint(udp::v6(), HELLO_PORT))
		{
			start();
		}
	};
}

int main()
{
	ba::io_context io;
	try
	{
		Server server(io);
		// here you may want to run() io on other threads
		io.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

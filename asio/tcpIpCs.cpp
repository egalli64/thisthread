/**
 * Boost ASIO TCP/IP synchronous client-server
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-synchronous-exchange-on-tcpip.html
 * Based on: Daytime.1 - A synchronous TCP daytime client
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tutdaytime1.html
 *           Daytime.2 - A synchronous TCP daytime server
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tutdaytime2.html
 */
#include <iostream>
#include <exception>
#include <array>
#include <string>
#include <boost/asio.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
using ba::ip::tcp;

namespace
{
	const unsigned short HELLO_PORT = 50013;
	const std::string HELLO_PORT_STR{ "50013" };
	const std::string HOSTNAME{ "localhost" };

	void server(ba::io_context& io)
	{
		try
		{
			tcp::acceptor acceptor{ io, tcp::endpoint{tcp::v6(), HELLO_PORT} };

			// just once
			{
				tcp::socket socket{ io };
				std::cout << "Server ready" << std::endl;
				acceptor.accept(socket);

				std::string message{ "Hello from ASIO" };
				bs::error_code ec; // ignored
				ba::write(socket, ba::buffer(message), ec);
			}
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << std::endl;
		}
	}

	void client(ba::io_context& io, const std::string& host)
	{
		try
		{
			tcp::resolver resolver{ io };
			tcp::resolver::results_type endpoints = resolver.resolve(host, HELLO_PORT_STR);

			tcp::socket socket{ io };
			ba::connect(socket, endpoints);

			for (;;)
			{
				std::array<char, 4> buf;
				bs::error_code error;
				size_t len = socket.read_some(ba::buffer(buf), error);

				if (error == ba::error::eof)
					break; // Connection closed cleanly by peer
				else if (error)
					throw bs::system_error(error);

				std::cout.write(buf.data(), len);
				std::cout << '|';
			}
			std::cout << std::endl;
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	ba::io_context io;

	if (argc > 1)
		client(io, HOSTNAME);
	else
		server(io);
}

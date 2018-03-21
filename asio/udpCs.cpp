/**
 * Boost ASIO UDP synchronous client-server
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-synchronous-udp-clientserver.html
 * Based on: Daytime.4 - A synchronous UDP daytime client
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tutdaytime4.html
 *           Daytime.5 - A synchronous UDP daytime server
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tutdaytime5.html
 */
#include <iostream>
#include <exception>
#include <array>
#include <string>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/io_context.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
using ba::ip::udp;

namespace
{
	const unsigned short HELLO_PORT = 50'013;
	const std::string HELLO_PORT_STR{ "50013" };
	const std::string HOSTNAME{ "localhost" };

	void server(ba::io_context& io)
	{
		try
		{
			udp::socket socket{ io, udp::endpoint{udp::v6(), HELLO_PORT} };
			std::cout << "Server ready" << std::endl;

			// for (;;)
			{
				std::array<char, 0> recvData;
				udp::endpoint endpoint;
				bs::error_code error;
				socket.receive_from(ba::buffer(recvData), endpoint, 0, error);
				if (error)
					throw bs::system_error(error);

				std::string message{ "UDP hello from ASIO" };

				bs::error_code ec;
				socket.send_to(boost::asio::buffer(message), endpoint, 0, ec);
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
			udp::resolver resolver{ io };
			udp::endpoint destination = *resolver.resolve(udp::v6(), host, HELLO_PORT_STR).begin();

			udp::socket socket{ io };
			socket.open(udp::v6());

			std::array<char, 0> sendData;
			socket.send_to(ba::buffer(sendData), destination);

			std::array<char, 128> recvData;
			udp::endpoint sender;
			size_t len = socket.receive_from(ba::buffer(recvData), sender);

			std::cout.write(recvData.data(), len);
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

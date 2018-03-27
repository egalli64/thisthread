/**
 * ASIO echo UDP synchronous client-server
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-echo-udp-synchronous-client.html
 * Based on: http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/example/cpp11/echo/blocking_udp_echo_client.cpp
 *			 http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/example/cpp11/echo/blocking_udp_echo_server.cpp
 */
#include <cstring>
#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
using ba::ip::udp;

namespace
{
	const int MAX_LEN = 1024;
	const uint16_t ECHO_PORT = 50'015;
	const std::string ECHO_PORT_STR{ std::to_string(ECHO_PORT) };
	const std::string HOSTNAME{ "localhost" };

	void client(ba::io_context& io, const std::string& host)
	{
		try
		{
			std::cout << "Enter message: ";
			char request[MAX_LEN];
			std::cin.getline(request, MAX_LEN);

			udp::socket socket{ io, udp::endpoint(udp::v4(), 0) };
			udp::resolver resolver{ io };
			udp::endpoint destination = *resolver.resolve(udp::v4(), host, ECHO_PORT_STR).begin();
			socket.send_to(ba::buffer(request, std::strlen(request)), destination);

			char reply[MAX_LEN];
			udp::endpoint sender;
			size_t len = socket.receive_from(ba::buffer(reply), sender);
			std::cout << "Reply is: ";
			std::cout.write(reply, len);
			std::cout << std::endl;
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}
	}

	void server(ba::io_context& io)
	{
		try
		{
			udp::socket socket(io, udp::endpoint(udp::v4(), ECHO_PORT));
			std::cout << "Server ready" << std::endl;
			for (;;)
			{
				char data[MAX_LEN];
				udp::endpoint client;
				size_t len = socket.receive_from(ba::buffer(data), client);
				std::cout << "Echoing ";
				std::cout.write(data, len);
				std::cout << std::endl;
				socket.send_to(ba::buffer(data, len), client);
			}
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
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

/**
 * ASIO echo TCP synchronous client-server
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-echo-tcp-synchronous-client.html
 * Based on: http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/example/cpp11/echo/blocking_tcp_echo_client.cpp
 *			 http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/example/cpp11/echo/blocking_tcp_echo_server.cpp
 */
#include <cstring>
#include <iostream>
#include <thread>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
using ba::ip::tcp;

namespace
{
	const int SERVER_MAX_LEN = 2;
	const int CLIENT_MAX_LEN = 1024;
	const int ECHO_PORT = 50'014;
	const std::string ECHO_PORT_STR{ std::to_string(ECHO_PORT) };
	const std::string HOSTNAME{ "localhost" };

	void session(tcp::socket socket)
	{
		std::cout << "Opening session" << std::endl;
		try
		{
			for (;;)
			{
				char data[SERVER_MAX_LEN];

				bs::error_code error;
				size_t len = socket.read_some(ba::buffer(data), error);
				if (error == ba::error::eof)
				{
					std::cout << "Closing session" << std::endl;
					return;
				}
				else if (error)
				{
					throw bs::system_error(error);
				}

				ba::write(socket, ba::buffer(data, len));
			}
		}
		catch (std::exception& e)
		{
			std::cerr << "Session interrupted: " << e.what() << std::endl;
		}
	}

	void server(ba::io_context& io)
	{
		tcp::acceptor acceptor{ io, tcp::endpoint(tcp::v4(), ECHO_PORT) };
		std::cout << "Server ready" << std::endl;

		for (;;)
		{
			std::thread(session, acceptor.accept()).detach();
		}
	}

	void client(ba::io_context& io, const std::string& host)
	{
		try
		{
			tcp::socket socket{ io };
			tcp::resolver resolver{ io };
			ba::connect(socket, resolver.resolve(host, ECHO_PORT_STR));

			std::cout << "Enter message: ";
			char request[CLIENT_MAX_LEN];
			std::cin.getline(request, CLIENT_MAX_LEN);
			size_t reqLen = std::strlen(request);
			ba::write(socket, ba::buffer(request, reqLen));

			char reply[CLIENT_MAX_LEN];
			size_t repLen = ba::read(socket, ba::buffer(reply, reqLen));
			std::cout << "Reply is: ";
			std::cout.write(reply, repLen);
			std::cout << std::endl;
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

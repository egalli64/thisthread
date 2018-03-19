/**
 * Boost ASIO TCP/IP asynchronous server
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-tcpip-asynchronous-server.html
 * Based on: Daytime.3 - An asynchronous TCP daytime server
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial/tutdaytime3.html
 */
#include <iostream>
#include <memory>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
using ba::ip::tcp;

namespace
{
	const int HELLO_PORT = 50013;

	class Connection : public std::enable_shared_from_this<Connection>
	{
	private:
		tcp::socket socket_;
		std::string message_{ "Async hello from ASIO " };
		static int counter_;

		Connection(ba::io_context& io) : socket_(io)
		{
			message_ += std::to_string(counter_++);
		}

		void write(const bs::error_code& ec, size_t size)
		{
			if (!ec)
			{
				std::cout << size << " bytes sent" << std::endl;
			}
		}
	public:
		static std::shared_ptr<Connection> create(ba::io_context& io)
		{
			return std::shared_ptr<Connection>(new Connection(io));
		}

		tcp::socket& socket()
		{
			return socket_;
		}

		void start()
		{
			ba::async_write(socket_, ba::buffer(message_),
				std::bind(&Connection::write, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
		}
	};

	int Connection::counter_ = 0;

	class Server
	{
	private:
		tcp::acceptor acceptor_;

		void start()
		{
			ba::io_context& io = acceptor_.get_executor().context();
			std::shared_ptr<Connection> connection = Connection::create(io);
			tcp::socket& socket = connection->socket();
			acceptor_.async_accept(socket, std::bind(&Server::handle, this, connection, std::placeholders::_1));
		}

		void handle(std::shared_ptr<Connection> connection, const bs::error_code& ec)
		{
			if (!ec)
			{
				connection->start();
			}
			start();
		}
	public:
		Server(ba::io_context& io) : acceptor_(io, tcp::endpoint(tcp::v4(), HELLO_PORT))
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
		std::cout << "Server ready" << std::endl;
		io.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

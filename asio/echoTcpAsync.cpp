/**
 * ASIO echo TCP asynchronous server
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-echo-tcp-asynchronous-server.html
 * Based on: http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/example/cpp11/echo/async_tcp_echo_server.cpp
 */
#include <iostream>
#include <thread>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
using ba::ip::tcp;

namespace
{
	const size_t MAX_LEN = 2;
	const uint16_t ECHO_PORT = 50'014;

	class Session : public std::enable_shared_from_this<Session>
	{
	private:
		tcp::socket socket_;
		char data_[MAX_LEN];

		void write(std::size_t len)
		{
			std::cout << "Sending " << len << " bytes: ";
			std::cout.write(data_, len);
			std::cout << std::endl;

			std::shared_ptr<Session> self{ shared_from_this() };
			ba::async_write(socket_, ba::buffer(data_, len), [self](bs::error_code ec, std::size_t) {
				if (!ec)
				{
					self->read();
				}
			});
		}
	public:
		Session(tcp::socket socket) : socket_(std::move(socket)) {}

		void read()
		{
			std::shared_ptr<Session> self{ shared_from_this() };
			socket_.async_read_some(ba::buffer(data_), [self](bs::error_code ec, std::size_t len) {
				if (!ec)
				{
					self->write(len);
				}
			});
		}
	};

	class Server
	{
	private:
		tcp::acceptor acceptor_;

		void accept()
		{
			std::cout << "Server ready" << std::endl;
			acceptor_.async_accept([this](bs::error_code ec, tcp::socket socket)
			{
				if (!ec)
				{
					std::make_shared<Session>(std::move(socket))->read();
				}
				accept();
			});
		}
	public:
		Server(ba::io_context& io) : acceptor_(io, tcp::endpoint(tcp::v4(), ECHO_PORT))
		{
			accept();
		}
	};
}

int main(int argc, char* argv[])
{
	ba::io_context io;
	try
	{
		Server server(io);
		io.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

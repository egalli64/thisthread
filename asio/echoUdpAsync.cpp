/**
 * ASIO echo UDP asynchronous server
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-echo-udp-asynchronous-server.html
 * Based on: http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/example/cpp11/echo/async_udp_echo_server.cpp
 */
#include <iostream>
#include <functional>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
using ba::ip::udp;

namespace
{
    const int MAX_LEN = 1'024;
    const uint16_t ECHO_PORT = 50'015;

    class Server
    {
    private:
        udp::socket socket_;
        udp::endpoint endpoint_;
        char data_[MAX_LEN];

    public:
        Server(ba::io_context& io) : socket_(io, udp::endpoint(udp::v4(), ECHO_PORT))
        {
            receive();
        }

        void receive()
        {
            std::cout << "Server ready" << std::endl;

            socket_.async_receive_from(ba::buffer(data_), endpoint_,
                [this](bs::error_code ec, std::size_t len)
            {
                if (!ec && len > 0)
                {
                    send(len);
                }
                else
                {
                    std::cout << ec.message() << std::endl;
                    receive();
                }
            });
        }

        void send(std::size_t len)
        {
            socket_.async_send_to(ba::buffer(data_, len), endpoint_, std::bind(&Server::receive, this));
        }
    };
}

int main(int argc, char* argv[])
{
    ba::io_context io;

    try
    {
        Server server{ io };
        io.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

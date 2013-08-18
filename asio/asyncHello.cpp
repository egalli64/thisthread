/**
 * Asynchronous Hello Asio
 *
 * Info: http://thisthread.blogspot.com/2013/08/waiting-asynchronously.html
 */

#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

namespace ba = boost::asio;
namespace sc = std::chrono;

void hello(const boost::system::error_code& ec)
{
    std::cout << "delayed hello [" << ec.value() << ']'  << std::endl;
}

int main()
{
    std::cout << "starting" << std::endl;

    ba::io_service aios;

    ba::steady_timer timer(aios, sc::seconds(1));
    timer.async_wait(hello);
    std::cout << "hello" << std::endl;
    aios.run();

    std::cout << "done" << std::endl;
}

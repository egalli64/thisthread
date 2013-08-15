/**
 * Hello Asio
 *
 * See: http://thisthread.blogspot.com/2013/08/hello-again-boost-asio.html
 */

#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

namespace ba = boost::asio;
namespace sc = std::chrono;

int main()
{
    std::cout << "Starting ... " << std::flush;

    ba::io_service aios;
    ba::steady_timer timer(aios, sc::seconds(1));
    timer.wait();

    std::cout << "done!" << std::endl;
}

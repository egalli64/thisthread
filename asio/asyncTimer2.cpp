/**
 * Info: http://thisthread.blogspot.com/2013/08/another-asynchronous-wait-on-steady.html
 *
 * Based on: http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/tutorial/tuttimer2.html
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

class MyJob
{
private:
    MyJob(const MyJob&) = delete;
    const MyJob& operator=(const MyJob& ) = delete;

    std::mutex mx_;
    bool expired_;
public:
    MyJob() : expired_(false) {}

    void log(const char* message)
    {
        std::unique_lock<std::mutex> lock(mx_);
        std::cout << message << std::endl;
    }

    void timeout()
    {
        expired_ = true;
        log("Timeout!");
    }

    void operator()()
    {
        for(int i = 0; !expired_; ++i)
        {
            std::ostringstream os;
            os << '[' << i << ']';

            log(os.str().c_str());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main()
{
    boost::asio::io_service io;
    boost::asio::steady_timer timer(io, std::chrono::seconds(3));

    MyJob job;
    timer.async_wait([&job](const boost::system::error_code&) {
        job.timeout();
    });

    std::thread thread(std::ref(job));
    io.run();
    thread.join();
}

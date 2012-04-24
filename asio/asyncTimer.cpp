/**
 * For code comments see: http://thisthread.blogspot.com/2011/02/simple-deadline-timer.html
 *
 * Boost ASIO example based on: http://www.boost.org/doc/libs/1_49_0/doc/html/boost_asio/tutorial/tuttimer2.html
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

class MyJob : public boost::noncopyable // 1
{
private:
    boost::mutex mx_;
    bool outOfTime_; // 2
public:
    MyJob() : outOfTime_(false) {}

    void log(const char* message) // 3
    {
        boost::lock_guard<boost::mutex> lock(mx_);
        std::cout << message << std::endl;
    }

    void timeout() // 4
    {
        outOfTime_ = true;
        log("Timeout!");
    }

    void operator()() // 5
    {
        for(int i = 0; !outOfTime_; ++i)
        {
            std::ostringstream os;
            os << '[' << i << ']';

            log(os.str().c_str());
            boost::this_thread::sleep(boost::posix_time::seconds(1));
        }
    }
};

class MyWaitHandler
{
private:
    MyJob& job_; // 1

public:
    MyWaitHandler(MyJob& job) : job_(job) {}

    void operator()(const boost::system::error_code&)
    {
        job_.timeout(); // 2
    }
};

void tt01()
{
    boost::asio::io_service io; // 1
    boost::asio::deadline_timer timer(io, boost::posix_time::seconds(3)); // 2

    MyJob job; // 3
    timer.async_wait(MyWaitHandler(job)); // 4

    boost::thread thread(std::ref(job)); // 5
    io.run();
    job.log("I/O service completed");

    thread.join();
    job.log("Local thread completed");
}

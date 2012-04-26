#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace ba = boost::asio;
namespace bt = boost::this_thread;

namespace
{
    boost::mutex mio;

    void dump(const char* msg)
    {   
        boost::lock_guard<boost::mutex> lock(mio);
        std::cout << bt::get_id() << ' ' << msg << std::endl;
    }

    void dump(const char* msg, unsigned int value)
    {   
        boost::lock_guard<boost::mutex> lock(mio);
        std::cout << bt::get_id() << ' ' << msg << ' ' << value << std::endl;
    }
}

void worker(ba::io_service& aios)
{
    dump("start worker");
	aios.run();
    dump("end worker");
}

unsigned int fibonacci(unsigned int n)
{
    if(n < 2)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void calculate(unsigned int input)
{
    dump("input", input);
    int result = fibonacci(input);
    dump("output", result);
}

void ex3a()
{
    ba::io_service aios;

    dump("starting up");

    aios.post(boost::bind(calculate, 35));
    aios.post(boost::bind(calculate, 30));
	aios.post(boost::bind(calculate, 20));
	aios.post(boost::bind(calculate, 10));

    boost::thread_group threads;
    for(int i = 0; i < 2; ++i)
        threads.create_thread(std::bind(worker, std::ref(aios)));

    dump("ready to join");
	threads.join_all();
    dump("job done");
}

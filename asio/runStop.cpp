/**
 * For information and comments see: http://thisthread.blogspot.com/2012/04/run-and-stop-asio-io-service.html
 */
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <iostream>

namespace
{
    boost::mutex mio;

    void dump(const char* msg)
    {   
        boost::lock_guard<boost::mutex> lock(mio);
        std::cout << msg << std::endl;
    }

    void dump(int id, const char* msg)
    {   
        boost::lock_guard<boost::mutex> lock(mio);
        std::cout << id << ' ' << msg << std::endl;
    }

    void worker(boost::asio::io_service& aios, int id)
    {
        dump(id, "first step");
	    aios.run();
        dump(id, "last step");
    }
}

void runStop()
{
    boost::asio::io_service aios;
    boost::asio::io_service::work work(aios);

	boost::thread_group threads;
	for(int i = 0; i < 6; ++i )
		threads.create_thread(std::bind(worker, std::ref(aios), i));

    dump("Main thread has spawned a bunch of threads");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    aios.stop();
    dump("Main thread asked ASIO io service to stop");
	threads.join_all();
    dump("Worker threads joined");
}

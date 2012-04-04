#include <iostream>
#include <string>
#include <algorithm>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include "zmq.hpp"

namespace
{
    const char* SOCK_ADDR = "inproc://workers";
    boost::mutex mio;

    void dumpMessage(const std::string& id, int value)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::cout << id << ' ' << value << std::endl;
    }

    void dumpMessage(const std::string& id, const char* msg)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::cout << id << ' ' << msg << std::endl;
    }

    void worker(zmq::context_t& context)
    {
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
        zmq::socket_t worker(context, ZMQ_REQ);
        zmq_setsockopt(worker, ZMQ_IDENTITY, id.c_str(), id.length());
        worker.connect(SOCK_ADDR);

        int processed = 0;
        while(true)
        {
            zmq::message_t msg(&processed, sizeof(int), NULL);
            worker.send(msg);
            zmq::message_t payload;
            if(worker.recv(&payload) == false)
            {
                dumpMessage(id, "error receiving message");
                return;
            }
            if(payload.size() != sizeof(int)) // only int are expected
            {
                dumpMessage(id, "terminating");
                return;
            }

            int value = *(int*)payload.data();
            dumpMessage(id, value);

            boost::this_thread::sleep(boost::posix_time::millisec(value));
            ++processed;
        }
    }

    class RandomTimeGenerator
    {
    private:
        boost::random::mt19937 generator_;
        boost::random::uniform_int_distribution<> random_;
    public:
        RandomTimeGenerator(int low, int high) : random_(low, high) {}
        int getValue() { return random_(generator_); }
    };

    class MyRouter
    {
    private:
        zmq::socket_t client_;
        RandomTimeGenerator rtg_;

    public:
        MyRouter(zmq::context_t& context) : client_(context, ZMQ_ROUTER), rtg_(1, 1000)
        {
            client_.bind(SOCK_ADDR);
        }

        void sendMessage()
        {
            zmq::message_t zmAddress;
            client_.recv(&zmAddress);

            zmq::message_t zmDummy1;
            client_.recv(&zmDummy1);
            zmq::message_t zmDummy2;
            client_.recv(&zmDummy2);

            client_.send(zmAddress, ZMQ_SNDMORE);

            zmq::message_t zmEmpty;
            client_.send(zmEmpty, ZMQ_SNDMORE);

            int value = rtg_.getValue();
            zmq::message_t zmPayload(sizeof(int));
            memcpy(zmPayload.data(), &value, sizeof(int));
            client_.send(zmPayload);
        }

        int terminateWorker()
        {
            zmq::message_t zmAddress;
            client_.recv(&zmAddress);
            zmq::message_t zmDummy;
            client_.recv(&zmDummy);

            zmq::message_t zmPayload;
            client_.recv(&zmPayload);
            std::string id((char*)zmAddress.data(), (char*)zmAddress.data() + zmAddress.size());
            int acknowledged = *(int*)zmPayload.data();
            dumpMessage(id, acknowledged);

            client_.send(zmAddress, ZMQ_SNDMORE);

            zmq::message_t zmEmpty;
            client_.send(zmEmpty, ZMQ_SNDMORE);
            client_.send(zmEmpty);

            return acknowledged;
        }
    };
}

void lru(int nWorkers)
{
    zmq::context_t context(1);
    MyRouter router(context);

    boost::thread_group threads;
    for(int i = 0; i < nWorkers; ++i)
        threads.create_thread(std::bind(worker, std::ref(context)));

    for(int i = 0; i < nWorkers * 10; ++i)
        router.sendMessage();

    int processed = 0;
    for(int i = 0; i < nWorkers; ++i)
        processed += router.terminateWorker();

    threads.join_all();
    std::cout << "Number of processed messages: " << processed << std::endl;
}

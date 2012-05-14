/**
 * Asynchronous Client-Server, for info and comments please check the following pages
 *
 * introduction: http://thisthread.blogspot.com/2012/05/asynchronous-clientserver-0mq.html
 * client: http://thisthread.blogspot.com/2012/05/async-0mq-application-dealerrouter.html
 * server: http://thisthread.blogspot.com/2012/05/async-0mq-application-server.html
 * worker: http://thisthread.blogspot.com/2012/05/async-0mq-app-worker.html
 *
 * Based on a ZGuide example: http://zguide.zeromq.org/page:all#Asynchronous-Client-Server
 */

#include <string>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include "zmq2.h"

namespace
{
    const char* SK_CLI_ADDR = "tcp://localhost:5570";
    const char* SK_SRV_ADDR = "tcp://*:5570";
    const char* SK_BCK_ADDR = "inproc://backend";

    boost::mutex mio;

    void dumpId(const char* msg)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());

        std::cout << id << ' ' << msg << std::endl;
    }

    void dumpId(const char* tag, const std::string& msg)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());

        std::cout << id << ' ' << tag << ' ' << msg << std::endl;
    }

    void dumpId(zmq::message_t& zm)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
        std::cout << id << ' ';
        const char* base = static_cast<const char*>(zm.data());
        std::for_each(base, base + zm.size(), [](char c){ std::cout << c; });
        std::cout << std::endl;
    }

    class MyRandom
    {
    private:
        boost::random::mt19937 generator_;
        boost::random::uniform_int_distribution<> random_;
    public:
        MyRandom(int low, int hi) : random_(low, hi) {}

        int getValue() { return random_(generator_); }
    };

    class AsynchronousCS
    {
    private:
        boost::thread_group threads_;
        MyRandom rand_;

        void client()
        {
            zmq::context_t context(1);
            std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
            zmq::Socket skClient(context, ZMQ_DEALER, id);
            skClient.connect(SK_CLI_ADDR);

            dumpId("Client started");

            zmq_pollitem_t items[] = { { skClient, 0, ZMQ_POLLIN, 0 } };

            std::string message("message ");
            for(int i =0; i < 6; ++i) // send a few messages
            {
                for(int heartbeat = 0; heartbeat < 100; ++heartbeat) // simulate heartbeat
                {
                    zmq_poll(items, 1, 10 * 1000); // polls each 10 millis
                    if(items[0].revents & ZMQ_POLLIN)
                    {
                        std::string msg = skClient.recvAsString();
                        dumpId("client receives", msg);

                        items[0].revents = 0; // cleanup
                    }
                }
                dumpId("client sends", message);
                skClient.send(message);
                message += 'x';
            }

            // coda
            while(zmq_poll(items, 1, 1000 * 1000) > 0) // 1 sec
            {
                if(items[0].revents & ZMQ_POLLIN)
                {
                    std::string msg = skClient.recvAsString();
                    dumpId("coda", msg);
                    items[0].revents = 0; // cleanup
                }
            }

            dumpId("Client completed");
        }

        void server(int nWorkers)
        {
            zmq::context_t context(1);
            zmq::Socket skFrontend(context, ZMQ_ROUTER);
            skFrontend.bind(SK_SRV_ADDR);

            zmq::Socket skBackend(context, ZMQ_DEALER);
            skBackend.bind(SK_BCK_ADDR);

            boost::thread_group threads;
            for(int i = 0; i < nWorkers; ++i)
                threads.create_thread(std::bind(&AsynchronousCS::worker, this, std::ref(context)));

            dumpId("Server started");

            zmq_pollitem_t items [] =
            {
                { skFrontend, 0, ZMQ_POLLIN, 0 },
                { skBackend,  0, ZMQ_POLLIN, 0 }
            };

            dumpId("Switching messages between frontend and backend");
            while(true)
            {
                if(zmq_poll(items, 2, 3000 * 1000) < 1) // 3 secs
                    break;

                if(items[0].revents & ZMQ_POLLIN)
                {
                    zmq::Frames frames = skFrontend.blockingRecv(2);
                    dumpId("Received on frontend", frames[0]);
                    skBackend.send(frames);
                    items[0].revents = 0; // cleanup
                }
                if(items[1].revents & ZMQ_POLLIN)
                {
                    zmq::Frames frames = skBackend.blockingRecv(2);
                    dumpId("Received on backend", frames[0]);
                    skFrontend.send(frames);
                    items[1].revents = 0; // cleanup
                }
            }

            dumpId("Nothing happened for a while");

            // send a terminator to each worker
            for(int i = 0; i < nWorkers; ++i)
                skBackend.send("");

            threads.join_all();
            dumpId("Server shutdown");
        }

        void worker(zmq::context_t& context)
        {
            zmq::Socket skWorker(context, ZMQ_DEALER);
            skWorker.connect(SK_BCK_ADDR);

            dumpId("Worker started");

            while(true)
            {
                zmq::Frames frames = skWorker.blockingRecv(2, false);
                if(frames.size() == 1)
                    break;

                // send a few replies
                int replies = rand_.getValue();
                for(int i =0; i < replies; ++i)
                {
                    boost::this_thread::sleep(boost::posix_time::millisec(100 * rand_.getValue()));
                    dumpId("worker reply");
                    skWorker.send(frames);
                }
            }

            dumpId("terminating worker");
        }
    public:
        AsynchronousCS(int nClients, int nWorkers) : rand_(0,2)
        {
            for(int i = 0; i < nClients; ++i)
                threads_.create_thread(std::bind(&AsynchronousCS::client, this));
            threads_.create_thread(std::bind(&AsynchronousCS::server, this, nWorkers));
        }

        ~AsynchronousCS() { threads_.join_all(); }
    };
}

void dealer2router(int nClients, int nWorkers)
{
    AsynchronousCS(nClients, nWorkers);
}

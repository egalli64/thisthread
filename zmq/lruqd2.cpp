/**
 * For information on this code see http://thisthread.blogspot.com/2012/04/lru-queue-broker-version-two.html
 * The original idea comes from the ZGuide: http://zguide.zeromq.org/page:all#A-High-Level-API-for-MQ
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include "zmq2.h"

namespace
{
    const char* SK_ADDR_BACKEND = "inproc://backend";
    const char* SK_ADDR_FRONTEND = "inproc://frontend";
    boost::mutex mio;

    void dump(const std::string& id, int value)
    {   
        boost::lock_guard<boost::mutex> lock(mio);
        std::cout << id << ' ' << value << std::endl;
    }

    void dump(const std::string& id, const char* msg)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::cout << id << ' ' << msg << std::endl;
    }

    void dump(const std::string& id, zmq::message_t& zm)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::cout << id << ' ';
        const char* base = static_cast<const char*>(zm.data());
        std::for_each(base, base + zm.size(), [](char c){ std::cout << c; });
        std::cout << std::endl;
    }

    void client(zmq::context_t& context)
    {
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
        zmq::Socket skClient(context, ZMQ_REQ, id);
        skClient.connect(SK_ADDR_FRONTEND);
        dump(id, "client is up");

        skClient.send(id);
        dump(id, "client request sent");

        std::string reply = skClient.recvAsString();
        dump(reply, "received by client");
    }

    void worker(zmq::context_t& context)
    {
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
        zmq::Socket skWorker(context, ZMQ_REQ, id);
        skWorker.connect(SK_ADDR_BACKEND);

        zmq::Frames frames(3);
        while(true)
        {
            // sending a request (first time is dummy)
            skWorker.send(frames);

            // receving dummy || clientID, "", payload
            frames = skWorker.blockingRecv(3, false);
            if(frames.size() != 3)
            {
                dump(id, "terminating");
                return;
            }
            dump(frames[0], "client id");
            dump(frames[2], "payload");
        }
    }

    class MyDevice
    {
    public:
        MyDevice() : context_(1), 
            backend_(context_, ZMQ_ROUTER), frontend_(context_, ZMQ_ROUTER)
        {
            backend_.bind(SK_ADDR_BACKEND);
            frontend_.bind(SK_ADDR_FRONTEND);
        }

        ~MyDevice()
        {
            thWorkers_.join_all();
            thClients_.join_all();
        }

        void start(int nClients, int nWorkers)
        {
            for(int i = 0; i < nWorkers; ++i)
                thWorkers_.create_thread(std::bind(worker, std::ref(context_)));

            for(int i = 0; i < nClients; ++i)
                thClients_.create_thread(std::bind(client, std::ref(context_)));
        }

        void poll()
        {
            zmq_pollitem_t items [] = {
                { backend_,  0, ZMQ_POLLIN, 0 },
                { frontend_, 0, ZMQ_POLLIN, 0 }
            };

            while(zmq_poll(items, qWorkers_.empty() ? 1 : 2, 1000000) > 0)
            {
                if(items[0].revents & ZMQ_POLLIN)
                {
                    receivingOnBackend();
                    items[0].revents = 0; // cleanup
                }
                if(items[1].revents & ZMQ_POLLIN)
                {
                    receivingOnFrontend();
                    items[1].revents = 0; // cleanup
                }
            }

            // kill all the pending workers
            while(!qWorkers_.empty())
            {
                std::string id = qWorkers_.front();
                qWorkers_.pop();
                dump(id, "Terminating worker");

                zmq::Frames frames;
                frames.reserve(3);
                frames.push_back(id);
                frames.push_back("");   // separator
                frames.push_back("");   // fake client address as terminator
                backend_.send(frames);
            }
        }
    private:
        zmq::context_t context_;
        zmq::Socket backend_;
        zmq::Socket frontend_;
        std::queue<std::string> qWorkers_;
        boost::thread_group thWorkers_;
        boost::thread_group thClients_;

        void receivingOnBackend()
        {
            zmq::Frames input = backend_.blockingRecv(5, false); // workerID (, "", clientID, "", payload)

            // adding the worker on queue
            dump(input[0], "registering worker");
            qWorkers_.push(input[0]);

            if(input.size() == 5) // full message
            { // forward to the frontend
                zmq::Frames output(input.begin() + 2, input.end()); // discarding workerID (and first separator)
                frontend_.send(output);
            }
        }

        void receivingOnFrontend()
        {
            zmq::Frames input = frontend_.blockingRecv(3); // clientID, "", payload

            dump(input[0], "client id received on frontend");
            dump(input[2], "payload received on frontend");

            // picking up a worker
            std::string id = qWorkers_.front();
            qWorkers_.pop();
            dump(id, "selected worker on frontend");

            zmq::Frames output;
            output.reserve(5); // workerID, "", clientID, "", payload
            output.push_back(id);
            output.push_back("");
            output.insert(output.end(), input.begin(), input.end());

            backend_.send(output);
            dump(id, "message sent to worker");
        }
    };
}

void lruQueue(int nClients, int nWorkers)
{
    MyDevice device;

    device.start(nClients, nWorkers);
    device.poll();
}

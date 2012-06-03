/**
 * Based on the ZGuide: http://zguide.zeromq.org/page:all#Basic-Reliable-Queuing-Simple-Pirate-Pattern
 */
#include <iostream>
#include <queue>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "zmq2.h"

namespace
{
    const int BASE_TIMEOUT = 1000;

    const char* SKA_FRONTEND_CLI = "tcp://localhost:5555";
    const char* SKA_BACKEND_CLI = "tcp://localhost:5556";
    const char* SKA_FRONTEND_SRV = "tcp://*:5555";
    const char* SKA_BACKEND_SRV = "tcp://*:5556";

    boost::mutex mio;
    void dumpId(const char* tag, const char* msg = nullptr)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());

        std::cout << id << ' ' << tag;
        if(msg)
            std::cout << ' ' << msg;
        std::cout << std::endl;
    }

    void dumpId(const char* tag, int value)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());

        std::cout << id << ' ' << tag << ' ' << value << std::endl;
    }

    class LazyPirateClient
    {
    private:
        zmq::context_t& context_;
        std::unique_ptr<zmq::Socket> sk_;

        int sent_;

        void reset()
        {
            std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
            sk_.reset(new zmq::Socket(context_, ZMQ_REQ, id));
            sk_->connect(SKA_FRONTEND_CLI);

            int linger = 0;
            sk_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
        }
    public:
        LazyPirateClient(zmq::context_t& context) : context_(context), sent_(-1)
        {
            reset();
        }

        bool checkedRecv()
        {
            int value = sk_->recvAsInt();
            if(value == sent_)
                return true;

            dumpId("Unexpected reply from server", value);
            return false;
        }

        zmq::Socket* operator()() { return sk_.get(); }

        void send(int value)
        {
            sk_->send(value);
            sent_ = value;
        }

        void resend()
        {
            reset();
            send(sent_);
        }
    };

    void client(int timeout, int retries)
    {
        dumpId("Starting client");

        zmq::context_t context(1);
        LazyPirateClient skClient(context);

        for(int sequence = 1; sequence < 100; ++sequence)
        {
            skClient.send(sequence);
            boost::this_thread::sleep(boost::posix_time::millisec(BASE_TIMEOUT));

            bool confirmed = false;
            zmq::pollitem_t items[] = { { *skClient(), 0, ZMQ_POLLIN, 0 } };
            for(int cycle = 0; cycle < retries; ++cycle)
            {
                zmq::poll(&items[0], 1, timeout * 1000);

                if(items[0].revents & ZMQ_POLLIN)
                {
                    if(skClient.checkedRecv())
                    {
                        dumpId("Server is synchronized", sequence);
                        confirmed = true;
                        break;
                    }
                }
                else
                {
                    dumpId("Retrying", cycle);
                    skClient.resend();
                    items[0].socket = *skClient();
                }
            }
            if(!confirmed)
            {
                dumpId("No answer from server, abandoning.");
                break;
            }
        }
    }

    void lruQueue()
    {
        dumpId("starting LRU queue");

        zmq::context_t context(1);

        zmq::Socket frontend(context, ZMQ_ROUTER);
        zmq::Socket backend(context, ZMQ_ROUTER);
        frontend.bind(SKA_FRONTEND_SRV);
        backend.bind(SKA_BACKEND_SRV);

        std::queue<std::string> workers;

        zmq::pollitem_t items [] =
        {
            { backend,  0, ZMQ_POLLIN, 0 },
            { frontend, 0, ZMQ_POLLIN, 0 }
        };

        while(true)
        {
            // no frontend polling if no available workers
            zmq::poll(items, workers.size() ? 2 : 1, -1);

            //  Handle worker activity on backend
            if(items[0].revents & ZMQ_POLLIN)
            {
                zmq::Frames input = backend.blockingRecv(5, false);
                workers.push(input[0]);

                if(input.size() == 5) // full message
                { // forward to the frontend
                    zmq::Frames output(input.begin() + 2, input.end()); // discarding workerID (and first separator)
                    frontend.send(output);
                }
                items[0].revents = 0;
            }

            if(items[1].revents & ZMQ_POLLIN)
            {
                zmq::Frames input = frontend.blockingRecv(3);

                std::string wid = workers.front();
                workers.pop();

                zmq::Frames output;
                output.reserve(5); // workerID, "", clientID, "", payload
                output.push_back(wid);
                output.push_back("");
                output.insert(output.end(), input.begin(), input.end());

                backend.send(output);
                items[1].revents = 0;
            }
        }
    }

    void worker()
    {
        dumpId("starting worker");

        zmq::context_t context(1);
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
        zmq::Socket skWorker(context, ZMQ_REQ, id);
        skWorker.connect(SKA_BACKEND_CLI);

        skWorker.send("");

        for(int i = 0; i < 6; ++i)
        {
            zmq::Frames frames = skWorker.blockingRecv(1, false);

            if(i == 2)
            {
                dumpId("Simulating CPU overload");
                boost::this_thread::sleep(boost::posix_time::seconds(3));
            }
            else
                dumpId("Normal reply");
            
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            skWorker.send(frames);
        }
        dumpId("Simulating a worker crash");
    }
}

void simplePirate()
{
    boost::thread_group threads;
    threads.create_thread(std::bind(client, 2500,  3));
    threads.create_thread(lruQueue);
    threads.create_thread(worker);

    boost::this_thread::sleep(boost::posix_time::seconds(20));
    dumpId("---");
    threads.create_thread(worker);
    threads.create_thread(std::bind(client, 2500,  3));

    threads.join_all();
}

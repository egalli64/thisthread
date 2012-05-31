/**
 * Lazy Pirate pattern C++ example
 * For comments and information: http://thisthread.blogspot.com/2012/05/client-side-reliable-req-rep.html
 * Based on the ZGuide: http://zguide.zeromq.org/page:all#Client-side-Reliability-Lazy-Pirate-Pattern
 */
#include <iostream>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "zmq2.h"

namespace
{
    const char* SK_ADDR_CLI = "tcp://localhost:5555";
    const char* SK_ADDR_SRV = "tcp://*:5555";

    const int BASE_TIMEOUT = 1000;

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
            sk_.reset(new zmq::Socket(context_, ZMQ_REQ));
            sk_->connect(SK_ADDR_CLI);

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

    void server()
    {
        dumpId("Starting server");

        zmq::context_t context(1);
        zmq::Socket skServer(context, ZMQ_REP);
        skServer.bind(SK_ADDR_SRV);

        for(int i = 1; i < 10; ++i)
        {
            int message = skServer.recvAsInt();

            if(i % 4 == 0)
            {
                dumpId("CPU overload");
                boost::this_thread::sleep(boost::posix_time::millisec(2 * BASE_TIMEOUT));
            }
            else
                dumpId("Normal request", message);

            boost::this_thread::sleep(boost::posix_time::millisec(BASE_TIMEOUT));
            skServer.send(message);
        }

        dumpId("Terminating, as for a server crash");
    }
}

void lazyPirate(int timeout, int retries)
{
    boost::thread_group threads;
    threads.create_thread(std::bind(client, timeout, retries));
    threads.create_thread(server);
    threads.join_all();
}

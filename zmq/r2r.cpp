#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "zmq2.h"

namespace
{
    const char* SK_ADDR_CLI = "tcp://localhost:5380";
    const char* SK_ADDR_SRV = "tcp://*:5380";

    boost::mutex mio;
    void dumpId(const char* msg, const char* ext = nullptr)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());

        std::cout << id << ' ' << msg;
        if(ext)
            std::cout << ' ' << ext;
        std::cout << std::endl;
    }

    // clients id are predefined
    void rrClientC(const char* id)
    {
        dumpId("client startup");
        zmq::context_t context(1);

        zmq::Socket skRouter(context, ZMQ_ROUTER, id);
        skRouter.connect(SK_ADDR_CLI);
        dumpId(SK_ADDR_CLI, id);

        zmq::Frames input = skRouter.blockingRecv(2);
        dumpId("client received", input[1].c_str());

        skRouter.send(input[0], id);
        dumpId("client shutdown");
    }

    void rrServerC(const char* ids[], int nClients)
    {
        dumpId("server startup");
        zmq::context_t context(1);

        zmq::Socket skRouter(context, ZMQ_ROUTER);
        skRouter.bind(SK_ADDR_SRV);

        dumpId("server ready on", SK_ADDR_SRV);
        boost::this_thread::sleep(boost::posix_time::seconds(1));

        for(int i =0; i < nClients; ++i)
        {
            dumpId("server send to", ids[i]);
            skRouter.send(ids[i], "hello");
        }

        for(int i =0; i < nClients; ++i)
        {
            zmq::Frames in = skRouter.blockingRecv(2);
            dumpId("receiving from", in[1].c_str());
        }
        dumpId("server shutdown");
    }

    // server id is predefined
    void rrClientS(const char* server)
    {
        dumpId("client startup");
        zmq::context_t context(1);

        std::string tid = boost::lexical_cast<std::string>(boost::this_thread::get_id());
        zmq::Socket skRouter(context, ZMQ_ROUTER, tid);
        skRouter.connect(SK_ADDR_CLI);
        dumpId(SK_ADDR_CLI, server);

        boost::this_thread::sleep(boost::posix_time::seconds(1));
        skRouter.send(server, "");

        zmq::Frames frames = skRouter.blockingRecv(2);
        dumpId("client shutdown");
    }

    void rrServerS(const char* sid, int nClients)
    {
        dumpId("server startup");
        zmq::context_t context(1);

        zmq::Socket skRouter(context, ZMQ_ROUTER, sid);
        skRouter.bind(SK_ADDR_SRV);

        dumpId("server ready on", SK_ADDR_SRV);

        std::vector<std::string> clients;
        clients.reserve(nClients);
        for(int i =0; i < nClients; ++i)
        {
            zmq::Frames in = skRouter.blockingRecv(2);
            dumpId("receiving from", in[0].c_str());
            clients.push_back(in[0]);
        }

        std::for_each(clients.begin(), clients.end(), [&skRouter](std::string& client)
        {
            skRouter.send(client, "");
        });
        dumpId("server shutdown");
    }
}

void router2router()
{
    // server knows clients id
    {
        const char* clients[] = { "ClientA", "ClientB", "ClientC" };
        int nClients = sizeof(clients) / sizeof(const char*);

        boost::thread_group threads;
        for(int i =0; i < nClients; ++i)
            threads.create_thread(std::bind(rrClientC, clients[i]));

        threads.create_thread(std::bind(rrServerC, clients, nClients));
        threads.join_all();
    }

    std::cout << std::endl;

    // clients know server id
    {
        const char* server = "Server";
        const int nClients = 3;

        boost::thread_group threads;
        for(int i =0; i < nClients; ++i)
            threads.create_thread(std::bind(rrClientS, server));

        threads.create_thread(std::bind(rrServerS, server, nClients));
        threads.join_all();
    }

    std::cout << std::endl;
}

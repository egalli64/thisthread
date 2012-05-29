/**
 * Router to router for a peer to peer cluster.
 * Comments and information on: http://thisthread.blogspot.com/2012/05/router-to-router-among-peers.html
 */
#include <iostream>
#include <string>
#include <queue>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "zmq2.h"

namespace
{
    const char* SKA_NODE_SRV[] = {"tcp://*:5380", "tcp://*:5381", "tcp://*:5382" };
    const char* SKA_NODE_CLI[] = {"tcp://localhost:5380", "tcp://localhost:5381", "tcp://localhost:5382" };

    const char* NODE_IDS[] = { "N0", "N1", "N2" };
    enum NodeId { N0 = 0, N1, N2, N_NODES = N2 + 1 };

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
}

namespace
{
    void node(NodeId nid)
    {
        zmq::context_t context(1);

        dumpId(NODE_IDS[nid], SKA_NODE_SRV[nid]);
        zmq::Socket skServer(context, ZMQ_ROUTER, NODE_IDS[nid]);
        skServer.bind(SKA_NODE_SRV[nid]);

        dumpId("client router", NODE_IDS[nid]);
        zmq::Socket skClient(context, ZMQ_ROUTER, NODE_IDS[nid]);
        for(NodeId id = N0; id < N_NODES; id = static_cast<NodeId>(id+1))
        {
            if(id == nid)
                continue;

            skClient.connect(SKA_NODE_CLI[id]);
            dumpId("client connected to", SKA_NODE_CLI[id]);
        }

        boost::this_thread::sleep(boost::posix_time::seconds(1));

        for(NodeId id = N0; id < N_NODES; id = static_cast<NodeId>(id+1))
        {
            if(id == nid)
                continue;

            dumpId("sending a message to", NODE_IDS[id]);
            skClient.send(NODE_IDS[id], "hey");
        }

        zmq_pollitem_t servers[] = { { skServer, 0, ZMQ_POLLIN, 0 } };
        while(zmq_poll(servers, 1, 1000 * 1000) > 0)
        {
            zmq::Frames frames;
            if(servers[0].revents & ZMQ_POLLIN)
            {
                dumpId("Receiving from peers");

                frames = skServer.blockingRecv(2);
                dumpId(frames[0].c_str(), frames[1].c_str());

                servers[0].revents = 0;
            }
        }
        dumpId(NODE_IDS[nid], "done");
    }
}

void r2rPeers()
{
    boost::thread_group threads;
    for(NodeId id = N0; id < N_NODES; id = static_cast<NodeId>(id+1))
        threads.create_thread(std::bind(node, id));
    threads.join_all();
}

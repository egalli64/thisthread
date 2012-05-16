/**
 * For information and comments: http://thisthread.blogspot.com/2012/05/monitoring-broker-state-with-pub-sub.html
 *
 * Based on this ZGuide example: http://zguide.zeromq.org/page:all#Prototyping-the-State-Flow
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "zmq2.h"

namespace
{
    const char* SKA_BROKER_STATE_CLI[] = {"tcp://localhost:5180", "tcp://localhost:5181", "tcp://localhost:5182"};
    const char* SKA_BROKER_STATE_SRV[] = {"tcp://*:5180", "tcp://*:5181", "tcp://*:5182"};

    const int N_BROKERS = sizeof(SKA_BROKER_STATE_SRV) / sizeof(const char*);

    boost::mutex mio;
    void dumpId(const char* tag, const char* msg)
    {
        boost::lock_guard<boost::mutex> lock(mio);
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());

        std::cout << id << ' ' << tag << ' ' << msg << std::endl;
    }

    void stateFlow(const char* broker, const std::vector<std::string>& peers)
    {
        zmq::context_t context(1);

        zmq::Socket stateBackend(context, ZMQ_PUB);
        stateBackend.bind(broker);
        dumpId(broker, "publisher bound");

        zmq::Socket stateFrontend(context, ZMQ_SUB);
        stateFrontend.setsockopt(ZMQ_SUBSCRIBE, "", 0); // no filter
        std::for_each(peers.begin(), peers.end(), [broker, &stateFrontend](const std::string& peer)
        {
            dumpId(broker, peer.c_str());
            stateFrontend.connect(peer.c_str());
        });

        // polling on frontend, sending on backend
        std::string tick(".");
        zmq_pollitem_t items [] = { { stateFrontend, 0, ZMQ_POLLIN, 0 } };
        for(int i = 0; i < 10; ++i)
        {
            if(zmq_poll(items, 1, 250 * 1000) < 0)
                break;

            if(items[0].revents & ZMQ_POLLIN)
            {
                zmq::Frames frames = stateFrontend.blockingRecv(2);
                dumpId(frames[0].c_str(), frames[1].c_str());
                items[0].revents = 0; // cleanup
            }
            else
            {
                dumpId("sending on", broker);
                zmq::Frames frames;
                frames.reserve(2);
                frames.push_back(broker);
                frames.push_back(tick);
                stateBackend.send(frames);

                tick += '.';
                boost::this_thread::sleep(boost::posix_time::millisec(333));
            }
        }
    }
}

void IBRstateFlow()
{
    boost::thread_group threads;

    {
        std::vector<std::string> peers;
        peers.push_back(SKA_BROKER_STATE_CLI[1]);
        peers.push_back(SKA_BROKER_STATE_CLI[2]);

        threads.create_thread(std::bind(stateFlow, SKA_BROKER_STATE_SRV[0], peers));
    }

    {
        std::vector<std::string> peers;
        peers.push_back(SKA_BROKER_STATE_CLI[2]);
        peers.push_back(SKA_BROKER_STATE_CLI[0]);

        threads.create_thread(std::bind(stateFlow, SKA_BROKER_STATE_SRV[1], peers));
    }

    {
        std::vector<std::string> peers;
        peers.push_back(SKA_BROKER_STATE_CLI[0]);
        peers.push_back(SKA_BROKER_STATE_CLI[1]);

        threads.create_thread(std::bind(stateFlow, SKA_BROKER_STATE_SRV[2], peers));
    }

    threads.join_all();
}

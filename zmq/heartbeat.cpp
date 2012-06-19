/**
 * Heartbeating for Paranoid Pirate: http://thisthread.blogspot.com/2012/06/paranoid-pirate-heartbeating.html
 * Server part disscussed here: http://thisthread.blogspot.com/2012/06/paranoid-pirate-heartbeating-server.html
 * Info on worker: http://thisthread.blogspot.com/2012/06/paranoid-pirate-heartbeating-worker.html
 * Based on the ZGuide: http://zguide.zeromq.org/page:all#Robust-Reliable-Queuing-Paranoid-Pirate-Pattern
 */

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "zmq2a.h"

namespace bp = boost::posix_time;

namespace
{
    const int BASE_INTERVAL = 1000; // msecs
    const int CHECK_FACTOR = 3;
    const char* SKA_BACKEND_CLI = "tcp://localhost:5556";
    const char* SKA_BACKEND_SRV = "tcp://*:5556";

    const uint8_t PHB_NOTHING = 0;
    const uint8_t PHB_READY = 1;
    const uint8_t PHB_HEARTBEAT = 2;
    const uint8_t PHB_DOWN = 3;

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
}

class HeartbeatWorker
{
private:
    zmq::context_t context_;
    std::unique_ptr<zmq::Socket> sk_;
    std::string id_;
    char idRoot_;
    bp::ptime heartbeat_;
    zmq::pollitem_t items_[1];

public:
    HeartbeatWorker(char id) : context_(1), idRoot_(id),
        heartbeat_(bp::microsec_clock::local_time() +  bp::millisec(BASE_INTERVAL))
    {
        reset();

        items_[0].fd = 0;
        items_[0].events = ZMQ_POLLIN;
        items_[0].revents = 0;
    }

    void reset()
    {
        id_ += idRoot_;
        sk_.reset(new zmq::Socket(context_, ZMQ_DEALER, id_));
        sk_->setLinger(0);
        sk_->connect(SKA_BACKEND_CLI);
        items_[0].socket = *sk_.get();

        dumpId("worker is up", id_.c_str());
        sk_->send(PHB_READY);
    }

    void heartbeat()
    {
        if(bp::microsec_clock::local_time() > heartbeat_)
        {
            heartbeat_ = bp::microsec_clock::local_time() + bp::millisec(BASE_INTERVAL);
            dumpId("worker sends heartbeat");
            sk_->send(PHB_HEARTBEAT);
        }
    }

    void shutdown()
    {
        dumpId("worker sends termination");
        sk_->send(PHB_DOWN);
    }

    uint8_t recv()
    {
        if(zmq::poll(items_, 1, BASE_INTERVAL * 1000) < 1)
            return PHB_NOTHING;

        uint8_t res = PHB_NOTHING;
        if(items_[0].revents & ZMQ_POLLIN)
            res = sk_->recvAsByte();
        items_[0].revents = 0;
        
        return res;
    }
};

class HeartbeatServer
{
private:
    zmq::context_t context_;
    zmq::Socket backend_;
    zmq::pollitem_t items_[1];

    std::string wid_;
    bp::ptime expiry_;

    int lifespan_;
    int busy_;
    bp::ptime heartbeat_;
    enum { INTERVAL = 1000, BUSY = 5 };
public:
    HeartbeatServer(int lifespan = INT_MAX) : context_(1), backend_(context_, ZMQ_ROUTER), lifespan_(lifespan), 
        heartbeat_(bp::microsec_clock::local_time() + bp::millisec(INTERVAL)), busy_(BUSY)
    {
        backend_.bind(SKA_BACKEND_SRV);
        backend_.setLinger(0);

        items_[0].socket = backend_;
        items_[0].fd = 0;
        items_[0].events = ZMQ_POLLIN;
        items_[0].revents = 0;
    }

    bool isAlive()
    {
        return busy_ > 0 && lifespan_ > 0;
    }

    zmq::Frames recv()
    {
        --lifespan_;

        zmq::Frames res;
        if(zmq::poll(items_, 1, INTERVAL * 1000) > 0)
        {
            busy_ = BUSY;

            if(items_[0].revents & ZMQ_POLLIN)
                res = backend_.blockingRecv();
            items_[0].revents = 0;

            return res;
        }
        else
            --busy_;

        return res; // no message
    }

    void heartbeat()
    {
        if(wid_.empty())
            return;

        // if it's time, send heartbeat
        if(bp::microsec_clock::local_time() > heartbeat_)
        {
            std::string control(&PHB_HEARTBEAT, &PHB_HEARTBEAT + 1);
            backend_.send(wid_, control);
            heartbeat_ = bp::microsec_clock::local_time() + bp::millisec(BASE_INTERVAL);
        }

        // if the worker is expired, remove its id 
        if(expiry_ < bp::microsec_clock::local_time())
        {
            wid_ = "";
            dumpId("No worker pending on server");
        }
    }

    void pushWorker(const std::string& id)
    {
        wid_ = id;
    }

    void refreshWorker(const std::string& id)
    {
        if(wid_ == id)
        {
            expiry_ = bp::microsec_clock::local_time() + bp::millisec(CHECK_FACTOR * BASE_INTERVAL);
            dumpId(id.c_str(), "refreshed");
        }
        else
            dumpId(id.c_str(), "not a pending worker");
    }

    void dropWorker(const std::string& id)
    {
        if(wid_ == id)
            wid_ = "";
        else
            dumpId(id.c_str(), "not a pending worker");
    }
};

namespace
{
    void server(int lifespan)
    {
        dumpId("Starting server");
        HeartbeatServer server(lifespan);

        while(server.isAlive())
        {
            zmq::Frames input = server.recv();
            if(!input.empty())
            {
                uint8_t control = (input.size() != 2 || input[1].empty()) ? PHB_NOTHING : input[1].at(0);

                switch(control)
                {
                case PHB_READY:
                    dumpId(input[0].c_str(), "ready");
                    server.pushWorker(input[0]);
                    break;
                case PHB_HEARTBEAT:
                    dumpId(input[0].c_str(), "heartbeat");
                    server.refreshWorker(input[0]);
                    break;
                case PHB_DOWN:
                    dumpId(input[0].c_str(), "disconnecting");
                    server.dropWorker(input[0]);
                    break;
                default:
                    dumpId(input[0].c_str(), control);
                    break;
                }
            }
            else
                dumpId("server idle");

            server.heartbeat();
        }
        dumpId("server shutdown");
    }

    void worker(char id, int lifespan)
    {
        const int PATIENCE = 3;
        HeartbeatWorker worker(id);

        int patience = 0;
        int cycle = 0;
        int sent = 0;
        int iteration = 1;

        while(true)
        {
            dumpId("worker loop", iteration);
            uint8_t control = worker.recv();
            if(control == PHB_NOTHING) // no valid input detected on worker socket
            {
                if(cycle++ == PATIENCE)
                {
                    if(patience++ == PATIENCE)
                        break; // out of patience

                    int factor = static_cast<int>(std::pow(2.0, patience));
                    dumpId("no heartbeat received, assuming server offline", factor);
                    boost::this_thread::sleep(bp::millisec(factor * BASE_INTERVAL));
                    dumpId("reconnecting ...");

                    worker.reset();
                    cycle = 0;
                }
                else
                    dumpId("worker idle", cycle + patience * 10);

                if(iteration++ == lifespan)
                    break;
            }
            else // polling succeeded
            {
                if(control == PHB_HEARTBEAT)
                    dumpId("heartbeat received on worker");
                else
                    dumpId("invalid message", control);
                cycle = patience = 0;
            }
            worker.heartbeat();
        }
        worker.shutdown();
        dumpId("worker shutdown");
    }
}

void heartbeating()
{
    std::cout << " *** 1 ***" << std::endl;
    {
        boost::thread_group threads;
        threads.create_thread(std::bind(server, INT_MAX));
        threads.create_thread(std::bind(worker, 'A', 6));
        threads.join_all();
    }

    std::cout << " *** 2 ***" << std::endl;
    {
        boost::thread_group threads;
        threads.create_thread(std::bind(server, 3));
        threads.create_thread(std::bind(worker, 'B', 7));

        boost::this_thread::sleep(boost::posix_time::seconds(10));
        threads.create_thread(std::bind(server, INT_MAX));
        threads.join_all();
    }
}

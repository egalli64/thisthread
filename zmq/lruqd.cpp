/**
 * Not much comments in the code, but have a look at this pages:
 * http://thisthread.blogspot.com/2012/04/lru-queue-device-general-view.html - what is about
 * http://thisthread.blogspot.com/2012/04/lru-queue-device-client.html - the client() function
 * http://thisthread.blogspot.com/2012/04/lru-queue-device-worker.html - the worker() function
 * http://thisthread.blogspot.com/2012/04/lru-queue-device-using-device.html - public section of class QueueDevice
 * http://thisthread.blogspot.com/2012/04/lru-queue-device-receiving-on-routers.html - private section of class QueueDevice
 *
 * Based on http://zguide.zeromq.org/page:all#A-Request-Reply-Message-Broker
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <zmq.hpp>

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

    void client(zmq::context_t& context, int value)
    {
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
        zmq::socket_t skClient(context, ZMQ_REQ);
        zmq_setsockopt(skClient, ZMQ_IDENTITY, id.c_str(), id.length());
        skClient.connect(SK_ADDR_FRONTEND);

        dump(id, "client is up");

        zmq::message_t zmPayload(sizeof(int));
        memcpy(zmPayload.data(), &value, sizeof(int));
        skClient.send(zmPayload);

        dump(id, "client request sent");

        zmq::message_t zmReply;
        skClient.recv(&zmReply);

        if(zmReply.size() == sizeof(int))
        {
            int reply = *((int*)zmReply.data());
            dump(id, reply);
        }
        else // unexpected
            dump(id, zmReply);
    }

    void worker(zmq::context_t& context)
    {
        std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
        zmq::socket_t skWorker(context, ZMQ_REQ);
        zmq_setsockopt(skWorker, ZMQ_IDENTITY, id.c_str(), id.length());
        skWorker.connect(SK_ADDR_BACKEND);

        std::string receiver;
        int payload = 0;
        while(true)
        {
            // sending a request (first time is dummy)
            zmq::message_t zmReceiver(receiver.length());
            memcpy(zmReceiver.data(), receiver.c_str(), receiver.length());
            skWorker.send(zmReceiver, ZMQ_SNDMORE);

            zmq::message_t zmDummy;
            skWorker.send(zmDummy, ZMQ_SNDMORE);

            zmq::message_t zmOutput(sizeof(int));
            memcpy(zmOutput.data(), &payload, sizeof(int));
            skWorker.send(zmOutput);
            //

            // receiving a reply
            zmq::message_t zmClientId;
            skWorker.recv(&zmClientId);
            dump(id, zmClientId);

            if(!zmClientId.size())
            {
                dump(id, "terminating");
                return;
            }
            const char* base = static_cast<const char*>(zmClientId.data());
            receiver = std::string(base, base + zmClientId.size());

            skWorker.recv(&zmDummy);

            zmq::message_t zmPayload;
            skWorker.recv(&zmPayload);
            if(zmPayload.size() != sizeof(int)) // only int are expected
            {
                dump(id, "bad payload detected");
                return;
            }

            payload = *(int*)zmPayload.data();
            dump(id, payload);

            boost::this_thread::sleep(boost::posix_time::millisec(payload));
        }
    }

    class QueueDevice
    {
    public:
        QueueDevice() : context_(1), 
            backend_(context_, ZMQ_ROUTER), frontend_(context_, ZMQ_ROUTER)
        {
            backend_.bind(SK_ADDR_BACKEND);
            frontend_.bind(SK_ADDR_FRONTEND);
        }

        ~QueueDevice()
        {
            thWorkers_.join_all();
            thClients_.join_all();
        }

        void start(int nClients, int nWorkers)
        {
            for(int i = 0; i < nWorkers; ++i)
                thWorkers_.create_thread(std::bind(worker, std::ref(context_)));

            boost::thread_group thClients;
            for(int i = 0; i < nClients; ++i)
            {
                int root = 42 + i * 10;
                thClients.create_thread(std::bind(client, std::ref(context_), root));
            }
        }

        void poll()
        {
            zmq_pollitem_t items [] =
            {
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

                zmq::message_t zmAddress(id.length());
                memcpy(zmAddress.data(), id.c_str(), id.length());

                backend_.send(zmAddress, ZMQ_SNDMORE);
                zmq::message_t zmEmpty;
                backend_.send(zmEmpty, ZMQ_SNDMORE);
                backend_.send(zmEmpty); // sending a fake client address as terminator
            }
        }
    private:
        zmq::context_t context_;
        zmq::socket_t backend_;
        zmq::socket_t frontend_;
        std::queue<std::string> qWorkers_;
        boost::thread_group thWorkers_;
        boost::thread_group thClients_;

        void receivingOnBackend()
        {
            zmq::message_t zmWorkerId;
            backend_.recv(&zmWorkerId);

            zmq::message_t zmDummy;
            backend_.recv(&zmDummy);

            zmq::message_t zmClientId;
            backend_.recv(&zmClientId);

            backend_.recv(&zmDummy);

            zmq::message_t zmPayload;
            backend_.recv(&zmPayload);

            // adding the worker on queue
            const char* base = static_cast<const char*>(zmWorkerId.data());
            std::string id(base, base + zmWorkerId.size());
            dump(id, "registering worker");
            qWorkers_.push(id);

            if(zmClientId.size())
            { // forward to the frontend
                frontend_.send(zmClientId, ZMQ_SNDMORE);
                frontend_.send(zmDummy, ZMQ_SNDMORE);
                frontend_.send(zmPayload);
            }
        }

        void receivingOnFrontend()
        {
            zmq::message_t zmIdCli;
            frontend_.recv(&zmIdCli);
            dump("Receiving on frontend from", zmIdCli);

            zmq::message_t zmDummy;
            frontend_.recv(&zmDummy);

            zmq::message_t zmPayload;
            frontend_.recv(&zmPayload);

            // picking up a worker
            std::string idWork = qWorkers_.front();
            qWorkers_.pop();
            dump(idWork, "selected worker");

            zmq::message_t zmIdWork(idWork.size());
            memcpy(zmIdWork.data(), idWork.c_str(), idWork.size());
            backend_.send(zmIdWork, ZMQ_SNDMORE);
            backend_.send(zmDummy, ZMQ_SNDMORE);
            backend_.send(zmIdCli, ZMQ_SNDMORE);
            backend_.send(zmDummy, ZMQ_SNDMORE);
            backend_.send(zmPayload);

            dump(idWork, "message sent to worker");
        }
    };
}

void lruQueue(int nWorkers, int nClients)
{
    QueueDevice device;

    device.start(nClients, nWorkers);
    device.poll();
}

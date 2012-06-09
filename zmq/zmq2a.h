/**
 * Extending zmq::socket_t to send/receive multipart messages.
 *
 * More information on first implementation: http://thisthread.blogspot.com/2012/04/extending-zmqsockett.html
 * Improved multipart send: http://thisthread.blogspot.com/2012/05/improved-sending-for-zmqsocket.html
 * Int messages: http://thisthread.blogspot.com/2012/05/sendingreceiving-ints-over-zeromq.html
 * Frames as a string deque:
 * Inspired by czmq, as described in the ZGuide: http://zguide.zeromq.org/page:all#A-High-Level-API-for-MQ
 */

#pragma once

#include <cstdint>
#include <deque>
#include <zmq.hpp>

namespace zmq
{
    typedef std::deque<std::string> Frames;

    class Socket : public socket_t
    {
    public:
        Socket(context_t& context, int type) : socket_t(context, type) {}

        Socket(context_t& context, int type, const std::string& id) : socket_t(context, type)
        {
            zmq_setsockopt(this->operator void*(), ZMQ_IDENTITY, id.c_str(), id.length());
        }
 
        bool send(const std::string& frame, int flags =0)
        {
            return send(frame.c_str(), frame.length(), flags);
        }

        bool send(const char* frame, int flags =0)
        {
            return send(frame, strlen(frame), flags);
        }

        bool send(const std::string& frame1, const std::string& frame2)
        {
            if(!send(frame1, ZMQ_SNDMORE))
                return false;
            // last frame
            return send(frame2);
        }

        bool send(const char* frame1, const char* frame2)
        {
            if(!send(frame1, ZMQ_SNDMORE))
                return false;
            // last frame
            return send(frame2);
        }

        bool send(const std::string& frame1, const std::string& frame2, const std::string& frame3)
        {
            if(!send(frame1, ZMQ_SNDMORE))
                return false;
            if(!send(frame2, ZMQ_SNDMORE))
                return false;
            // last frame
            return send(frame3);
        }

        bool send(const char* frame1, const char* frame2, const char* frame3)
        {
            if(!send(frame1, ZMQ_SNDMORE))
                return false;
            if(!send(frame2, ZMQ_SNDMORE))
                return false;
            // last frame
            return send(frame3);
        }

        bool send(const Frames& frames)
        {
            if(!frames.size())
                throw error_t();

            // all frames but last one
            for(unsigned int i = 0; i < frames.size() - 1; ++i)
                if(!send(frames[i], ZMQ_SNDMORE))
                    return false;
            // last frame
            return send(frames.back());
        }

        bool send(int value, int flags =0)
        {
            zmq::message_t msg(sizeof(int));
            memcpy(msg.data(), &value, sizeof(int));
            return socket_t::send(msg, flags);
        }

        Frames blockingRecv()
        {
            Frames frames;
            do {
                zmq::message_t message;
                if(!socket_t::recv(&message, 0))
                    throw error_t();

                const char* base = static_cast<const char*>(message.data());
                frames.push_back(std::string(base, base + message.size()));
            } while(sockopt_rcvmore());

            return frames;
        }

        std::string recvAsString(int flags =0)
        {
            zmq::message_t message;
            if(!socket_t::recv(&message, flags))
                throw error_t();

            const char* base = static_cast<const char*>(message.data());
            return std::string(base, base + message.size());
        }

        int recvAsInt(int flags =0)
        {
            zmq::message_t message;
            if(!socket_t::recv(&message, flags))
                throw error_t();

            return *(static_cast<int*>(message.data()));
        }
    private:
        Socket(const Socket&);
        void operator=(const Socket&);

        bool sockopt_rcvmore()
        {
            int64_t rcvmore;
            size_t type_size = sizeof(int64_t);
            getsockopt(ZMQ_RCVMORE, &rcvmore, &type_size);
            return rcvmore ? true : false;
        }

        bool send(const char* frame, size_t len, int flags =0)
        {
            zmq::message_t msg(len);
            memcpy(msg.data(), frame, len);
            return socket_t::send(msg, flags);
        }
    };
}

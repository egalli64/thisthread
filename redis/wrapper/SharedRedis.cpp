/**
 * SharedRedis.cpp
 * More info on http://thisthread.blogspot.com/2013/04/connecting-to-multiple-redis-servers.html 
 */

#include "boost/lexical_cast.hpp"
#include "SharedRedis.h"
#include "hiredis.h"

namespace TTR
{

SharedRedis::SharedRedis(const std::string& server, int port)
{
    std::string sport = server + ":" + boost::lexical_cast<std::string>(port);

    if(!port || server.empty())
    {
        std::cout << "Can't connect to Redis [" + sport + "]" << std::endl;
        return;
    }

    redisContext* context = redisConnect(server.c_str(), port);
    if(!context) // paranoid
    {
        std::cout << "No memory for Redis on " + sport << std::endl;
        return;
    }

    if(context->err)
    {
        std::cout << "Can't connect to Redis on " + sport + " - " + context->errstr << std::endl;

        redisFree(context);
        return;
    }

    spContext_ = SharedContext(context, std::bind(&SharedRedis::free, this));
    std::cout << "Connected to Redis [" + sport + "]" << std::endl;
}

void SharedRedis::free()
{
    redisFree(spContext_.get());
    std::cout << "Redis connection released" << std::endl;
}

void SharedRedis::set(const std::string& key, const std::string& value)
{
    if(!spContext_)
        return;

    void* reply = redisCommand(spContext_.get(), "SET %b %b", key.c_str(), key.length(), value.c_str(), value.length());
    if(reply)
    {
        freeReplyObject(reply);
        return;
    }

    // unexpected
    std::cout << "No reply from Redis" << std::endl;
}

std::string SharedRedis::get(const std::string& key)
{
	if(!spContext_)
		return "";

	redisReply* reply = static_cast<redisReply*>(redisCommand(spContext_.get(), "GET %b", key.c_str(), key.length()));
    if(!reply)
        return "";

    std::string result = reply->str ? reply->str : "";
	freeReplyObject(reply);
	return result;
}

} // namespace TTL

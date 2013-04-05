/**
 * UniqueRedis.cpp
 */

#include "boost/lexical_cast.hpp"
#include "UniqueRedis.h"
#include "hiredis.h"

namespace TTR
{

const std::string REDIS_HOST = "localhost";
const int REDIS_PORT = 6379;

UniqueRedis::UniqueRedis()
{
    std::string sport = REDIS_HOST + ":" + boost::lexical_cast<std::string>(REDIS_PORT);

    if(!REDIS_PORT || REDIS_HOST.empty())
    {
		std::cout << "Can't connect to Redis [" + sport + "]" << std::endl;
		return;
    }

	redisContext* context = redisConnect(REDIS_HOST.c_str(), REDIS_PORT);
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

	spContext_ = UniqueContext(context, std::bind(&UniqueRedis::free, this));
	std::cout << "Connected to Redis [" + sport + "]" << std::endl;
}

void UniqueRedis::free()
{
	redisFree(spContext_.get());
	std::cout << "Redis connection released" << std::endl;
}

void UniqueRedis::set(const std::string& key, const std::string& value)
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

std::string UniqueRedis::get(const std::string& key)
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

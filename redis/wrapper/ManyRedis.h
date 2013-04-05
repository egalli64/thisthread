/**
 * ManyRedis.h
 */

#pragma once

#include <iostream>
#include <memory>
class redisContext;

namespace TTR
{
typedef std::unique_ptr<redisContext, std::function<void(redisContext*)>> UniqueContext;

class ManyRedis
{
public:
	static UniqueRedis& instance(const std::string& host, const int port)
	{
		static UniqueRedis redis;
		return redis;
	}

	bool isConnected(const std::string& which) { return context_[which] ? true : false; }

	void set(const std::string& key, const std::string& value);
	std::string get(const std::string& key);
private:
	UniqueRedis();
	void free();

	ManyRedis(const ManyRedis&) = delete;
	const ManyRedis& operator=(const ManyRedis&) = delete;

	std::map<std::string, UniqueContext> contexts_;
};

} // namespace TTR


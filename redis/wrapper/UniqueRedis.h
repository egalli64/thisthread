/**
 * UniqueRedis.h
 */

#pragma once

#include <iostream>
#include <memory>
class redisContext;

namespace TTR
{
typedef std::unique_ptr<redisContext, std::function<void(redisContext*)>> UniqueContext;

class UniqueRedis
{
public:
	static UniqueRedis& instance()
	{
		static UniqueRedis redis;
		return redis;
	}

	bool isConnected() { return spContext_ ? true : false; }

	void set(const std::string& key, const std::string& value);
	std::string get(const std::string& key);
private:
	UniqueRedis();
	void free();

	UniqueRedis(const UniqueRedis&) = delete;
	const UniqueRedis& operator=(const UniqueRedis&) = delete;

	UniqueContext spContext_;
};

} // namespace TTR


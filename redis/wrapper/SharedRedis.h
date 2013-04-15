/**
 * SharedRedis.h
 * More info on http://thisthread.blogspot.com/2013/04/connecting-to-multiple-redis-servers.html
 */

#pragma once

#include <iostream>
#include <memory>
class redisContext;

namespace TTR
{
typedef std::shared_ptr<redisContext> SharedContext;

class SharedRedis
{
    enum { DEFAULT_PORT = 6379 };
public:
    SharedRedis(const std::string& server, int port = DEFAULT_PORT);
    SharedRedis(int port = DEFAULT_PORT) : SharedRedis("localhost", port) {}

    bool isConnected() { return spContext_ ? true : false; }

    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
private:
    void free();
    SharedContext spContext_;
};

} // namespace TTR


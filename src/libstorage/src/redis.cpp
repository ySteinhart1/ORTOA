#include "redis.h"

using namespace sw::redis;

redisCli::redisCli() {
}

void redisCli::reconnect() {
    this->redisConn = sw::redis::Redis("tcp://127.0.0.1:6379");
}

std::string redisCli::get(const std::string &key) {
    return this->redisConn.get(key).value_or("");
}

sw::redis::Pipeline redisCli::pipe() {
    return this->redisConn.pipeline();
}

void redisCli::put(const std::string &key, const std::string &value) {
    this->redisConn.set(key, value);
}
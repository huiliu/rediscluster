#include "common.h"
#include "../libhiredis-cluster/src/hiredis.h"
#include <cassert>
#include <cstdio>
#include <thread>

redisContext* create_connection(const char*, int);

static const char* ip = "127.0.0.1";
static const int port = 6379;

static redisContext* create_connection(const char* ip, int port)
{
    redisContext* c = redisConnect(ip, port);
    if (nullptr == c)
        return nullptr;

    if (0 != c->err)
    {
        printf("failed to connect %s:%d. reason: %s\n", ip, port, c->errstr);
        redisFree(c);
        return nullptr;
    }

    return c;
}

static void incr_case()
{
    auto* c = create_connection(ip, port);
    assert(nullptr != c);

    static const char* COMMAND = "INCR xxx";
    redisReply* reply = nullptr;
    int ret = REDIS_ERR;

    for (size_t i = 0; i < 100000; i++)
    {
        reply = (redisReply*) redisCommand(c, COMMAND);
        if (nullptr == reply)
        {
            printf("execute command[%s]. reason: %s\n", COMMAND, c->errstr);
            freeReplyObject(reply);
            break;
        }
        freeReplyObject(reply);
    }

    redisFree(c);
}

static void get_command(const char* key)
{
    auto* c = create_connection(ip, port);
    assert(nullptr != c);

    redisReply* reply = nullptr;
    int ret = REDIS_ERR;

    reply = (redisReply*) redisCommand(c, "GET %s", key);
    if (nullptr == reply)
        printf("execute command[GET %s]. reason: %s\n", key, c->errstr);

    printf("reply type[%d] int[%lld] str[%s]\n", reply->type, reply->integer, reply->str);
    freeReplyObject(reply);
    redisFree(c);
}

void other()
{
    incr_case();
}

int case_multi_thread(const char*)
{
    std::thread thr(other);
    incr_case();
    thr.join();

    get_command("xxx");

    return 0;
}

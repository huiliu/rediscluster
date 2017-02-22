#include "common.h"
#include<stdio.h>

int case02(const char* addr)
{
    redisClusterContext *cc = redisClusterConnect(addr, HIRCLUSTER_FLAG_NULL);
    redisReply *reply;
    if (cc == NULL || cc->err)
    {
        printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
        return -1;
    }

    redisClusterAppendCommand(cc, "set key1 value1");
    redisClusterAppendCommand(cc, "get key1");
    redisClusterAppendCommand(cc, "mset key2 value2 key3 value3");
    redisClusterAppendCommand(cc, "mget key2 key3");
    redisClusterAppendCommand(cc, "hmset kk f1 1 f2 2");
    redisClusterAppendCommand(cc, "hmget kk f1 f2");

    redisClusterGetReply(cc, (void**)&reply);  //for "set key1 value1"
    freeReplyObject(reply);
    redisClusterGetReply(cc, (void**)&reply);  //for "get key1"
    fprintf(stdout, "get reply: %s\n", reply->str);
    freeReplyObject(reply);
    redisClusterGetReply(cc, (void**)&reply);  //for "mset key2 value2 key3 value3"
    freeReplyObject(reply);
    redisClusterGetReply(cc, (void**)&reply);  //for "mget key2 key3"
    fprintf(stdout, "mget reply type: %d\n", reply->type);
    freeReplyObject(reply);
    redisClusterGetReply(cc, (void**)&reply);  //for "hmset key3 field1 value2 ..."
    fprintf(stdout, "hmset reply type: %d str: %s\n", reply->type, reply->str);
    freeReplyObject(reply);
    redisClusterGetReply(cc, (void**)&reply);  //for "hmget ..."
    fprintf(stdout, "hmget reply type: %d str: %s\n", reply->type, reply->str);
    freeReplyObject(reply);

    redisClusterReset(cc);

    redisClusterFree(cc);
    return 0;
}
#include "common.h"
#include<stdio.h>

int case01(const char* addr)
{
    char *key = "key-a";
    char *field = "field-1";
    char *key1 = "key1";
    char *value1 = "value-1";
    char *key2 = "key1";
    char *value2 = "value-1";

    redisClusterContext *cc = redisClusterConnect(addr, HIRCLUSTER_FLAG_NULL);
    if (cc == NULL || cc->err)
    {
        printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
        return -1;
    }

    redisReply *reply = (redisReply*)redisClusterCommand(cc, "hmget %s %s", key, field);
    if (reply == NULL)
    {
        printf("reply is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }

    printf("reply->type:%d value: %s", reply->type, reply->str);

    freeReplyObject(reply);

    reply = (redisReply*)redisClusterCommand(cc, "mset %s %s %s %s", key1, value1, key2, value2);
    if (reply == NULL)
    {
        printf("reply is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }

    printf("reply->str:%s", reply->str);

    freeReplyObject(reply);
    redisClusterFree(cc);
    return 0;
}
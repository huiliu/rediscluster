#include "common.h"
#include "libevent.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <thread>


void getCallback(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = (redisReply*)r;
    if (reply == NULL) return;
    printf("argv[%s]: %s\n", (char*)privdata, reply->str);

    /* Disconnect after receiving the reply to GET */
    redisAsyncDisconnect(c);
}

void connectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Connected...\n");
}

void disconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Disconnected...\n");
}

struct event_base *base;
void
get_domain()
{
    redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);

    redisLibeventAttach(c,base);
    redisAsyncSetConnectCallback(c,connectCallback);
    redisAsyncSetDisconnectCallback(c,disconnectCallback);

    for (size_t i = 0; i < 100; i++)
    {
        redisAsyncCommand(c, getCallback, (char*)"end-1", "GET k1");
    }
}
// ���߳��Ƿ��������ɷ���˿��Ƶġ�
// ����redis������ǵ��̵߳ģ������ܱ�֤�ȴﵽ�������ȴ�
// �첽��Ϣ�������ڿͻ��˱����Ѹ�������Ļص����У��յ�һ���𸴣���ͷ��ȡ���ص���ִ��
int async_case()
{
    // signal(SIGPIPE, SIG_IGN);
    base = event_base_new();

    redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);

    if (c->err) {
        /* Let *c leak for now... */
        printf("Error: %s\n", c->errstr);
        return 1;
    }

    redisLibeventAttach(c,base);
    redisAsyncSetConnectCallback(c,connectCallback);
    redisAsyncSetDisconnectCallback(c,disconnectCallback);

    redisAsyncCommand(c, NULL, NULL, "SET k1 %s", "abc1");
    redisAsyncCommand(c, NULL, NULL, "SET k2 %s", "abc2");

    std::thread thr(get_domain);

    for (size_t i = 0; i < 100; i++)
    {
        redisAsyncCommand(c, getCallback, (char*)"end-2", "GET k2");
    }

    thr.join();

    event_base_dispatch(base);
    return 0;
}

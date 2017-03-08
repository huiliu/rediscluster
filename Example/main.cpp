#include "common.h"
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

class WSocketGuard
{
public:
    WSocketGuard()
    {
        // Initialize Winsock
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR) {
            printf("WSAStartup failed: %d\n", iResult);
        }
    }

    ~WSocketGuard()
    {
        WSACleanup();
    }
};

int main()
{
    WSocketGuard wguard;
    const char* addr = "127.0.0.1:7000,127.0.0.1:7001,127.0.0.1:7002";
    // case01(addr);
    // case02(addr);
    // case_multi_thread(addr);
    async_case();
    return 0;
}


// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "NetworkHelper.h"
#include <WinSock2.h>
#include <ws2tcpip.h>

//#pragma comment (lib, "Ws2_32.lib")
using namespace std;

uint32_t ResolveIpAddress(const string& address)
{
    uint32_t resolvedAddress = 0;

    if (address == "255.255.255.255")
    {
        resolvedAddress = INADDR_BROADCAST;
    }
    else
    {
        if (inet_pton(AF_INET, address.c_str(), &resolvedAddress) <= 0)
        {
            addrinfo hints;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET;
            addrinfo* pAddrInfo = nullptr;
            if (getaddrinfo(address.c_str(), NULL, &hints, &pAddrInfo) == 0 && pAddrInfo)
            {
                resolvedAddress = reinterpret_cast<sockaddr_in*>(pAddrInfo->ai_addr)->sin_addr.s_addr;
                freeaddrinfo(pAddrInfo);
            }
            else
            {
                resolvedAddress = 0;
            }
        }
    }

    return resolvedAddress;
}

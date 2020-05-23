// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TcpSocket.h"
#include <Winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

SocketInit::SocketInit()
{
    WSADATA init;
    WSAStartup(MAKEWORD(2, 2), &init);
}

SocketInit::~SocketInit()
{
    WSACleanup();
}

TcpSocket::~TcpSocket()
{
    close();
}

TcpSocket::ConnectResult TcpSocket::connect(const char* pAddress, uint16_t port, uint32_t sendTimeoutMillseconds)
{
    ConnectResult result = ResolveAddressFailed;

    close();

    SOCKADDR_IN addr;

    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    addrinfo* pResult = nullptr;
    if (getaddrinfo(pAddress, nullptr, &hints, &pResult) == 0 && pResult)
    {
        addr.sin_addr.s_addr = reinterpret_cast<sockaddr_in*>(pResult->ai_addr)->sin_addr.s_addr;
        freeaddrinfo(pResult);

        result = connect(&addr, sendTimeoutMillseconds);
    }
    else
    {
        setLastError();
    }

    return result;
}

TcpSocket::ConnectResult TcpSocket::connect(const void* pSockAddr, uint32_t sendTimeoutMillseconds)
{
    ConnectResult result = ConnectFailed;

    if (pSockAddr)
    {
        close();

        _socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (isValid())
        {
            int option = 1;

            ::setsockopt(_socket, SOL_SOCKET, TCP_NODELAY  , reinterpret_cast<char*>(&option), sizeof(option));
            ::setsockopt(_socket, SOL_SOCKET, SO_DONTLINGER, reinterpret_cast<char*>(&option), sizeof(option));
            ::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR , reinterpret_cast<char*>(&option), sizeof(option));

            if (::connect(_socket, reinterpret_cast<const sockaddr*>(pSockAddr), sizeof(sockaddr)) == SOCKET_ERROR)
            {
                setLastError();
                close();
            }
            else
            {
                ::setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&sendTimeoutMillseconds), sizeof(sendTimeoutMillseconds));

                constexpr size_t MaxPortLen = 6;
                constexpr size_t MaxHostLen = NI_MAXHOST + MaxPortLen;

                _address.resize(MaxHostLen);

                getnameinfo(reinterpret_cast<const sockaddr*>(pSockAddr), sizeof(sockaddr), _address.data(), MaxHostLen, nullptr, 0, NI_NUMERICHOST);

                _address.resize(strlen(_address.data()));
                _address += ':';
                _address += to_string(reinterpret_cast<const sockaddr_in*>(pSockAddr)->sin_port);

                result = Connected;
            }
        }
    }

    return result;
}

void TcpSocket::setReceiveTimeout(uint32_t timeoutMillseconds)
{
    if (isValid())
    {
        ::setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&timeoutMillseconds), sizeof(timeoutMillseconds));
    }
}

void TcpSocket::close()
{
    _address.resize(0);

    if (_socket != INVALID_SOCKET)
    {
        ::closesocket(_socket);
        _socket = INVALID_SOCKET;
    }
}

int TcpSocket::selectRead(long timeout, bool timeoutIsSeconds) //-V126
{
    TIMEVAL tv;
    if (timeoutIsSeconds)
    {
        tv.tv_sec  = timeout;
        tv.tv_usec = 0;
    }
    else
    {
        tv.tv_sec  = timeout / MILLISECONDS_PER_SECOND;
        tv.tv_usec = (timeout % MILLISECONDS_PER_SECOND) * MILLISECONDS_PER_SECOND;
    }

    fd_set readFd;
    readFd.fd_count = 1;
    readFd.fd_array[0] = _socket;

    return select(0, &readFd, nullptr, nullptr, &tv);
}

int TcpSocket::selectWrite(long timeout, bool timeoutIsSeconds) //-V126
{
    TIMEVAL tv;
    if (timeoutIsSeconds)
    {
        tv.tv_sec  = timeout;
        tv.tv_usec = 0;
    }
    else
    {
        tv.tv_sec  = timeout / MILLISECONDS_PER_SECOND;
        tv.tv_usec = (timeout % MILLISECONDS_PER_SECOND) * MILLISECONDS_PER_SECOND;
    }

    fd_set writeFd;
    writeFd.fd_count = 1;
    writeFd.fd_array[0] = _socket;

    return select(0, nullptr, &writeFd, nullptr, &tv);
}

int TcpSocket::receive(void* pBuffer, int bufferLen)
{
    return ::recv(_socket, reinterpret_cast<char*>(pBuffer), bufferLen, 0);
}

int TcpSocket::send(const void* pData, int dataLen)
{
    return ::send(_socket, reinterpret_cast<const char*>(pData), dataLen, 0);
}

int TcpSocket::getLastError() const
{
    return _lastError;
}

int TcpSocket::setLastError()
{
    _lastError = GetLastError();
    return _lastError;
}

int TcpSocket::GetLastError()
{
    return WSAGetLastError();
}
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <functional>
#include <string>
#include "Util/Time/Defines.Time.h"

struct SocketInit final
{
    SocketInit();
    ~SocketInit();
};

/// <summary>
/// Class for a TCP Socket.
/// </summary>
/// <remarks>
/// SocketInit must be instantiated while this is in use.
/// </remarks>
class TcpSocket
{
public:
    /// <summary>
    /// TcpSocket constructor.
    /// </summary>
    TcpSocket() = default;

    TcpSocket(const TcpSocket&) = delete;
    void operator=(const TcpSocket&) = delete;

    /// <summary>
    /// TcpSocket destructor.
    /// </summary>
    virtual ~TcpSocket();

    static constexpr uint64_t InvalidSocket{ static_cast<uint64_t>(-1) };
    static constexpr int      SocketError  { -1 };

    static constexpr int      ConnectionRefused{ 10061 };
    static constexpr int      HostNotFound     { 11001 };

    enum ConnectResult
    {
        Connected,
        ResolveAddressFailed,
        ConnectFailed
    };

    /// <summary>
    /// Creates a TCP socket and connects to pAddress and port.
    /// </summary>
    /// <param name="pAddress">The address to which to connect.</param>
    /// <param name="sendTimeoutMillseconds">The timeout to set for sends.</param>
    ConnectResult connect(const char* pAddress, uint16_t port, uint32_t sendTimeoutMillseconds = 15 * MILLISECONDS_PER_SECOND);

    /// <summary>
    /// Creates a TCP socket and connects to addr.
    /// </summary>
    /// <param name="pSockAddr">A pointer to the sockaddr to which to connect.</param>
    /// <param name="sendTimeoutMillseconds">The timeout to set for sends.</param>
    ConnectResult connect(const void* pSockAddr, uint32_t sendTimeoutMillseconds = 15 * MILLISECONDS_PER_SECOND);

    /// <summary>
    /// Sets the receive timeout.
    /// </summary>
    /// <param name="timeoutMillseconds">Sets the SO_RCVTIMEO option.</param>
    void setReceiveTimeout(uint32_t timeoutMillseconds = 15 * MILLISECONDS_PER_SECOND);

    /// <summary>
    /// Closes the socket.
    /// </summary>
    virtual void close();

    /// <summary>
    /// Indicates whether the contained socket handle is not INVALID_SOCKET.
    /// </summary>
    /// <returns><b>true</b> if the contains socket handle is not INVALID_SOCKET,
    ///     else <b>false</b>.</returns>
    virtual bool isValid() const        { return _socket != InvalidSocket; }

    /// <summary>
    /// Check if socket is ready for reading.
    /// </summary>
    /// <param name="timeout">The time to wait for the status of the socket.</param>
    /// <param name="timeoutIsSeconds">If <b>true</b>, the timeout is in seconds, else milliseconds.</param>
    /// <returns>1 if the socket is ready to read, 0 if a timeout occurred, else SocketError.</returns>
    virtual int selectRead(long timeout, bool timeoutIsSeconds = true); //-V126

    /// <summary>
    /// Check if socket is ready for writing.
    /// </summary>
    /// <param name="timeoutMilliseconds">The time to wait for the status of the socket.</param>
    /// <param name="timeoutIsSeconds">If <b>true</b>, the timeout is in seconds, else milliseconds.</param>
    /// <returns>1 if the socket is ready to write, 0 if a timeout occurred, else SocketError.</returns>
    virtual int selectWrite(long timeout, bool timeoutIsSeconds = true); //-V126

    /// <summary>
    /// Receive data.
    /// </summary>
    /// <param name="pBuffer">A buffer to receive the data.</param>
    /// <param name="bufferLen">The length, in bytes, of <paramref name="pBuffer"/>.</param>
    /// <returns>Bytes received or SocketError.</returns>
    virtual int receive(void* pBuffer, int bufferLen);

    /// <summary>
    /// Sends data.
    /// </summary>
    /// <param name="pData">The data to send.</param>
    /// <param name="dataLen">The length, in bytes, of <paramref name="pData"/>.</param>
    /// <returns>Bytes sent or SocketError.</returns>
    virtual int send(const void* pData, int dataLen);

    /// <summary>
    /// Returns the last recorded error.
    /// </summary>
    /// <returns>The last recorded error.</returns>
    /// <remarks>
    /// This is automatically set for the connect methods. For all other methods, there
    //  are no intervening calls to the OS, so either setLastError() or GetLastError()
    /// should be immediately called if the method returns -1 and the error code is
    //  wanted.
    /// </remarks>
    int getLastError() const;

    /// <summary>
    /// Sets the last recorded error and then returns it.
    /// </summary>
    /// <returns>The last recorded error.</returns>
    /// <remarks>
    /// This is called internally by the connect methods. For all other methods, there
    //  are no intervening calls to the OS, so either setLastError() or GetLastError()
    /// should be immediately called if the method returns -1 and the error code is
    //  wanted.
    /// </remarks>
    int setLastError();

    /// <summary>
    /// Returns the last error.
    /// </summary>
    /// <returns>The last error.</returns>
    /// <remarks>
    /// For accurate results, this must be called immediately after the failed method.
    ///
    /// For Windows, this returns the result of WSAGetLastError(), for other systems
    ///     this returns errno.
    /// </remarks>
    static int GetLastError();

private:
    uint64_t    _socket   { InvalidSocket };
    std::string _address;
    int         _lastError{ 0 };
};

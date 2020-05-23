// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <queue>
#include "WinHandle.h"

enum class WinQueueResult
{
    Success,
    Closed, // Not used with pop
    ShuttingDown,
    Timeout,
    Empty,  // Not used with push
    Full,   // Not used with pop
};

template <class T, size_t MaxSize = INT32_MAX>
class WinSafeQueue
{
public:
    WinSafeQueue(size_t maxSize = MaxSize)
        : _maxSize(maxSize)
        , _closedEvent(TRUE)
        , _shutdownEvent(TRUE)
        , _itemQueued(FALSE)
        , _queueMutex(FALSE)
    {
    }
    WinSafeQueue(HANDLE shutdownEvent, size_t maxSize = MaxSize)
        : _maxSize(maxSize)
        , _closedEvent(TRUE)
        , _shutdownEvent(shutdownEvent, true)
        , _itemQueued(FALSE)
        , _queueMutex(FALSE)
    {
    }
    WinSafeQueue(const WinSafeQueue&) = delete;
    void operator=(const WinSafeQueue&) = delete;
    ~WinSafeQueue()
    {
        close();
        _shutdownEvent.Set();
        Sleep(0);
    }

    void setShutdownEvent(HANDLE hHandle, bool dupHandle = false)
    {
        _shutdownEvent.SetHandle(hHandle, dupHandle);
    }

    void setShutdownEvent()
    {
        _shutdownEvent.Set();
    }

    WinQueueResult push(T item, DWORD timeoutMilliseconds = 10)
    {
        auto result = WinQueueResult::Closed;

        if (isOpen())
        {
            HANDLE handles[3]
            {
                _shutdownEvent,
                _closedEvent,
                _queueMutex
            };

            DWORD waitResult = ::WaitForMultipleObjects(3, handles, FALSE, timeoutMilliseconds);
            switch (waitResult)
            {
                case WAIT_OBJECT_0:
                case WAIT_ABANDONED_2:
                    result = WinQueueResult::ShuttingDown;
                    break;

                case WAIT_OBJECT_1:
                    break;

                case WAIT_OBJECT_2:
                {
                    if (isOpen())
                    {
                        if (_queue.size() < _maxSize)
                        {
                            _queue.push(move(item));
                            _itemQueued.Set();

                            result = WinQueueResult::Success;
                        }
                        else
                        {
                            result = WinQueueResult::Full;
                        }
                    }
                    _queueMutex.Release();
                    break;
                }

                default:
                    result = WinQueueResult::Timeout;
                    break;
            }
        }

        return result;
    }

    WinQueueResult pop(T& item, DWORD timeoutMilliseconds = INFINITE)
    {
        auto result = WinQueueResult::Success;

        HANDLE handles[2]
        {
            _shutdownEvent,
            _itemQueued
        };

        DWORD waitResult = ::WaitForMultipleObjects(2, handles, FALSE, timeoutMilliseconds);
        switch (waitResult)
        {
            case WAIT_OBJECT_0:
                clear();
                result = WinQueueResult::ShuttingDown;
                break;

            case WAIT_TIMEOUT:
                result = WinQueueResult::Timeout;
                break;

            default:
                break;
        }

        if (result == WinQueueResult::Success)
        {
            _queueMutex.WaitFor(INFINITE);
            if (_queue.empty())
            {
                result = WinQueueResult::Empty;
            }
            else
            {
                item = move(_queue.front());
                _queue.pop();
                result = WinQueueResult::Success;
            }
            _queueMutex.Release();
        }

        return result;
    }

    bool isOpen() const
    {
        return _closedEvent.IsNotSet();
    }

    void open()
    {
        _closedEvent.Reset();
    }

    void close()
    {
        _closedEvent.Set();
    }

private:
    void clear()
    {
        _queueMutex.WaitFor(INFINITE);
        {
            std::queue<T> tmpQueue;
            _queue.swap(tmpQueue);
        }
        _queueMutex.Release();
    }

private:
    size_t            _maxSize;
    std::queue<T>     _queue;
    WinEvent          _closedEvent;
    WinEvent          _shutdownEvent;
    WinEvent          _itemQueued;
    WinMutex          _queueMutex;
};

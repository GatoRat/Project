// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

template <class T, size_t MaxSize = INT32_MAX>
class SafeQueueWaitable
{
public:
    SafeQueueWaitable(size_t maxSize = MaxSize, bool isOpen = true)
        : _isOpen { isOpen  }
        , _maxSize{ maxSize }
    {
    }
    explicit SafeQueueWaitable(bool isOpen, size_t maxSize = MaxSize)
        : _isOpen{ isOpen }
        , _maxSize{ maxSize }
    {
    }
    SafeQueueWaitable(const SafeQueueWaitable&) = delete;
    void operator=(const SafeQueueWaitable&) = delete;
    ~SafeQueueWaitable()
    {
        clear();
    }

    /**
        @param pQueueIsFull A pointer to a bool which will be set to true if item
            is not queued because queue is full.
    */
    bool push(T item, bool* pQueueIsFull = nullptr)
    {
        bool success = false;

        if (pQueueIsFull)
        {
            *pQueueIsFull = false;
        }

        if (isOpen())
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_queue.size() < _maxSize)
            {
                _queue.push(move(item));
                success = true;

                lock.unlock();
                _conditionVariable.notify_one();
            }
            else if (pQueueIsFull)
            {
                *pQueueIsFull = true;
            }
        }

        return success;
    }

    bool pop(T& item, bool wait = true)
    {
        bool success = false;

        std::unique_lock<std::mutex> lock(_mutex);
        if (wait)
        {
            while (_queue.empty())
            {
                _conditionVariable.wait(lock);
            }
        }

        if (!_queue.empty())
        {
            item = move(_queue.front());
            _queue.pop();
            success = true;
        }

        return success;
    }

    bool pop(T& item, unsigned timeoutMilliseconds)
    {
        bool success = false;

        std::unique_lock<std::mutex> lock(_mutex);
        if (_queue.empty() && timeoutMilliseconds)
        {
            if (timeoutMilliseconds == UINT32_MAX)
            {
                _conditionVariable.wait(lock, [&]()->bool
                {
                    return !_queue.empty();
                });
            }
            else
            {
                _conditionVariable.wait_for(lock, std::chrono::milliseconds(timeoutMilliseconds), [&]()->bool
                {
                    return !_queue.empty();
                });
            }
        }

        if (!_queue.empty())
        {
            item = move(_queue.front());
            _queue.pop();
            success = true;
        }

        return success;
    }

    bool isOpen() const
    {
        return _isOpen;
    }

    void open()
    {
        _isOpen = true;
    }

    void close(bool notify = false)
    {
        _isOpen = false;
        if (notify)
        {
            _conditionVariable.notify_one();
        }
    }

private:
    void clear()
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _isOpen = false;
            std::queue<T> tmpQueue;
            _queue.swap(tmpQueue);
        }
        _conditionVariable.notify_one();
    }

private:
    std::atomic<bool> _isOpen = true;
    size_t            _maxSize;
    std::mutex        _mutex;
    std::queue<T>     _queue;
    std::condition_variable _conditionVariable;
};

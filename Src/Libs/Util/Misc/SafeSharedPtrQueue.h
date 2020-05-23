// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <atomic>
#include <chrono>
#include <mutex>
#include <queue>

template <class T, size_t MaxSize = INT32_MAX>
class SafePtrQueue
{
public:
    SafePtrQueue(size_t maxSize = MaxSize)
        : _maxSize(maxSize)
    {
    }
    SafePtrQueue(const SafePtrQueue&) = delete;
    void operator=(const SafePtrQueue&) = delete;
    ~SafePtrQueue()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _isOpen = false;
        std::queue<std::shared_ptr<T>> tmpQueue;
        _queue.swap(tmpQueue);
    }

    bool push(T item)
    {
        bool success = false;

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
        }

        return success;
    }

    bool pop(T& item)
    {
        bool success = false;

        if (isOpen())
        {
            std::unique_lock<std::mutex> lock(_mutex);

            if (!_queue.empty() && isOpen())
            {
                item = move(_queue.front());
                _queue.pop();
                success = true;
            }
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

    void close()
    {
        _isOpen = false;
    }

private:
    std::atomic<bool> _isOpen = true;
    size_t _maxSize;
    std::mutex _mutex;
    std::queue<std::shared_ptr<T>> _queue;
};

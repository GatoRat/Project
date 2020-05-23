// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

template <class T, size_t MaxSize = INT32_MAX>
class SafeQueue
{
public:
    SafeQueue(size_t maxSize = MaxSize)
        : _maxSize(maxSize)
    {
    }
    SafeQueue(const SafeQueue&) = delete;
    void operator=(const SafeQueue&) = delete;
    ~SafeQueue()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        std::queue<T> tmpQueue;
        _queue.swap(tmpQueue);
    }

    bool push(T item)
    {
        bool success = false;

        std::unique_lock<std::mutex> lock(_mutex);
        if (_queue.size() < _maxSize)
        {
            _queue.push(move(item));
            success = true;

            lock.unlock();
        }

        return success;
    }

    bool pop(T& item)
    {
        bool success = false;

        std::unique_lock<std::mutex> lock(_mutex);
        if (!_queue.empty())
        {
            item = move(_queue.front());
            _queue.pop();
            success = true;
        }

        return success;
    }

private:
    size_t        _maxSize;
    std::mutex    _mutex;
    std::queue<T> _queue;
};

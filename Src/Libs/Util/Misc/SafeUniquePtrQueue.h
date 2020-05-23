#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template <class T>
class SafeUniquePtrQueue
{
public:
    SafeUniquePtrQueue() = default;
    SafeUniquePtrQueue(const SafeUniquePtrQueue&) = delete;
    void operator=(const SafeUniquePtrQueue&) = delete;
    ~SafeUniquePtrQueue()
    {
        close();
        pop();
    }

    void push(std::unique_ptr<T>&& item)
    {
        if (isOpen())
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _queue.push(move(item));
            }
            _conditionVariable.notify_one();
        }
    }

    std::unique_ptr<T> pop()
    {
        if (isOpen())
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while (_queue.empty() && isOpen())
            {
                _conditionVariable.wait(lock);
            }

            if (!_queue.empty())
            {
                std::unique_ptr<T> val = move(q.front());
                q.pop();
                return val;
            }
        }
        return std::unique_ptr<T>();
    }

    bool isOpen() const
    {
        return _isOpen;
    }

    void close() const
    {
        _isOpen = false;
    }

private:
    std::atomic<bool> _isOpen = true;
    std::mutex _mutex;
    std::queue<T> _queue;
    std::condition_variable _conditionVariable;
};

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <condition_variable>
#include <mutex>

class WaitEvent
{
public:
    WaitEvent() = default;
    WaitEvent(const WaitEvent&) = delete;
    void operator=(const WaitEvent&) = delete;
    ~WaitEvent() = default;

    void set();
    void reset();
    void wait();
    bool waitFor(int64_t timeoutMilliseconds);

private:
    bool _isSet = false;
    std::mutex _mutex;
    std::condition_variable _cv;
};

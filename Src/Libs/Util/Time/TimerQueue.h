// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <functional>
#include <map>

/*
class TimerQueue
{
public:
    TimerQueue() = default;
    TimerQueue(const TimerQueue&) = delete;
    void operator=(const TimerQueue&) = delete;
    ~TimerQueue() = default;

    typedef std::function<void()> TimerCallback;

    uint64_t startTimer(bool oneShot, float duration, TimerCallback timerCallback);
    void stopTimer(uint64_t key);
    void update(float dt);

private:
    struct TimerItem
    {
        TimerCallback callback;
        float duration;
        float elapsed;
        bool oneShot;
    };
    std::map<uint64_t, TimerItem> _timers;
    uint64_t _nextKey{ 1 };
};
*/

class TimerQueue
{
public:
    TimerQueue() = default;
    TimerQueue(const TimerQueue&) = delete;
    void operator=(const TimerQueue&) = delete;
    ~TimerQueue() = default;

    class Key;
    typedef std::function<void()> TimerCallback;
    Key startTimer(bool oneShot, float duration, TimerCallback timerCallback);
    void stopTimer(Key key);
    void update(float dt);

private:
    struct TimerItem
    {
        TimerCallback callback;
        float duration;
        float elapsed;
        bool oneShot;
    };
    std::map<uint64_t, TimerItem> _timers;
    uint64_t _nextKey{ 1 };
};

class TimerQueue::Key
{
public:
    Key() = default;

private:
    friend TimerQueue;
    explicit Key(uint64_t key) : _key{ key } {};
    uint64_t _key{ 0 };
};

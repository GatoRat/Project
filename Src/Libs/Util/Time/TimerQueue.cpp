// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TimerQueue.h"

/*
uint64_t TimerQueue::startTimer(bool oneShot, float duration, TimerCallback timerCallback)
{
    TimerItem item;
    item.callback = timerCallback;
    item.duration = duration;
    item.elapsed = 0.0f;
    item.oneShot = oneShot;

    uint64_t key = _nextKey++;
    _timers[key] = item;
    return key;
}

void TimerQueue::update(float dt)
{
    for (auto it = _timers.begin(); it != _timers.end();)
    {
        it->second.elapsed += dt;
        if (it->second.elapsed >= it->second.duration)
        {
            it->second.callback();
            if (it->second.oneShot)
            {
                it = _timers.erase(it);
                continue;
            }
            it->second.elapsed = 0.0f;
        }
        ++it;
    }
}

void TimerQueue::stopTimer(uint64_t key)
{
    auto it = _timers.find(key);
    if (it != _timers.end())
    {
        _timers.erase(it);
    }
}
*/

TimerQueue::Key TimerQueue::startTimer(bool oneShot, float duration, TimerCallback timerCallback)
{
    TimerItem item;
    item.callback = move(timerCallback);
    item.duration = duration;
    item.elapsed  = 0.0f;
    item.oneShot  = oneShot;

    const uint64_t key = _nextKey++;
    _timers[key] = item;
    return Key(key);
}

void TimerQueue::update(float dt)
{
    for (auto it = _timers.begin(); it != _timers.end();)
    {
        it->second.elapsed += dt;
        if (it->second.elapsed >= it->second.duration)
        {
            it->second.callback();
            if (it->second.oneShot)
            {
                it = _timers.erase(it);
                continue;
            }
            it->second.elapsed = 0.0f;
        }
        ++it;
    }
}

void TimerQueue::stopTimer(Key key)
{
    _timers.erase(key._key);
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <functional>
#include <map>
#include "TimeStamp.h"

class TimeStampQueue
{
public:
    TimeStampQueue() = default;
    TimeStampQueue(const TimeStampQueue&) = delete;
    void operator=(const TimeStampQueue&) = delete;
    ~TimeStampQueue() = default;

    class Key;
    typedef std::function<void()> Callback;
    Key  startTimer(TimeStamp timeStamp, Callback timeStampCallback);
    void stopTimer(Key key);

    void update(TimeStamp timeStamp);
    void update();

private:
    struct Item
    {
        Callback callback;
        TimeStamp timeStamp;
    };
    std::map<uint64_t, Item> _timers;
    uint64_t _nextKey{ 1 };
};

class TimeStampQueue::Key
{
public:
    Key() = default;

private:
    friend TimeStampQueue;
    explicit Key(uint64_t key) : _key{ key } {};
    uint64_t _key{ 0 };
};

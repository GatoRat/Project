// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TimeStampQueue.h"

TimeStampQueue::Key TimeStampQueue::startTimer(TimeStamp timeStamp, Callback timeStampCallback)
{
    Item item;
    item.callback = move(timeStampCallback);
    item.timeStamp = timeStamp;

    const uint64_t key = _nextKey++;
    _timers[key] = item;
    return key;
}

void TimeStampQueue::update(TimeStamp timeStamp)
{
    for (auto it = _timers.begin(); it != _timers.end();)
    {
        if (it->second.timeStamp <= timeStamp)
        {
            it->second.callback();
            it = _timers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void TimeStampQueue::update()
{
    TimeStamp timeStamp(TimeStamp::Set::ToCurTime);
    update(timeStamp);
}

void TimeStampQueue::stopTimer(Key key)
{
    _timers.erase(key._key);
}

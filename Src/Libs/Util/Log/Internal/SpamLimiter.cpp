// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SpamLimiter.h"
#include <ctime>
#include "../../Secure/Crc32c.h"
#include "../../String/Str.h"

using namespace std;

SpamLimiter::SpamLimiter(int16_t timeoutSeconds, int16_t maxItems)
    : _timeoutSeconds{ timeoutSeconds <= 0 ? 0 : static_cast<time_t>(timeoutSeconds) }
    , _maxItems{ maxItems <= 0 ? 0 : static_cast<size_t>(maxItems) }
{
}

void SpamLimiter::setLimits(int16_t timeLimitSeconds, int16_t maxItems)
{
    if (maxItems < 0)
    {
        _cache.clear();
    }
    else if (maxItems < INT16_MAX)
    {
        if (static_cast<size_t>(maxItems) < _maxItems)
        {
            _cache.clear();
        }
        _maxItems = static_cast<size_t>(maxItems);
    }

    if (timeLimitSeconds >= 0)
    {
        _timeoutSeconds = static_cast<time_t>(timeLimitSeconds);
    }
}

bool SpamLimiter::check(string_view logStr, time_t curTime)
{
    bool doLog = true;

    if (_timeoutSeconds > 0 && _maxItems > 0)
    {
        if (curTime == 0)
        {
            curTime = time(nullptr);
        }

        const uint32_t checksum = Crc32c(logStr);
        auto it = _cache.find(checksum);
        if (it == _cache.end())
        {
            if (_maxItems == _cache.size())
            {
                removeExpired(true, curTime);
            }

            _cache[checksum] = curTime + _timeoutSeconds;
        }
        else if (curTime < it->second)
        {
            doLog = false;
        }
        else
        {
            it->second = curTime + _timeoutSeconds;
        }
    }

    return doLog;
}

void SpamLimiter::removeExpired(bool plusOne, time_t curTime)
{
    if (curTime == 0)
    {
        curTime = time(nullptr);
    }

    auto lowestIt = _cache.begin();
    time_t lowestExpiration = INT64_MAX;

    for (auto it = _cache.begin(); it != _cache.end();)
    {
        if (it->second <= curTime)
        {
            it = _cache.erase(it);
            continue;
        }

        if (plusOne && it->second < lowestExpiration)
        {
            lowestExpiration = it->second;
            lowestIt = it;
        }

        ++it;
    }

    if (plusOne && _maxItems == _cache.size())
    {
        _cache.erase(lowestIt);
    }
}

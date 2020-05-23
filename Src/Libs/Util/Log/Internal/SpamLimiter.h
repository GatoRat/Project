// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include <map>
#include <string_view>
#include <thread>

class SpamLimiter
{
public:
    SpamLimiter(int16_t timeoutSeconds, int16_t maxItems);
    ~SpamLimiter() = default;

    /// <summary>
    /// Sets spam limits.
    /// </summary>
    /// <param name="timeLimitSeconds">The number of seconds during which an error or warning log entry won't be repeated.</param>
    /// <param name="maxItems">The maximum items to cache. A negative number will reset the cache. A zero will be ignored.</param>
    void setLimits(int16_t timeLimitSeconds, int16_t maxItems);

    bool check(std::string_view logStr, time_t curTime = 0);

    void removeExpired(bool plusOne = false, time_t curTime = 0);

protected:
    time_t _timeoutSeconds;
    size_t _maxItems;
    std::map<uint32_t, time_t> _cache;
};

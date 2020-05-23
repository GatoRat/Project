// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Log/Internal/SpamLimiter.h"
#include "TestHelper/catch.hpp"

using namespace std;

class SpamLimiterTest : public SpamLimiter
{
public:
    SpamLimiterTest(int16_t timeoutSeconds, int16_t maxItems)
        : SpamLimiter(timeoutSeconds, maxItems)
    {
    }

    time_t getTimeout() const
    {
        return _timeoutSeconds;
    }

    size_t getMaxItems() const
    {
        return _maxItems;
    }

    map<uint32_t, time_t>& getCache()
    {
        return _cache;
    }
};

TEST_CASE("Log/Internal/SpamLimiter")
{
    SECTION("SpamLimiter")
    {
        SECTION("ctor")
        {
            struct
            {
                int16_t timeoutSeconds;
                int16_t maxItems;
                struct
                {
                    time_t timeLimit;
                    size_t maxItems;
                } expected;
            }
            data[] =
            {
                { -1, -1, { 0, 0 }},
                { -1,  0, { 0, 0 }},
                {  0, -1, { 0, 0 }},
                {  0,  0, { 0, 0 }},
                {  0,  1, { 0, 1 }},
                {  1,  0, { 1, 0 }},
                {  1,  1, { 1, 1 }}
            };

            for (auto& item : data)
            {
                SpamLimiterTest limiter(item.timeoutSeconds, item.maxItems);

                REQUIRE(limiter.getTimeout() == item.expected.timeLimit); //-V521
                REQUIRE(limiter.getMaxItems() == item.expected.maxItems); //-V521
                REQUIRE(limiter.getCache().size() == 0); //-V521
            }
        }
        SECTION("setLimits")
        {
            struct
            {
                int16_t timeoutSeconds;
                int16_t maxItems;
                struct
                {
                    time_t timeLimit;
                    size_t maxItems;
                    size_t cacheSize;
                } expected;
            }
            data[] =
            {
                { -1,  0, { 8, 0, 0 }},
                {  0, -1, { 0, 3, 0 }},
                {  0,  0, { 0, 0, 0 }},
                {  0,  1, { 0, 1, 0 }},
                {  1,  0, { 1, 0, 0 }},
                {  1,  3, { 1, 3, 3 }},
                {  1,  4, { 1, 4, 3 }},
                {  2,  INT16_MAX, { 2, 3, 3 }}
            };

            for (auto& item : data)
            {
                SpamLimiterTest limiter(8, 3);

                limiter.getCache().emplace(2, 22); //-V807
                limiter.getCache().emplace(4, 44); //-V112
                limiter.getCache().emplace(6, 66);

                limiter.setLimits(item.timeoutSeconds, item.maxItems);

                REQUIRE(limiter.getTimeout() == item.expected.timeLimit); //-V521
                REQUIRE(limiter.getMaxItems() == item.expected.maxItems); //-V521
                REQUIRE(limiter.getCache().size() == item.expected.cacheSize); //-V521
            }
        }
        SECTION("removeExpired")
        {
            struct
            {
                bool    plusOne;
                time_t  curTime;
                struct
                {
                    size_t cacheSize;
                    time_t itemsLeft[3];
                } expected;
            }
            data[] =
            {
                {  true,  3, { 2, { 44, 66     }}},
                {  true,  1, { 2, { 44, 66     }}},
                { false,  1, { 3, { 22, 44, 66 }}},
                { false, 10, { 0                }},
            };

            for (auto& item : data)
            {
                SpamLimiterTest limiter(8, 3);

                limiter.getCache().emplace(22, 2);
                limiter.getCache().emplace(44, 4); //-V112
                limiter.getCache().emplace(66, 6);

                limiter.removeExpired(item.plusOne, item.curTime);
                REQUIRE(limiter.getCache().size() == item.expected.cacheSize); //-V521
                size_t index = 0;
                for (auto& cacheItem : limiter.getCache())
                {
                    REQUIRE(cacheItem.first == item.expected.itemsLeft[index]); //-V521
                    ++index;
                }
            }
        }
        SECTION("check")
        {
            SpamLimiterTest limiter(4, 2); //-V112

            REQUIRE(limiter.check("1", 1)); //-V521
            REQUIRE(limiter.check("2", 2)); //-V521
            REQUIRE_FALSE(limiter.check("1", 3)); //-V521
            REQUIRE(limiter.check("1", 5)); //-V521
            REQUIRE(limiter.check("3", 6)); //-V521
            REQUIRE(limiter.check("2", 7)); //-V521
        }
    }
}

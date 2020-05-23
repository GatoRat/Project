// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <chrono>
#include "Util/Time/TimeStamp.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("TimeStamp")
{
    SECTION("ctor")
    {
        TimeStamp timeStamp;
        REQUIRE(timeStamp.val == 0); //-V521
    }
    SECTION("ctor(Set::ToMin)")
    {
        TimeStamp timeStamp(TimeStamp::Set::ToMin);
        REQUIRE(timeStamp.val == 0); //-V521
    }
    SECTION("ctor(Set::ToMax)")
    {
        TimeStamp timeStamp(TimeStamp::Set::ToMax);
        REQUIRE(timeStamp.val == UINT64_MAX); //-V521
    }
    SECTION("ctor(val)")
    {
        TimeStamp timeStamp(0x1206140d0e0f02a6);
        REQUIRE(timeStamp.dt.year == 18 ); //-V521
        REQUIRE(timeStamp.dt.mon  ==  6 ); //-V521
        REQUIRE(timeStamp.dt.day  == 20 ); //-V521
        REQUIRE(timeStamp.dt.hour == 13 ); //-V521
        REQUIRE(timeStamp.dt.min  == 14 ); //-V521
        REQUIRE(timeStamp.dt.sec  == 15 ); //-V521
        REQUIRE(timeStamp.dt.msec == 678); //-V521
    }
    SECTION("ctor(Set::ToCurTime)")
    {
        TimeStamp timeStamp(TimeStamp::Set::ToCurTime);
        REQUIRE(timeStamp.val != 0); //-V521

        time_t curTime = time(nullptr);
        struct tm tm;
        localtime_s(&tm, &curTime);

        // If this test is run microseconds before midnight, this may fail.
        REQUIRE(timeStamp.dt.year == (tm.tm_year + 1900) - 2000); //-V521

        int diff_seconds = tm.tm_sec - static_cast<int>(timeStamp.dt.sec);
        REQUIRE(diff_seconds >= 0); //-V521
        REQUIRE(diff_seconds <= 2); //-V521
    }
    SECTION("ctor(true)")
    {
        struct
        {
            tm left;
            tm right;
            int relationship;

            //struct tm
            //{
            //    int tm_sec;   // seconds after the minute - [0, 60] including leap second
            //    int tm_min;   // minutes after the hour - [0, 59]
            //    int tm_hour;  // hours since midnight - [0, 23]
            //    int tm_mday;  // day of the month - [1, 31]
            //    int tm_mon;   // months since January - [0, 11]
            //    int tm_year;  // years since 1900
            //    int tm_wday;  // days since Sunday - [0, 6]
            //    int tm_yday;  // days since January 1 - [0, 365]
            //    int tm_isdst; // daylight savings time flag
            //};

            //uint16_t year;
            //uint8_t  mon;
            //uint8_t  day;
            //uint8_t  hour;
            //uint8_t  min;
            //uint8_t  sec;

        }
        data[] =
        {
            { { 45, 30, 15, 11, 3, 118, 0, 0, 0 },{ 45, 30, 15, 11, 3, 118, 0, 0, 0 },  0 },
            { { 45, 30, 15, 11, 3, 118, 0, 0, 0 },{ 46, 30, 15, 11, 3, 118, 0, 0, 0 }, -1 },
            { { 46, 30, 15, 11, 3, 118, 0, 0, 0 },{ 45, 30, 15, 11, 3, 118, 0, 0, 0 },  1 },
            { { 45, 30,  2, 11, 3, 118, 0, 0, 0 },{ 45, 30,  2, 11, 3, 118, 0, 0, 1 },  0 }, // Note that there is no such time
        };

        for (auto& item : data)
        {
            TimeStamp left(item.left);
            TimeStamp right(item.right);

            if (item.relationship < 0)
            {
                REQUIRE(left < right); //-V521
            }
            else if (item.relationship == 0)
            {
                REQUIRE(left <= right); //-V521
            }
            else if (item.relationship > 0)
            {
                REQUIRE(right < left); //-V521
            }
        }
    }
    SECTION("general")
    {
        TimeStamp t1(0, 0, 0, 0, 0, 0);
        REQUIRE(t1.val == 0); //-V521

        TimeStamp t2(0, 0, 0, 0, 0, 0, 2);
        REQUIRE(t2.val == 2); //-V521

        TimeStamp t3(0, 0, 0, 0, 0, 0, 5);
        REQUIRE(t3.val == 5); //-V521
        REQUIRE((t3.val - t2.val) == 3); //-V521
    }
    SECTION("set with string")
    {
        SECTION("nullptr")
        {
            TimeStamp timeStamp;

            const char* pEnd = "nonsense";
            REQUIRE_FALSE(timeStamp.set(nullptr, false, &pEnd)); //-V521
            REQUIRE(pEnd == nullptr); //-V521
        }
        SECTION("valid")
        {
            struct
            {
                const char* pStr;
                bool requireSeconds;
                struct
                {
                    bool        result;
                    uint8_t     year;
                    uint8_t     mon;
                    uint8_t     day;
                    uint8_t     hour;
                    uint8_t     min;
                    uint8_t     sec;
                    uint16_t    msec;
                    const char* pEnd;
                } expected;
            }
            data[] =
            {
                { "2018-06-20 13:14:15.678" , false, { true, 18, 6, 20, 13, 14, 15, 678, "" }},
                { "2019-07-21T14:15:16.789Z", false, { true, 19, 7, 21, 14, 15, 16, 789, "" }}
            };

            int test = 0;
            for (const auto& item : data)
            {
                TimeStamp timeStamp;
                const char* pEnd = "???";
                bool result = timeStamp.set(item.pStr, false, &pEnd);
                REQUIRE(result            == item.expected.result); //-V521
                REQUIRE(timeStamp.dt.year == item.expected.year  ); //-V521
                REQUIRE(timeStamp.dt.mon  == item.expected.mon   ); //-V521
                REQUIRE(timeStamp.dt.day  == item.expected.day   ); //-V521
                REQUIRE(timeStamp.dt.hour == item.expected.hour  ); //-V521
                REQUIRE(timeStamp.dt.min  == item.expected.min   ); //-V521
                REQUIRE(timeStamp.dt.sec == item.expected.sec); //-V521
                REQUIRE(timeStamp.dt.msec == item.expected.msec); //-V521
                REQUIRE(strcmp(pEnd, item.expected.pEnd) == 0); //-V521

                ++test;
            }
        }
    }
    SECTION("toStr")
    {
        //enum StrStyle
        //{
        //    UseStrict8601 = 0b0000001,
        //    UseSeconds = 0b0000010,
        //    UseMilliseconds = 0b0000100 | UseSeconds,
        //    TimeOnly = 0b0001000,
        //    UseTwoDigitYear = 0b0010000,
        //};

        TimeStamp ts(2019, 1, 2, 3, 4, 5, 6); //-V112

        struct
        {
            TimeStamp::StrStyle style;
            const char* pExpected;
        }
        data[] =
        {
            { TimeStamp::UseStrict8601 | TimeStamp::UseMilliseconds, "2019-01-02T03:04:05.006Z" },
            { TimeStamp::UseStrict8601  , "2019-01-02T03:04:05Z"    },
            { TimeStamp::UseTwoDigitYear, "19-01-02 03:04"          },
            { TimeStamp::TimeOnly       , "03:04"                   },
            { TimeStamp::UseSeconds     , "2019-01-02 03:04:05"     },
            { TimeStamp::UseMilliseconds, "2019-01-02 03:04:05.006" },
        };

        for (auto& item : data)
        {
            REQUIRE(ts.toStr(item.style) == item.pExpected); //-V521
        }
    }
}
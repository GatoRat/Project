// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <chrono>
#include <iostream>
#include <sstream>
#include "Util/Time/DateTime.h"
#include "TestHelper/catch.hpp"

using namespace std;

static bool CompareDateTime(time_t expectedTime, time_t actualTime, time_t tolerance = SECONDS_PER_MINUTE * 2)
{
    time_t diff = actualTime <= expectedTime ? expectedTime - actualTime : actualTime - expectedTime;
    return diff <= tolerance;
}

TEST_CASE("DateTime")
{
    tm _tm;

    _tm.tm_sec   = 58;
    _tm.tm_min   = 59;
    _tm.tm_hour  = 23;
    _tm.tm_mday  = 31;
    _tm.tm_mon   = 11;
    _tm.tm_year  = 111;
    _tm.tm_wday  = 6;
    _tm.tm_yday  = 365;
    _tm.tm_isdst = 0;

    SECTION("DateTime()")
    {
        DateTime dateTime;
        REQUIRE(CompareDateTime(dateTime.secondsSince1970(), time(nullptr))); //-V106 //-V521
    }
    SECTION("DateTime(const tm& _tm, uint32_t microseconds)")
    {
        DateTime dateTime(_tm, 163456);

        REQUIRE(dateTime.year()         == 2011); //-V521
        REQUIRE(dateTime.month()        == 11); //-V521
        REQUIRE(dateTime.day()          == 31); //-V521
        REQUIRE(dateTime.hour()         == 23); //-V521
        REQUIRE(dateTime.minute()       == 59); //-V521
        REQUIRE(dateTime.second()       == 58); //-V521
        REQUIRE(dateTime.deciseconds()  == 1); //-V521
        REQUIRE(dateTime.microseconds() == 163456); //-V521
        REQUIRE(dateTime.secondOfDay()  == 86398); //-V521
        REQUIRE(dateTime.dayOfWeek()    == 6); //-V521
        REQUIRE(dateTime.dayOfYear()    == 365); //-V521
    }
    SECTION("operator TimeStamp=")
    {
        DateTime dateTime(_tm, 163456);
        TimeStamp timeStamp = dateTime;

        REQUIRE(timeStamp.dt.year == 11 ); //-V521
        REQUIRE(timeStamp.dt.mon  == 12 ); //-V521
        REQUIRE(timeStamp.dt.day  == 31 ); //-V521
        REQUIRE(timeStamp.dt.hour == 23 ); //-V521
        REQUIRE(timeStamp.dt.min  == 59 ); //-V521
        REQUIRE(timeStamp.dt.sec  == 58 ); //-V521
        REQUIRE(timeStamp.dt.msec == 163); //-V521
    }
    SECTION("reset(const tm& _tm, uint32_t microseconds)")
    {
        DateTime dateTime;
        dateTime.reset(_tm, 163456);

        REQUIRE(dateTime.year() == 2011); //-V521
        REQUIRE(dateTime.month() == 11); //-V521
        REQUIRE(dateTime.day() == 31); //-V521
        REQUIRE(dateTime.hour() == 23); //-V521
        REQUIRE(dateTime.minute() == 59); //-V521
        REQUIRE(dateTime.second() == 58); //-V521
        REQUIRE(dateTime.deciseconds() == 1); //-V521
        REQUIRE(dateTime.microseconds() == 163456); //-V521
        REQUIRE(dateTime.dayOfWeek() == 6); //-V521
        REQUIRE(dateTime.dayOfYear() == 365); //-V521
    }
    SECTION("reset()")
    {
        DateTime dateTime(_tm, 16345678);
        dateTime.reset();
        REQUIRE(CompareDateTime(dateTime.secondsSince1970(), time(nullptr))); //-V106 //-V521
    }
    SECTION("clear()")
    {
        DateTime dateTime(_tm, 16345678);
        dateTime.clear();
        REQUIRE(dateTime.secondsSince1970() < SECONDS_PER_DAY); //-V521
    }
    SECTION("reset(const char* pStr, Precision Precision)")
    {
        static char* pFull = "2017-12-19 12:15:30.123456";
        DateTime::Precision precision = DateTime::Precision::Year;

        DateTime dateTime(_tm, 16345678);

        SECTION("Precision::Microsecond")
        {
            dateTime.reset(pFull, DateTime::Precision::Microsecond);
            precision = DateTime::Precision::Microsecond;
        }
        SECTION("Precision::Second")
        {
            dateTime.reset(pFull, DateTime::Precision::Second);
            precision = DateTime::Precision::Second;
        }
        SECTION("Precision::Minute")
        {
            dateTime.reset(pFull, DateTime::Precision::Minute);
            precision = DateTime::Precision::Minute;
        }
        SECTION("Precision::Hour")
        {
            dateTime.reset(pFull, DateTime::Precision::Minute);
            precision = DateTime::Precision::Minute;
        }
        SECTION("Precision::Day")
        {
            dateTime.reset(pFull, DateTime::Precision::Minute);
            precision = DateTime::Precision::Minute;
        }
        SECTION("Precision::Month")
        {
            dateTime.reset(pFull, DateTime::Precision::Minute);
            precision = DateTime::Precision::Minute;
        }

        REQUIRE(dateTime.year() == 2017); //-V521
        if (precision >= DateTime::Precision::Month)
        {
            REQUIRE(dateTime.month() == 11); //-V521
            if (precision >= DateTime::Precision::Day) //-V547 //-V1016
            {
                REQUIRE(dateTime.day() == 19); //-V521
                if (precision >= DateTime::Precision::Hour) //-V547 //-V1016
                {
                    REQUIRE(dateTime.hour() == 12); //-V521
                    if (precision >= DateTime::Precision::Minute) //-V547 //-V1016
                    {
                        REQUIRE(dateTime.minute() == 15); //-V521
                        if (precision >= DateTime::Precision::Second)
                        {
                            REQUIRE(dateTime.second() == 30); //-V521
                            if (precision >= DateTime::Precision::Microsecond)
                            {
                                REQUIRE(dateTime.microseconds() == 123456); //-V521
                            }
                        }
                    }
                }
            }
        }

        if (precision < DateTime::Precision::Microsecond)
        {
            REQUIRE(dateTime.microseconds() == 0); //-V521
            if (precision < DateTime::Precision::Second)
            {
                REQUIRE(dateTime.second() == 0); //-V521
                if (precision < DateTime::Precision::Minute)
                {
                    REQUIRE(dateTime.minute() == 0); //-V521
                    if (precision < DateTime::Precision::Hour) //-V547
                    {
                        REQUIRE(dateTime.hour() == 0); //-V521
                        if (precision < DateTime::Precision::Day) //-V547
                        {
                            REQUIRE(dateTime.day() == 1); //-V521
                            if (precision < DateTime::Precision::Month) //-V547
                            {
                                REQUIRE(dateTime.month() == 0); //-V521
                            }
                        }
                    }
                }
            }
        }
    }
    SECTION("copy")
    {
        DateTime orgDateTime(_tm, 163456);
        DateTime dateTime(orgDateTime);

        REQUIRE(dateTime.year() == 2011); //-V521
        REQUIRE(dateTime.month() == 11); //-V521
        REQUIRE(dateTime.day() == 31); //-V521
        REQUIRE(dateTime.hour() == 23); //-V521
        REQUIRE(dateTime.minute() == 59); //-V521
        REQUIRE(dateTime.second() == 58); //-V521
        REQUIRE(dateTime.deciseconds() == 1); //-V521
        REQUIRE(dateTime.microseconds() == 163456); //-V521
        REQUIRE(dateTime.dayOfWeek() == 6); //-V521
        REQUIRE(dateTime.dayOfYear() == 365); //-V521
    }
    SECTION("assignment")
    {
        DateTime orgDateTime(_tm, 163456);
        DateTime dateTime;
        dateTime = orgDateTime;

        REQUIRE(dateTime.year() == 2011); //-V521
        REQUIRE(dateTime.month() == 11); //-V521
        REQUIRE(dateTime.day() == 31); //-V521
        REQUIRE(dateTime.hour() == 23); //-V521
        REQUIRE(dateTime.minute() == 59); //-V521
        REQUIRE(dateTime.second() == 58); //-V521
        REQUIRE(dateTime.deciseconds() == 1); //-V521
        REQUIRE(dateTime.microseconds() == 163456); //-V521
        REQUIRE(dateTime.dayOfWeek() == 6); //-V521
        REQUIRE(dateTime.dayOfYear() == 365); //-V521
    }
    SECTION("equality")
    {
        DateTime dateTime1(_tm, 163456);

        tm _tm2;
        memcpy(&_tm2, &_tm, sizeof(_tm));
        _tm2.tm_isdst = 1;

        DateTime dateTime2(_tm2, 163456);

        REQUIRE(dateTime1 == dateTime2); //-V521
    }
    SECTION("!equality")
    {
        DateTime dateTime1(_tm, 163456);

        tm _tm2;
        memcpy(&_tm2, &_tm, sizeof(_tm));
        _tm2.tm_isdst = 1;

        DateTime dateTime2(_tm2, 163457);

        REQUIRE(!(dateTime1 == dateTime2)); //-V521
    }
    SECTION("!isSame")
    {
        DateTime dateTime1(_tm, 163456);

        tm _tm2;
        memcpy(&_tm2, &_tm, sizeof(_tm));
        _tm2.tm_isdst = 1;

        DateTime dateTime2(_tm2, 163456);

        REQUIRE_FALSE(dateTime1.isSame(dateTime2)); //-V521
    }
    SECTION("compare")
    {
        struct
        {
            DateTime dt1;
            DateTime dt2;
            int expected[7];
        }
        static data[] =
        {
            {{{ 30, 30, 12, 15, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 5, 128, false }, 12345 }, { 0, 0, 0, 0, 0, 0, 0 }},
            {{{ 30, 30, 12, 15, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0, 0, 0, 0, 0, 0 }},
            {{{ 30, 30, 12, 15, 6, 100, 5, 128, false }, 12346 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0, 0, 0, 0, 0,  1 }},
            {{{ 30, 30, 12, 15, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12346 }, { 0, 0, 0, 0, 0, 0, -1 }},
            {{{ 31, 30, 12, 15, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0, 0, 0, 0,  1,  1 }},
            {{{ 29, 30, 12, 15, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0, 0, 0, 0, -1, -1 }},
            {{{ 30, 31, 12, 15, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0, 0, 0,  1,  1,  1 }},
            {{{ 30, 29, 12, 15, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0, 0, 0, -1, -1, -1 }},
            {{{ 30, 30, 13, 15, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0, 0,  1,  1,  1,  1 }},
            {{{ 30, 30, 11, 15, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0, 0, -1, -1, -1, -1 }},
            {{{ 30, 30, 12, 16, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0,  1,  1,  1,  1,  1 }},
            {{{ 30, 30, 12, 14, 6, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, 0, -1, -1, -1, -1, -1 }},
            {{{ 30, 30, 12, 15, 7, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0,  1,  1,  1,  1,  1,  1 }},
            {{{ 30, 30, 12, 15, 5, 100, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { 0, -1, -1, -1, -1, -1, -1 }},
            {{{ 30, 30, 12, 15, 6, 101, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, {  1,  1,  1,  1,  1,  1,  1 }},
            {{{ 30, 30, 12, 15, 6,  99, 5, 128, false }, 12345 }, {{ 30, 30, 12, 15, 6, 100, 3, 300, true  }, 12345 }, { -1, -1, -1, -1, -1, -1, -1 }},
        };

        int testNum = 0;
        for (const auto& item : data)
        {
            for (int p = static_cast<int>(DateTime::Precision::Year); p <= static_cast<int>(DateTime::Precision::Microsecond); ++p)
            {
                int result = item.dt1.compare(item.dt2, static_cast<DateTime::Precision>(p));
                if (result < 0)
                {
                    result = -1;
                }
                else if (result > 0)
                {
                    result = 1;
                }

                if (result != item.expected[p])
                {
                    stringstream ss;
                    ss << "test: " << testNum << " precision: " << p << " expected: " << item.expected[p] << " returned: " << result;
                    FAIL(ss.str().c_str());
                }
            }

            ++testNum;
        }
    }
    SECTION("operator<>")
    {
                     // s   m   h   d  m  y    xxxxxx  isDst    usec
        DateTime dt1{ {30, 30, 12, 15, 6, 100, 5, 128, false }, 12345 };

        struct
        {
            DateTime dt2;
            struct
            {
                bool lessThan;
                bool greaterThan;
            } expected;
        }
        static data[] =
        {
            {{{ 30, 30, 12, 15, 6, 100, 0, 0, false }, 12345 }, false, false },
            {{{ 30, 30, 12, 15, 6, 100, 0, 0, false }, 12344 }, false, true  },
            {{{ 30, 30, 12, 15, 6, 100, 0, 0, false }, 12346 }, true , false },
            {{{ 29, 30, 12, 15, 6, 100, 0, 0, false }, 12345 }, false, true  },
            {{{ 31, 30, 12, 15, 6, 100, 0, 0, false }, 12345 }, true , false },
            {{{ 30, 29, 12, 15, 6, 100, 0, 0, false }, 12345 }, false, true  },
            {{{ 30, 31, 12, 15, 6, 100, 0, 0, false }, 12345 }, true , false },
            {{{ 30, 30, 11, 15, 6, 100, 0, 0, false }, 12345 }, false, true  },
            {{{ 30, 30, 13, 15, 6, 100, 0, 0, false }, 12345 }, true , false },
            {{{ 30, 30, 12, 14, 6, 100, 0, 0, false }, 12345 }, false, true  },
            {{{ 30, 30, 12, 16, 6, 100, 0, 0, false }, 12345 }, true , false },
            {{{ 30, 30, 12, 15, 5, 100, 0, 0, false }, 12345 }, false, true  },
            {{{ 30, 30, 12, 15, 7, 100, 0, 0, false }, 12345 }, true , false },
            {{{ 30, 30, 12, 15, 6,  99, 0, 0, false }, 12345 }, false, true  },
            {{{ 30, 30, 12, 15, 6, 101, 0, 0, false }, 12345 }, true , false }
        };

        int testNum = 0;
        for (const auto& item : data)
        {
            bool lessThanResult = dt1 < item.dt2;
            if (lessThanResult != item.expected.lessThan)
            {
                stringstream ss;
                ss << "test: " << testNum << ": lessThanResult != item.expected.lessThan";
                FAIL(ss.str().c_str());
            }

            bool greaterThanResult = dt1 > item.dt2;
            if (greaterThanResult != item.expected.greaterThan)
            {
                stringstream ss;
                ss << "test: " << testNum << ": greaterThanResult != item.expected.greaterThan";
                FAIL(ss.str().c_str());
            }

            ++testNum;
        }
    }
    SECTION("operator<<")
    {
        DateTime dateTime(_tm, 163456);

        stringstream ss;
        ss << dateTime;
        REQUIRE(ss.str() == "2011-12-31 23:59:58"); //-V521
    }
    SECTION("reset")
    {
        DateTime dateTime(_tm, 163456);

        SECTION("full success")
        {
            const char* pEnd;
            REQUIRE(dateTime.reset("2017-08-15 12:34:56", true, &pEnd)); //-V521
            REQUIRE(!*pEnd); //-V521

            REQUIRE(dateTime.year() == 2017); //-V521
            REQUIRE(dateTime.month() == 7); //-V521
            REQUIRE(dateTime.day() == 15); //-V521
            REQUIRE(dateTime.hour() == 12); //-V521
            REQUIRE(dateTime.minute() == 34); //-V521
            REQUIRE(dateTime.second() == 56); //-V521
        }
        SECTION("noseconds success")
        {
            const char* pEnd;
            REQUIRE(dateTime.reset("2017-08-15 12:34", false, &pEnd)); //-V521
            REQUIRE(!*pEnd); //-V521

            REQUIRE(dateTime.year() == 2017); //-V521
            REQUIRE(dateTime.month() == 7); //-V521
            REQUIRE(dateTime.day() == 15); //-V521
            REQUIRE(dateTime.hour() == 12); //-V521
            REQUIRE(dateTime.minute() == 34); //-V521
            REQUIRE(dateTime.second() == 0); //-V521
        }
        SECTION("noseconds fail")
        {
            const char* pEnd;
            REQUIRE_FALSE(dateTime.reset("2017-08-15 12:34", true, &pEnd)); //-V521
            REQUIRE(!*pEnd); //-V521

            REQUIRE(dateTime.year() == 2017); //-V521
            REQUIRE(dateTime.month() == 7); //-V521
            REQUIRE(dateTime.day() == 15); //-V521
            REQUIRE(dateTime.hour() == 12); //-V521
            REQUIRE(dateTime.minute() == 34); //-V521
            REQUIRE(dateTime.second() == 0); //-V521
        }
        SECTION("extra string")
        {
            const char* pEnd;
            REQUIRE(dateTime.reset("2017-08-15 12:34:567", true, &pEnd)); //-V521
            REQUIRE(*pEnd == '7'); //-V521

            REQUIRE(dateTime.year() == 2017); //-V521
            REQUIRE(dateTime.month() == 7); //-V521
            REQUIRE(dateTime.day() == 15); //-V521
            REQUIRE(dateTime.hour() == 12); //-V521
            REQUIRE(dateTime.minute() == 34); //-V521
            REQUIRE(dateTime.second() == 56); //-V521
        }
        SECTION("microseconds")
        {
            int64_t usec = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count();
            DateTime dt(usec);
            DateTime now;

            int64_t diffInSeconds = now.secondsSince1970() - dt.secondsSince1970();
            REQUIRE(abs(diffInSeconds) < 10); //-V521
        }
    }
    SECTION("toString")
    {
        DateTime dateTime(_tm, 163456);
        string s = dateTime.toString(DateTime::MaxPlaces);
        REQUIRE(s == "2011-12-31 23:59:58.163456"); //-V521
    }
    SECTION("FastWrite")
    {
        DateTime dateTime(_tm, 163456);
        string s("z");
        SECTION("true, true")
        {
            REQUIRE(dateTime.fastAppend(s, true, true) == "z2011-12-31 23:59:58.163"); //-V521
        }
        SECTION("true, false")
        {
            REQUIRE(dateTime.fastAppend(s, true, false) == "z2011-12-31 23:59:58"); //-V521
        }
        SECTION("false, true")
        {
            REQUIRE(dateTime.fastAppend(s, false, true) == "z11-12-31 23:59:58.163"); //-V521
        }
        SECTION("false, false")
        {
            REQUIRE(dateTime.fastAppend(s, false, false) == "z11-12-31 23:59:58"); //-V521
        }
        SECTION("Precision::Year, false")
        {
            REQUIRE(dateTime.fastAppend(s, DateTime::Precision::Year, false) == "z11"); //-V521
        }
        SECTION("Precision::Year, true")
        {
            REQUIRE(dateTime.fastAppend(s, DateTime::Precision::Year, true) == "z2011"); //-V521
        }
        SECTION("Precision::Month, true")
        {
            REQUIRE(dateTime.fastAppend(s, DateTime::Precision::Month) == "z2011-12"); //-V521
        }
        SECTION("Precision::Day, true")
        {
            REQUIRE(dateTime.fastAppend(s, DateTime::Precision::Day) == "z2011-12-31"); //-V521
        }
        SECTION("Precision::Hour, true")
        {
            REQUIRE(dateTime.fastAppend(s, DateTime::Precision::Hour) == "z2011-12-31 23"); //-V521
        }
        SECTION("Precision::Minute, true")
        {
            REQUIRE(dateTime.fastAppend(s, DateTime::Precision::Minute) == "z2011-12-31 23:59"); //-V521
        }
        SECTION("Precision::Second, true")
        {
            REQUIRE(dateTime.fastAppend(s, DateTime::Precision::Second) == "z2011-12-31 23:59:58"); //-V521
        }
        SECTION("Precision::Microsecond, true")
        {
            REQUIRE(dateTime.fastAppend(s, DateTime::Precision::Microsecond) == "z2011-12-31 23:59:58.163"); //-V521
        }
    }
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <sstream>
#include "Util/Time/ParseDate.h"
#include "Util/Time/DateTime.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("ParseDate")
{
    SECTION("validateBasic")
    {
        struct
        {
            tm time;
            bool expected;
        }
        static data[] =
        {
            { { 59, 59, 23, 32, DateTime::Jan, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Jan, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 30, DateTime::Jan, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 29, DateTime::Feb, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 29, DateTime::Feb, 104, 0, 0, 0 }, true  },
            { { 59, 59, 23, 29, DateTime::Feb, 200, 0, 0, 0 }, false },
            { { 59, 59, 23, 32, DateTime::Mar, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Mar, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 31, DateTime::Apr, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 30, DateTime::Apr, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 32, DateTime::May, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::May, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 31, DateTime::Jun, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 30, DateTime::Jun, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 32, DateTime::Jul, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Jul, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 32, DateTime::Aug, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Aug, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 31, DateTime::Sep, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 30, DateTime::Sep, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 32, DateTime::Oct, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Oct, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 31, DateTime::Nov, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 30, DateTime::Nov, 100, 0, 0, 0 }, true  },
            { { 59, 59, 23, 32, DateTime::Dec, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Dec, 100, 0, 0, 0 }, true  },

            { { 60, 59, 23,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { { 59, 59, 23,  0, DateTime::Jan, 100, 0, 0, 0 }, false },
            { { 59, 60, 23,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { { 59, 59, 24,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { { -1, 59, 23,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { {  0, -1, 23,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { {  0,  0, -1,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { {  0,  0,  0, -1, DateTime::Jan, 100, 0, 0, 0 }, false },
        };

        for (const auto& item : data)
        {
            REQUIRE(ParseDate::ValidateBasic(item.time) == item.expected); //-V521
        }
    }
    SECTION("validateBasic (with leap second)")
    {
        struct
        {
            tm time;
            bool expected;
        }
        static data[] =
        {
            { { 59, 59, 23, 32, DateTime::Jan, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Jan, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 30, DateTime::Jan, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 29, DateTime::Feb, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 29, DateTime::Feb, 104, 0, 0, 0 }, true },
            { { 59, 59, 23, 29, DateTime::Feb, 200, 0, 0, 0 }, false },
            { { 59, 59, 23, 32, DateTime::Mar, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Mar, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 31, DateTime::Apr, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 30, DateTime::Apr, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 32, DateTime::May, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::May, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 31, DateTime::Jun, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 30, DateTime::Jun, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 32, DateTime::Jul, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Jul, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 32, DateTime::Aug, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Aug, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 31, DateTime::Sep, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 30, DateTime::Sep, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 32, DateTime::Oct, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Oct, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 31, DateTime::Nov, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 30, DateTime::Nov, 100, 0, 0, 0 }, true },
            { { 59, 59, 23, 32, DateTime::Dec, 100, 0, 0, 0 }, false },
            { { 59, 59, 23, 31, DateTime::Dec, 100, 0, 0, 0 }, true },

            { { 60, 59, 23,  1, DateTime::Jan, 100, 0, 0, 0 }, true },
            { { 59, 59, 23,  0, DateTime::Jan, 100, 0, 0, 0 }, false },
            { { 59, 60, 23,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { { 59, 59, 24,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { { -1, 59, 23,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { {  0, -1, 23,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { {  0,  0, -1,  1, DateTime::Jan, 100, 0, 0, 0 }, false },
            { {  0,  0,  0, -1, DateTime::Jan, 100, 0, 0, 0 }, false },
        };

        for (const auto& item : data)
        {
            REQUIRE(ParseDate::ValidateBasic(item.time, true) == item.expected); //-V521
        }
    }
    SECTION("Basic (nullptr)")
    {
        SECTION("bad format")
        {
            struct
            {
                const char* pStr;
                int failureIndex;
            }
            static data[] =
            {   // 012345678901234567890
                { "2017-02-28 16_30" , 13 },
                { "200-02-29 16:30"  ,  0 },
                { "2017-02-15  16:30", 11 }
            };

            for (const auto& item : data)
            {
                const char* pEnd;
                if (ParseDate::Basic(item.pStr, nullptr, false, &pEnd))
                {
                    stringstream ss;
                    ss << "ParseDate::basic(" << item.pStr << ") != false";
                    FAIL(ss.str().c_str());
                }
                REQUIRE(pEnd - item.pStr == item.failureIndex); //-V521
            }
        }
        SECTION("good format")
        {
            struct
            {
                const char* pStr;
                bool expected;
            }
            static data[] =
            {
                { "2017-02-28 16:30", true  },
                { "2017-02-29 16:30", false },
                { "2000-02-29 16:30", true  },
                { "2100-02-29 16:30", false },
                { "2100-02-28 16:30", true  },

                { "2100-02-28 24:30", false },
                { "2100-02-28 16:60", false },

                { "2017-02-28 16:30:10", true  },
                { "2017-02-29 16:30:10", false },
                { "2000-02-29 16:30:10", true  },
                { "2100-02-29 16:30:10", false },
                { "2100-02-28 16:30:10", true  },

                { "2100-02-28 24:30:10", false },
                { "2100-02-28 16:60:10", false },
                { "2017-02-28 16:30:60", false },
            };

            for (const auto& item : data)
            {
                const char* pEnd;
                if (ParseDate::Basic(item.pStr, nullptr, false, &pEnd) != item.expected)
                {
                    stringstream ss;
                    ss << "ParseDate::basic(" << item.pStr << ") != " << (item.expected ? "true" : "false");
                    FAIL(ss.str().c_str());
                }
                REQUIRE(*pEnd == 0); //-V521
            }
        }
    }
    SECTION("Basic")
    {
        struct
        {
            const char* pStr;
            tm time;
            bool requireSeconds;
            bool expected;
        }
        static data[] =
        {
            { "2017-02-28 12:30", { 0, 30, 12, 28, DateTime::Feb, 117, 0, 0, 0 }, false, true  },
            { "2017-02-29 13:31", { 0, 31, 13, 29, DateTime::Feb, 117, 0, 0, 0 }, false, false },
            { "2000-02-29 14:32", { 0, 32, 14, 29, DateTime::Feb, 100, 0, 0, 0 }, false, true  },
            { "2100-02-29 15:33", { 0, 33, 15, 29, DateTime::Feb, 200, 0, 0, 0 }, false, false },
            { "2100-02-28 16:34", { 0, 34, 16, 28, DateTime::Feb, 200, 0, 0, 0 }, false, true  },

            { "2100-02-28 16:34"   , { 00, 34, 16, 28, DateTime::Feb, 200, 0, 0, 0 }, false, true  },
            { "2100-02-28 16:34:10", { 10, 34, 16, 28, DateTime::Feb, 200, 0, 0, 0 }, false, true  },
            { "2100-02-28 16:34"   , { 00, 34, 16, 28, DateTime::Feb, 200, 0, 0, 0 }, true,  false },
            { "2100-02-28 16:34:10", { 10, 34, 16, 28, DateTime::Feb, 200, 0, 0, 0 }, true,  true  },
        };

        for (const auto& item : data)
        {
            tm time;
            REQUIRE(ParseDate::Basic(item.pStr, &time, item.requireSeconds) == item.expected); //-V521
            if (memcmp(&time, &item.time, sizeof(tm)) != 0)
            {
                stringstream ss;
                ss << "ParseDate::basic(" << item.pStr << ", time, " << (item.requireSeconds ? "true" : "false") << "): memcmp failed";
                FAIL(ss.str().c_str());
            }
        }
    }
}

//    bool parse(const char* pStr, Format format, std::tm* pTime = nullptr);
//    bool parseBasic(const char* pStr, std::tm* pTime = nullptr);

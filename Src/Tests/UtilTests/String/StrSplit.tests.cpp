// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <sstream>
#include "Util/String/StrSplit.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Util/StrSplit")
{
    SECTION("returns vector")
    {
        struct
        {
            const char* pStr;
            char delimiter;
            const char* expected[3];
        }
        static data[] =
        {
            { "1:2:3",  ':', { "1", "2", "3" } },
            { "1::3" ,  ':', { "1", "" , "3" } },
            { "1::"  ,  ':', { "1", "" , ""  } },
            { "::"   ,  ':', { "" , "" , ""  } },
            { ":2:"  ,  ':', { "" , "2", ""  } },
            { ":2:3" ,  ':', { "" , "2", "3" } },
            { "::3"  ,  ':', { "" , "" , "3" } },
        };

        for (const auto& item : data)
        {
            auto result = Str::Split(item.pStr, item.delimiter);
            REQUIRE(result.size() == 3); //-V521
            REQUIRE(result[0] == item.expected[0]); //-V521
            REQUIRE(result[1] == item.expected[1]); //-V521
            REQUIRE(result[2] == item.expected[2]); //-V521
        }
    }
    SECTION("callback")
    {
        struct
        {
            const char* pStr;
            char delimiter;
            const char* expected[3];
        }
        static data[] =
        {
            { "1:2:3",  ':', { "1", "2", "3" } },
            { "1::3" ,  ':', { "1", "" , "3" } },
            { "1::"  ,  ':', { "1", "" , ""  } },
            { "::"   ,  ':', { "" , "" , ""  } },
            { ":2:"  ,  ':', { "" , "2", ""  } },
            { ":2:3" ,  ':', { "" , "2", "3" } },
            { "::3"  ,  ':', { "" , "" , "3" } },
        };

        for (const auto& item : data)
        {
            auto count = Str::Split(item.pStr, item.delimiter, [&](size_t num, string_view str)->bool
            {
                REQUIRE(num < 3); //-V521
                REQUIRE(str == item.expected[num]); //-V521

                return true;
            });

            REQUIRE(count == 3); //-V521
        }
    }
    SECTION("callback2")
    {
        struct
        {
            const char* pStr;
            char delimiter;
            string_view expected[3];
        }
        static data[] =
        {
            { "1:2:3",  ':', { "1", "2", "3" } },
            { "1::3" ,  ':', { "1", "" , "3" } },
            { "1::"  ,  ':', { "1", "" , ""  } },
            { "::"   ,  ':', { "" , "" , ""  } },
            { ":2:"  ,  ':', { "" , "2", ""  } },
            { ":2:3" ,  ':', { "" , "2", "3" } },
            { "::3"  ,  ':', { "" , "" , "3" } },
        };

        for (const auto& item : data)
        {
            size_t curNum = 0;
            char buffer[6];
            strcpy_s(buffer, item.pStr);
            auto count = Str::Split2(buffer, item.delimiter, [&](size_t num, char* pStr)->bool
            {
                REQUIRE(curNum++ == num); //-V521
                REQUIRE(pStr == item.expected[num]); //-V521

                return true;
            });

            REQUIRE(count == 3); //-V521
        }
    }
}

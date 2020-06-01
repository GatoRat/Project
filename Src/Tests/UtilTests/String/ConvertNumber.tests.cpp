// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/Convert.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Util/Convert[Number]::ToStr")
{
    SECTION("ToStr64(uint64_t, char*, char, bool, char**)")
    {
        struct
        {
            uint64_t val;
            char groupSeparator;
            bool isNegative;
            struct
            {
                char* pStr;
            } expected;
        }
        data[] =
        {
            { 18446744073709551615LLU,   0, false, { "18446744073709551615"       }},
            { 18446744073709551615LLU, ',', true , { "-9,223,372,036,854,775,807" }},
            { 184                    ,   0, false, { "184"                        }},
            { 1844                   , '.', false, { "1.844"                      }},
        };

        for (const auto item : data)
        {
            char buffer[Convert::MaxStrLen64_Commas_Signed + 2];
            memset(buffer, 'z', sizeof(buffer));

            char* pEnd = nullptr;
            const char* pStr = Convert::ToStr64(item.val, buffer, item.groupSeparator, item.isNegative, &pEnd);

            REQUIRE(string_view(pStr) == item.expected.pStr); //-V521
            REQUIRE(static_cast<size_t>(pEnd - pStr) == strlen(pStr)); //-V814 //-V521
        }
    }
    SECTION("ToStrAligned64(uint64_t, char*, char, bool)")
    {
        struct
        {
            uint64_t val;
            char groupSeparator;
            bool isNegative;
            struct
            {
                char* pStr;
            } expected;
        }
        data[] =
        {
            { 18446744073709551615LLU,   0, false, { "18446744073709551615" } },
            { 18446744073709551615LLU, ',', true , { "-9,223,372,036,854,775,807" } },
            { 184                    ,   0, false, { "184" } },
            { 1844                   , '.', false, { "1.844" } },
        };

        for (const auto item : data)
        {
            char buffer[Convert::MaxStrLen64_Commas_Signed + 2];
            memset(buffer, 'z', sizeof(buffer));

            const char* pStr = Convert::ToStrAligned64(item.val, buffer, item.groupSeparator, item.isNegative, nullptr);

            REQUIRE(string_view(pStr) == item.expected.pStr); //-V521
            REQUIRE(string_view(buffer) == item.expected.pStr); //-V521
        }
    }
    SECTION("ToStr(string&, uint64_t, size_t, char)")
    {
        struct
        {
            uint64_t val;
            size_t minWidth;
            char fillChar;
            char groupSeparator;
            struct
            {
                char* pStr;
            } expected;
        }
        data[] =
        {
            { 18446744073709551615LLU,   0, '#',   0, { "z18446744073709551615"  } },
            { 18446744073709551615LLU,  21, '#',   0, { "z#18446744073709551615" } },
            { 123                    ,   3, '#', ',', { "z123"     } },
            { 1234                   ,   4, '#', ',', { "z1,234"   } },
            { 12345                  ,   5, '#', ',', { "z12,345"  } },
            { 123456                 ,   6, '#', ',', { "z123,456" } },
            { 123                    ,   8, '#', ',', { "z#####123"} },
            { 1234                   ,   8, '#', ',', { "z###1,234"} },
            { 12345                  ,   8, '#', ',', { "z##12,345"} },
            { 123456                 ,   8, '#', ',', { "z#123,456"} },
        };

        for (const auto item : data)
        {
            string str = "z";

            Convert::ToStr64(str, item.val, item.minWidth, item.fillChar, item.groupSeparator);

            REQUIRE(str == item.expected.pStr); //-V521
        }
    }
    SECTION("ToStr64(uint64_t, char*, char, bool, char**)")
    {
        struct
        { //-V802
            uint64_t val;
            char groupSeparator;
            bool isNegative;
            int  width;
            char padChar;
            struct
            {
                char* pStr;
            } expected;
        }
        data[] =
        {
            { 18446744073709551615LLU,   0, false, 1, '#', { "18446744073709551615"       }},
            { 18446744073709551615LLU, ',', true , 1, '#', { "-9,223,372,036,854,775,807" }},
            { 184                    ,   0, false, 1, '#', { "184"                        }},
            { 1844                   , '.', false, 1, '#', { "1.844"                      }},

            { 184                    ,   0, false, 5, '#', { "##184"                      }},
            { 184                    ,   0, true , 5, '#', { "-#184"                      }},
            { 18446744073709551615LLU, ',', true ,99, '#', { "-9,223,372,036,854,775,807" }},
            { 18446744073709551615LLU,   0, false,99, '#', { "18446744073709551615"       }},
            {  8446744073709551615LLU,   0, false,99, '#', { "#8446744073709551615"       }},
        };

        for (const auto item : data)
        {
            char buffer[Convert::MaxStrLen64_Commas_Signed + 2];
            memset(buffer, 'z', sizeof(buffer));

            char* pEnd = nullptr;
            const char* pStr = Convert::ToStr64Padded(item.val, buffer, item.width, item.padChar, item.groupSeparator, item.isNegative, &pEnd);

            REQUIRE(string_view(pStr) == item.expected.pStr); //-V521
            REQUIRE(static_cast<size_t>(pEnd - pStr) == strlen(pStr)); //-V814 //-V521
        }
    }
}

TEST_CASE("Util/Convert[Number]::AppendFixedFast")
{
    SECTION("char")
    {
        struct
        {
            uint64_t val;
            size_t width;
            struct
            {
                char* pStr;
            } expected;
        }
        data[]
        {
            {     0, 0, { "z"     } },
            {     9, 0, { "z"     } },
            {    99, 0, { "z"     } },
            {   999, 0, { "z"     } },
            {     0, 1, { "z0"    } },
            {     9, 1, { "z9"    } },
            {    19, 1, { "z9"    } },
            {   195, 1, { "z5"    } },
            {     0, 2, { "z00"   } },
            {     9, 2, { "z09"   } },
            {    19, 2, { "z19"   } },
            {   195, 2, { "z95"   } },
            {     0, 3, { "z000"  } },
            {     9, 3, { "z009"  } },
            {    19, 3, { "z019"  } },
            {   195, 3, { "z195"  } },
            {  4195, 3, { "z195"  } },
            { 74195, 3, { "z195"  } },

            {                    0, 20, { "z00000000000000000000" } },
            {                   90, 20, { "z00000000000000000090" } },
            {             74195000, 20, { "z00000000000074195000" } },
            { 18446744073709551615, 20, { "z18446744073709551615" } },

            {                    0, 21, { "z000000000000000000000" } },
            {                   90, 21, { "z000000000000000000090" } },
            {             74195000, 21, { "z000000000000074195000" } },
            { 18446744073709551615, 21, { "z018446744073709551615" } },
        };

        for (const auto& item : data)
        {
            string str("z");
            REQUIRE(Convert::AppendFixedFast(str, item.val, item.width) == item.expected.pStr); //-V521
        }
    }
}

TEST_CASE("Util/Convert[Number]::ToStrFixedFast")
{
    SECTION("char")
    {
        struct
        {
            uint64_t val;
            size_t width;
            struct
            {
                char* pStr;
            } expected;
        }
        data[]
        {
            {     0, 0, { ""     } },
            {     9, 0, { ""     } },
            {    99, 0, { ""     } },
            {   999, 0, { ""     } },
            {     0, 1, { "0"    } },
            {     9, 1, { "9"    } },
            {    19, 1, { "9"    } },
            {   195, 1, { "5"    } },
            {     0, 2, { "00"   } },
            {     9, 2, { "09"   } },
            {    19, 2, { "19"   } },
            {   195, 2, { "95"   } },
            {     0, 3, { "000"  } },
            {     9, 3, { "009"  } },
            {    19, 3, { "019"  } },
            {   195, 3, { "195"  } },
            {  4195, 3, { "195"  } },
            { 74195, 3, { "195"  } },

            {                    0, 20, { "00000000000000000000" } },
            {                   90, 20, { "00000000000000000090" } },
            {             74195000, 20, { "00000000000074195000" } },
            { 18446744073709551615, 20, { "18446744073709551615" } },

            {                    0, 21, { "000000000000000000000" } },
            {                   90, 21, { "000000000000000000090" } },
            {             74195000, 21, { "000000000000074195000" } },
            { 18446744073709551615, 21, { "018446744073709551615" } },
        };

        for (const auto& item : data)
        {
            char buffer[Convert::MinBufferLen];
            memset(buffer, 'z', sizeof(buffer));

            char* pStr = Convert::ToStrFixedFast(buffer, item.val, item.width);
            REQUIRE(*pStr == 'z'); //-V521
            *pStr = 0;
            REQUIRE(strcmp(buffer, item.expected.pStr) == 0); //-V521
        }
    }
}
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/ConvertW.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Util/ConvertW[Number]::ToStr")
{
    SECTION("ToStr64(uint64_t, wchar_t*, wchar_t, bool, wchar_t**)")
    {
        struct
        {
            uint64_t val;
            wchar_t groupSeparator;
            bool isNegative;
            struct
            {
                wchar_t* pStr;
            } expected;
        }
        data[] =
        {
            { 18446744073709551615LLU,   0, false, { L"18446744073709551615"       }},
            { 18446744073709551615LLU, ',', true , { L"-9,223,372,036,854,775,807" }},
            { 184                    ,   0, false, { L"184"                        }},
            { 1844                   , '.', false, { L"1.844"                      }},
        };

        for (const auto item : data)
        {
            wchar_t buffer[ConvertW::MaxStrLen64_Commas_Signed + 2];
            memset(buffer, 'z', sizeof(buffer));

            wchar_t* pEnd = nullptr;
            wchar_t* pStr = ConvertW::ToStr64U(item.val, buffer, item.groupSeparator, item.isNegative, &pEnd);

            REQUIRE(wstring_view(pStr) == item.expected.pStr); //-V521
            REQUIRE(static_cast<size_t>(pEnd - pStr) == wcslen(pStr)); //-V814 //-V521
        }
    }
    SECTION("ToStrAligned64(uint64_t, wchar_t*, wchar_t, bool)")
    {
        struct
        {
            uint64_t val;
            wchar_t groupSeparator;
            bool isNegative;
            struct
            {
                wchar_t* pStr;
            } expected;
        }
        data[] =
        {
            { 18446744073709551615LLU,   0, false, { L"18446744073709551615" } },
            { 18446744073709551615LLU, ',', true , { L"-9,223,372,036,854,775,807" } },
            { 184                    ,   0, false, { L"184" } },
            { 1844                   , '.', false, { L"1.844" } },
        };

        for (const auto item : data)
        {
            wchar_t buffer[ConvertW::MaxStrLen64_Commas_Signed + 2];
            memset(buffer, 'z', sizeof(buffer));

            wchar_t* pStr = ConvertW::ToStrAligned64U(item.val, buffer, item.groupSeparator, item.isNegative, nullptr);

            REQUIRE(wstring_view(pStr) == item.expected.pStr); //-V521
            REQUIRE(wstring_view(buffer) == item.expected.pStr); //-V521
        }
    }
    SECTION("ToStr(wstring&, uint64_t, size_t, wchar_t)")
    {
        struct
        {
            uint64_t val;
            size_t minWidth;
            wchar_t fillChar;
            wchar_t groupSeparator;
            struct
            {
                wchar_t* pStr;
            } expected;
        }
        data[] =
        {
            { 18446744073709551615LLU,   0, L'#',    0, { L"z18446744073709551615"  } },
            { 18446744073709551615LLU,  21, L'#',    0, { L"z#18446744073709551615" } },
            { 123                    ,   3, L'#', L',', { L"z123"     } },
            { 1234                   ,   4, L'#', L',', { L"z1,234"   } },
            { 12345                  ,   5, L'#', L',', { L"z12,345"  } },
            { 123456                 ,   6, L'#', L',', { L"z123,456" } },
            { 123                    ,   8, L'#', L',', { L"z#####123"} },
            { 1234                   ,   8, L'#', L',', { L"z###1,234"} },
            { 12345                  ,   8, L'#', L',', { L"z##12,345"} },
            { 123456                 ,   8, L'#', L',', { L"z#123,456"} },
        };

        for (const auto item : data)
        {
            wstring str = L"z";

            ConvertW::ToStr64U(str, item.val, item.minWidth, item.fillChar, item.groupSeparator);

            REQUIRE(str == item.expected.pStr); //-V521
        }
    }
    SECTION("ToStr64(uint64_t, wchar_t*, wchar_t, bool, wchar_t**)")
    {
        struct
        {
            uint64_t val;
            wchar_t groupSeparator;
            bool isNegative;
            int  width;
            wchar_t padChar;
            struct
            {
                wchar_t* pStr;
            } expected;
        }
        data[] =
        {
            { 18446744073709551615LLU,    0, false,  1, L'#', { L"18446744073709551615"       }},
            { 18446744073709551615LLU, L',', true ,  1, L'#', { L"-9,223,372,036,854,775,807" }},
            { 184                    ,    0, false,  1, L'#', { L"184"                        }},
            { 1844                   , L'.', false,  1, L'#', { L"1.844"                      }},

            { 184                    ,    0, false,  5, L'#', { L"##184"                      }},
            { 184                    ,    0, true ,  5, L'#', { L"-#184"                      }},
            { 18446744073709551615LLU, L',', true , 99, L'#', { L"-9,223,372,036,854,775,807" }},
            { 18446744073709551615LLU,    0, false, 99, L'#', { L"18446744073709551615"       }},
            {  8446744073709551615LLU,    0, false, 99, L'#', { L"#8446744073709551615"       }},
        };

        for (const auto item : data)
        {
            wchar_t buffer[ConvertW::MaxStrLen64_Commas_Signed + 2];
            memset(buffer, L'z', sizeof(buffer));

            wchar_t* pEnd = nullptr;
            wchar_t* pStr = ConvertW::ToStrPadded64U(item.val, buffer, item.width, item.padChar, item.groupSeparator, item.isNegative, &pEnd);

            REQUIRE(wstring_view(pStr) == item.expected.pStr); //-V521
            REQUIRE(static_cast<size_t>(pEnd - pStr) == wcslen(pStr)); //-V814 //-V521
        }
    }
}

TEST_CASE("Util/ConvertW[Number]::AppendFixedFast")
{
    SECTION("wchar_t")
    {
        struct
        {
            uint64_t val;
            size_t width;
            struct
            {
                wchar_t* pStr;
            } expected;
        }
        data[]
        {
            {     0, 0, { L"z"     } },
            {     9, 0, { L"z"     } },
            {    99, 0, { L"z"     } },
            {   999, 0, { L"z"     } },
            {     0, 1, { L"z0"    } },
            {     9, 1, { L"z9"    } },
            {    19, 1, { L"z9"    } },
            {   195, 1, { L"z5"    } },
            {     0, 2, { L"z00"   } },
            {     9, 2, { L"z09"   } },
            {    19, 2, { L"z19"   } },
            {   195, 2, { L"z95"   } },
            {     0, 3, { L"z000"  } },
            {     9, 3, { L"z009"  } },
            {    19, 3, { L"z019"  } },
            {   195, 3, { L"z195"  } },
            {  4195, 3, { L"z195"  } },
            { 74195, 3, { L"z195"  } },

            {                    0, 20, { L"z00000000000000000000" } },
            {                   90, 20, { L"z00000000000000000090" } },
            {             74195000, 20, { L"z00000000000074195000" } },
            { 18446744073709551615, 20, { L"z18446744073709551615" } },

            {                    0, 21, { L"z000000000000000000000" } },
            {                   90, 21, { L"z000000000000000000090" } },
            {             74195000, 21, { L"z000000000000074195000" } },
            { 18446744073709551615, 21, { L"z018446744073709551615" } },
        };

        for (const auto& item : data)
        {
            wstring str(L"z");
            REQUIRE(ConvertW::AppendFixedFast(str, item.val, item.width) == item.expected.pStr); //-V521
        }
    }
}

TEST_CASE("Util/ConvertW[Number]::ToStrFixedFast")
{
    SECTION("wchar_t")
    {
        struct
        {
            uint64_t val;
            size_t width;
            struct
            {
                wchar_t* pStr;
            } expected;
        }
        data[]
        {
            {     0, 0, { L""     } },
            {     9, 0, { L""     } },
            {    99, 0, { L""     } },
            {   999, 0, { L""     } },
            {     0, 1, { L"0"    } },
            {     9, 1, { L"9"    } },
            {    19, 1, { L"9"    } },
            {   195, 1, { L"5"    } },
            {     0, 2, { L"00"   } },
            {     9, 2, { L"09"   } },
            {    19, 2, { L"19"   } },
            {   195, 2, { L"95"   } },
            {     0, 3, { L"000"  } },
            {     9, 3, { L"009"  } },
            {    19, 3, { L"019"  } },
            {   195, 3, { L"195"  } },
            {  4195, 3, { L"195"  } },
            { 74195, 3, { L"195"  } },

            {                    0, 20, { L"00000000000000000000" } },
            {                   90, 20, { L"00000000000000000090" } },
            {             74195000, 20, { L"00000000000074195000" } },
            { 18446744073709551615, 20, { L"18446744073709551615" } },

            {                    0, 21, { L"000000000000000000000" } },
            {                   90, 21, { L"000000000000000000090" } },
            {             74195000, 21, { L"000000000000074195000" } },
            { 18446744073709551615, 21, { L"018446744073709551615" } },
        };

        for (const auto& item : data)
        {
            wchar_t buffer[ConvertW::MinBufferLen];
            for (wchar_t& ch : buffer)
            {
                ch = L'z';
            }

            wchar_t* pStr = ConvertW::ToStrFixedFast(buffer, item.val, item.width);
            REQUIRE(*pStr == L'z'); //-V521
            *pStr = 0;
            REQUIRE(wcscmp(buffer, item.expected.pStr) == 0); //-V521
        }
    }
}
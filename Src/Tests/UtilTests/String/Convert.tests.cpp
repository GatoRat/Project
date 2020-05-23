// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/Convert.h"
#include "TestHelper/catch.hpp"

using namespace std;

// Note: this file is saved as UTF-8, do not change this.
static const char test_str[] = u8"ぬるを";
static const wchar_t test_wstr[] = L"ぬるを";

TEST_CASE("string/Convert::ToStr(const wchar_t*)")
{
    //SECTION("nullptr")
    //{
    //    string s = Convert::ToUtf8(nullptr);
    //    REQUIRE(s.empty());
    //}
    SECTION("empty")
    {
        string s = Convert::ToUtf8(L"");
        REQUIRE(s.empty()); //-V521
    }
    SECTION("string")
    {
        string s = Convert::ToUtf8(test_wstr);
        REQUIRE(s == test_str); //-V521
    }
}

TEST_CASE("Util/Convert::ToStr(const wchar_t*)")
{
    //SECTION("nullptr")
    //{
    //    wstring s = Convert::ToWide(nullptr);
    //    REQUIRE(s.empty());
    //}
    SECTION("empty")
    {
        wstring s = Convert::ToWide("");
        REQUIRE(s.empty()); //-V521
    }
    SECTION("string")
    {
        wstring s = Convert::ToWide(test_str);
        REQUIRE(s == test_wstr); //-V521
    }
}

/*
// Note: this file is saved as UTF-8, do not change this.
static const char test_str[] = u8"ぬるを";
static const wchar_t test_wstr[] = L"ぬるを";

TEST_CASE("string/Convert::ToStr(const char*)")
{
    SECTION("nullptr")
    {
        string s = Convert::ToStr((const char*) nullptr);
        REQUIRE(s.empty());
    }
    SECTION("empty")
    {
        string s = Convert::ToStr("");
        REQUIRE(s.empty());
    }
    SECTION("string")
    {
        string s = Convert::ToStr(test_str);
        REQUIRE(s == test_str);
    }
}

TEST_CASE("Util/Convert::ToStr(const wchar_t*)")
{
    SECTION("nullptr")
    {
        string s = Convert::ToStr((const wchar_t*) nullptr);
        REQUIRE(s.empty());
    }
    SECTION("empty")
    {
        string s = Convert::ToStr(L"");
        REQUIRE(s.empty());
    }
    SECTION("string")
    {
        string s = Convert::ToStr(test_wstr);
        REQUIRE(s == test_str);
    }
}

TEST_CASE("Util/Convert::ToStr(const wstring&)")
{
    SECTION("empty")
    {
        wstring ws;
        string s = Convert::ToStr(ws);
        REQUIRE(s.empty());
    }
    SECTION("wstring")
    {
        wstring ws(test_wstr);
        string s = Convert::ToStr(ws);
        REQUIRE(s == test_str);
    }
}

TEST_CASE("Util/Convert::ToWStr(const char*)")
{
    SECTION("nullptr")
    {
        wstring s = Convert::ToWStr((const char*) nullptr);
        REQUIRE(s.empty());
    }
    SECTION("empty")
    {
        wstring s = Convert::ToWStr("");
        REQUIRE(s.empty());
    }
    SECTION("wstring")
    {
        wstring s = Convert::ToWStr(test_str);
        REQUIRE(s == test_wstr);
    }
}

TEST_CASE("Util/Convert::ToWStr(const string&)")
{
    SECTION("empty")
    {
        string s;
        wstring ws = Convert::ToWStr(s);
        REQUIRE(ws.empty());
    }
    SECTION("string")
    {
        string s(test_str);
        wstring ws = Convert::ToWStr(s);
        REQUIRE(s == test_str);
        REQUIRE(ws == test_wstr);
    }
}
*/

TEST_CASE("Util/Convert::ToUint64")
{
    SECTION("Without Offset")
    {
        struct
        {
            const char* pStr;
            struct
            {
                bool      rval;
                uint64_t  result;
            } expected;
        }
        static data[] =
        {
            { "1"   , { true , 1 }},
            { "2"   , { true , 2 }},
            { "3"   , { true , 3 }},
            { "1."  , { true , 1 }},
            { "2."  , { true , 2 }},
            { "3."  , { true , 3 }},
            { " 4"  , { true , 4 }},
            { ""    , { false, 0 }},
            { nullptr,{ false, 0 }},
            { "18446744073709551615", { true, 18446744073709551615LLU }}
        };

        for (const auto item : data)
        {
            uint64_t result;
            REQUIRE(item.expected.rval == Convert::ToUint64(item.pStr, result)); //-V521
            REQUIRE(item.expected.result == result); //-V521
        }
    }
    SECTION("With Offset")
    {
        struct
        {
            const char* pStr;
            struct
            {
                bool      rval;
                uint64_t  result;
                ptrdiff_t offset;
            } expected;
        }
        static data[] =
        {
            { ""     , { false,  0, 0 }},
            { "1"    , { true ,  1, 1 }},
            { "2."   , { true ,  2, 1 }},
            { "31."  , { true , 31, 2 }},
            { " 4"   , { true ,  4, 2 }},
            { nullptr, { false,  0, 0 }},
            { "18446744073709551615", { true, 18446744073709551615LLU, 20 }}
        };

        for (const auto item : data)
        {
            const char* pEnd = nullptr;
            uint64_t result;
            REQUIRE(item.expected.rval == Convert::ToUint64(item.pStr, result, &pEnd)); //-V521
            REQUIRE(item.expected.result == result); //-V521
            REQUIRE(item.expected.offset == pEnd - item.pStr); //-V521
        }
    }
}

TEST_CASE("Util/Convert::ToInt64")
{
    struct
    {
        const char* pStr;
        struct
        {
            bool rval;
            int64_t result;
        } expected;
    }
    static data[] =
    {
        { "9223372036854775807" , { true , INT64_MAX }},
        { " 9223372036854775807", { true , INT64_MAX }},
        { "-9223372036854775808", { true , INT64_MIN }},
        { " 0"                  , { true , 0         }},
        { ""                    , { false, 0         }},
        { nullptr               , { false, 0         }}
    };

    for (const auto item : data)
    {
        int64_t result;
        REQUIRE(item.expected.rval == Convert::ToInt64(item.pStr, result)); //-V521
        REQUIRE(item.expected.result == result); //-V521
    }
}

TEST_CASE("Util/Convert::ToInt64 (with commas)")
{
    struct
    {
        const char* pStr;
        struct
        {
            bool rval;
            int64_t result;
        } expected;
    }
    static data[] =
    {
        { "9,223,372,036,854,775,807" , { true , INT64_MAX }},
        { " 9,223,372,036,854,775,807", { true , INT64_MAX }},
        { "-9,223,372,036,854,775,808", { true , INT64_MIN }},
        { " 0"                        , { true , 0         }},
        { ""                          , { false, 0         }},
        { nullptr                     , { false, 0         }}
    };

    for (const auto item : data)
    {
        int64_t result;
        REQUIRE(item.expected.rval == Convert::ToInt64(item.pStr, result, nullptr, true)); //-V521
        REQUIRE(item.expected.result == result); //-V521
    }
}

TEST_CASE("Util/Convert::ToUint32")
{
    SECTION("(const char* pStr, uint32_t& val)")
    {
        struct
        {
            const char* pStr;
            struct
            {
                bool     rval;
                unsigned result;
            } expected;
        }
        static data[] =
        {
            { "1"    , { true , 1 }},
            { "2"    , { true , 2 }},
            { "3"    , { true , 3 }},
            { " 3"   , { true , 3 }},
            { "1."   , { true , 1 }},
            { "2."   , { true , 2 }},
            { "3."   , { true , 3 }},
            { " 3."  , { true , 3 }},
            { ""     , { false, 0 }},
            { nullptr, { false, 0 }},
            { "4294967295", { true, 4294967295u }} //-V112
        };

        for (const auto item : data)
        {
            uint32_t result;
            REQUIRE(Convert::ToUint32(item.pStr, result) == item.expected.rval); //-V521
            REQUIRE(result == item.expected.result); //-V521
        }
    }
    SECTION("(const char* pStr, uint32_t& val, const char** pEnd)")
    {
        struct
        {
            const char* pStr;
            struct
            {
                bool      rval;
                uint32_t  result;
                ptrdiff_t offset;
            } expected;
        }
        static data[] =
        {
            { "",    { false,  0,  0 }},
            { "1",   { true ,  1,  1 }},
            { "2.",  { true,   2,  1 }},
            { "31.", { true , 31,  2 }},
            { "4294967295", { true, 4294967295u, 10 }} //-V112
        };

        for (const auto item : data)
        {
            const char* pEnd = nullptr;
            uint32_t result;
            REQUIRE(item.expected.rval == Convert::ToUint32(item.pStr, result, &pEnd)); //-V521
            REQUIRE(item.expected.result == result); //-V521
            REQUIRE(item.expected.offset == pEnd - item.pStr); //-V521
        }
    }
}

TEST_CASE("Util/Convert::ToInt32")
{
    struct
    {
        const char* pStr;
        struct
        {
            bool rval;
            int32_t result;
        } expected;
    }
    static data[] =
    {
        { "2147483647",   { true , 2147483647  }}, //-V112
        { " 2147483647",  { true , 2147483647  }}, //-V112
        { "-2147483248",  { true , -2147483248 }},
        { " -2147483248", { true , -2147483248 }},
        { " 0",           { true , 0           }},
        { nullptr,        { false, 0           }}
    };

    for (const auto item : data)
    {
        int32_t result;
        REQUIRE(Convert::ToInt32(item.pStr, result) == item.expected.rval); //-V521
        REQUIRE(result == item.expected.result); //-V521
    }
}

TEST_CASE("Util/Convert::ToUint64_FromView")
{
    SECTION("data")
    {
        struct
        {
            const char* pStr;
            struct
            {
                Convert::ToUint64Err err;
                uint64_t result;
            } expected;
        }
        data[] =
        {
            { "3"                   , { Convert::ToUint64_Success    , 3 } },
            { "4."                  , { Convert::ToUint64_EndWithChar, 4 } },
            { " 5"                  , { Convert::ToUint64_NotNumber  , 0 } },
            { ""                    , { Convert::ToUint64_Empty      , 0 } },
            { "18446744073709551615", { Convert::ToUint64_Success    , 18446744073709551615LLU } }
        };

        for (const auto item : data)
        {
            Convert::ToUint64Err err;
            string_view str(item.pStr);
            REQUIRE(item.expected.result == Convert::ToUint64_FromView(str, err)); //-V521
            REQUIRE(item.expected.err == err); //-V521
        }
    }
    SECTION("embedded zero")
    {
        string str("5");
        str.push_back(0);
        str.push_back('9');

        string_view strView(str);
        Convert::ToUint64Err err;
        REQUIRE(Convert::ToUint64_FromView(strView, err) == 5); //-V521
        REQUIRE(err == Convert::ToUint64_EndWithChar); //-V521
        REQUIRE(strView.size() == 2); //-V521
        REQUIRE(strView.front() == 0); //-V521
    }
}

TEST_CASE("Util/Convert::Quick")
{
    const char* pStr = "1234";
    REQUIRE(Convert::Quick4(pStr) == 1234); //-V521
    REQUIRE(*pStr == 0); //-V521

    pStr = "123";
    REQUIRE(Convert::Quick4(pStr) < 0); //-V521
    REQUIRE(*pStr == '1'); //-V521

    pStr = "56";
    REQUIRE(Convert::Quick2(pStr) == 56); //-V521
    REQUIRE(*pStr == 0); //-V521

    pStr = "7";
    REQUIRE(Convert::Quick2(pStr) < 0); //-V521
    REQUIRE(*pStr == '7'); //-V521
}

TEST_CASE("Util/Convert::BinToHex")
{
    SECTION("(string&, nullptr, 0, false, 0)")
    {
        string str(":");
        Convert::AppendHex(str, nullptr, 0, false, 0);
        REQUIRE(str == ":"); //-V521
    }
    SECTION("(string&, <data>, 0, false, 0)")
    {
        string str(":");
        Convert::AppendHex(str, "abc", 0, false, 0); //-V666
        REQUIRE(str == ":"); //-V521
    }
    SECTION("(string&, nullptr, 5, false, 0)")
    {
        string str(":");
        Convert::AppendHex(str, nullptr, 5, false, 0);
        REQUIRE(str == ":"); //-V521
    }
    SECTION("(string&, <data>, 3, false, 0)")
    {
        string str(":");
        Convert::AppendHex(str, "a+Z", 3, false, 0);
        REQUIRE(str == ":612b5a"); //-V521
    }
    SECTION("(string&, <data>, 3, true, 0)")
    {
        string str(":");
        Convert::AppendHex(str, "a+Z", 3, true, 0);
        REQUIRE(str == ":612B5A"); //-V521
    }
    SECTION("(string&, <data>, 3, false, ' ')")
    {
        string str(":");
        Convert::AppendHex(str, "a+Z", 3, false, ' ');
        REQUIRE(str == ":61 2b 5a"); //-V521
    }
    SECTION("(string&, <data>, 3, true, ' ')")
    {
        string str(":");
        Convert::AppendHex(str, "a+Z", 3, true, ' ');
        REQUIRE(str == ":61 2B 5A"); //-V521
    }
}
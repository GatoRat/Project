// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <sstream>
#include "Util/String/Str.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Util/Str::IsNull()")
{
    struct
    {
        const char* pStr;
        bool isNull;
        bool isNullOrEmpty;
        bool isNullOrWhiteSpace;
        bool isEmptyOrWhiteSpace;
    }
    static data[] =
    {
        { " "    ,  false, false,  true,  true },
        { nullptr,   true,  true,  true,  true },
        { ""     ,  false,  true,  true,  true },
        { " "    ,  false, false,  true,  true },
        { " 1"   ,  false, false, false, false },
    };

    for (const auto& item : data)
    {
        REQUIRE(Str::IsNull(item.pStr) == item.isNull); //-V521
        REQUIRE(Str::IsNullOrEmpty(item.pStr) == item.isNullOrEmpty); //-V521
        REQUIRE(Str::IsNullOrWhiteSpace(item.pStr) == item.isNullOrWhiteSpace); //-V521
        REQUIRE(Str::IsEmptyOrWhiteSpace(item.pStr) == item.isEmptyOrWhiteSpace); //-V521

        if (item.pStr)
        {
            string str(item.pStr);
            REQUIRE(Str::IsEmptyOrWhiteSpace(str) == item.isEmptyOrWhiteSpace); //-V521
        }
    }
}

TEST_CASE("Util/Str::IsNull(wchar_t)")
{
    struct
    {
        const wchar_t* pStr;
        bool isNull;
        bool isNullOrEmpty;
        bool isNullOrWhiteSpace;
        bool isEmptyOrWhiteSpace;
    }
    static data[] =
    {
        { L" "   ,  false, false,  true,  true },
        { nullptr,   true,  true,  true,  true },
        { L""    ,  false,  true,  true,  true },
        { L" "   ,  false, false,  true,  true },
        { L" 1"  ,  false, false, false, false },
    };

    for (const auto& item : data)
    {
        REQUIRE(Str::IsNull(item.pStr) == item.isNull); //-V521
        REQUIRE(Str::IsNullOrEmpty(item.pStr) == item.isNullOrEmpty); //-V521
        REQUIRE(Str::IsNullOrWhiteSpace(item.pStr) == item.isNullOrWhiteSpace); //-V521

        if (item.pStr)
        {
            wstring str(item.pStr);
            REQUIRE(Str::IsEmptyOrWhiteSpace(str) == item.isEmptyOrWhiteSpace); //-V521
        }
    }
}

TEST_CASE("Util/Str::GetLen(const char*)")
{
    struct
    {
        const char* pStr;
        size_t expected;
    }
    data[] =
    {
        { nullptr, 0 },
        { ""     , 0 },
        { " "    , 1 },
        { "One"  , 3 },
    };

    for (const auto& item : data)
    {
        REQUIRE(Str::GetLen(item.pStr) == item.expected); //-V521
    }
}

TEST_CASE("Util/Str::ReserveAtLeast")
{
    string str{ "a" };

    REQUIRE(Str::ReserveAtLeast(str, 10).capacity() >= 10); //-V521
    REQUIRE(Str::ReserveAtLeast(str, 5 ).capacity() >= 10); //-V521
    REQUIRE(Str::ReserveAtLeast(str, 20).capacity() >= 20); //-V521
    REQUIRE(str == "a"); //-V521
}

TEST_CASE("Util/Str::ReserveAdditional")
{
    string str{ "a" };

    REQUIRE(Str::ReserveAtLeast(str, 10).capacity() >= 10); //-V521
    REQUIRE(Str::ReserveAtLeast(str, 5).capacity() >= 15); //-V521
    REQUIRE(Str::ReserveAtLeast(str, 20).capacity() >= 35); //-V521
    REQUIRE(str == "a"); //-V521
}

TEST_CASE("Util/Str::Trim(const char*)")
{
    SECTION("TrimLeft")
    {
        struct
        {
            const char* pStr;
            string expected;
        }
        data[] =
        {
            { nullptr , ""    },
            { ""      , ""    },
            { "One"   , "One" },
            { " One"  , "One" },
            { " \tOne", "One" },
            { " One " , "One "},
            { "One "  , "One "},
        };

        for (const auto& item : data)
        {
            string_view returnStr = Str::TrimLeft(item.pStr);
            REQUIRE(returnStr == item.expected); //-V521
        }
    }
    SECTION("TrimRight")
    {
        struct
        {
            const char* pStr;
            string expected;
        }
        data[] =
        {
            { nullptr , ""    },
            { ""      , ""    },
            { "One"   , "One" },
            { "One "  , "One" },
            { "One\t ", "One" },
            { " One " , " One"},
            { " One"  , " One"},
        };

        for (const auto& item : data)
        {
            string_view returnStr = Str::TrimRight(item.pStr);
            REQUIRE(returnStr == item.expected); //-V521
        }
    }
    SECTION("Trim")
    {
        struct
        {
            const char* pStr;
            string expected;
        }
        data[] =
        {
            { nullptr    , ""    },
            { ""         , ""    },
            { "One"      , "One" },
            { "One "     , "One" },
            { " \tOne\t ", "One" }
        };

        for (const auto& item : data)
        {
            string_view returnStr = Str::Trim(item.pStr);
            REQUIRE(returnStr == item.expected); //-V521
        }
    }
}

TEST_CASE("Util/Str::TrimView(string_view)")
{
    SECTION("TrimLeftView")
    {
        struct
        {
            const char* pStr;
            string expected;
        }
        data[] =
        {
            { ""      , ""     },
            { "\t\r " , ""     },
            { "One"   , "One"  },
            { " One"  , "One"  },
            { " \tOne", "One"  },
            { " One " , "One " },
            { "One "  , "One " },
        };

        for (const auto& item : data)
        {
            string_view returnStr = Str::TrimLeftView(item.pStr);
            REQUIRE(returnStr == item.expected); //-V521
        }
    }
    SECTION("TrimRightView")
    {
        struct
        {
            const char* pStr;
            string expected;
        }
        data[] =
        {
            { ""      , ""     },
            { "\t\r " , ""     },
            { "One"   , "One"  },
            { "One "  , "One"  },
            { "One\t ", "One"  },
            { " One " , " One" },
            { " One"  , " One" },
        };

        for (const auto& item : data)
        {
            string_view returnStr = Str::TrimRightView(item.pStr);
            REQUIRE(returnStr == item.expected); //-V521
        }
    }
    SECTION("TrimView")
    {
        struct
        {
            const char* pStr;
            string expected;
        }
        data[] =
        {
            { ""         , ""    },
            { "\t\r "    , ""    },
            { "One"      , "One" },
            { "One "     , "One" },
            { " \tOne\t ", "One" }
        };

        for (const auto& item : data)
        {
            string_view returnStr = Str::TrimView(item.pStr);
            REQUIRE(returnStr == item.expected); //-V521
        }
    }
}

TEST_CASE("Util/Str::Trim(string)")
{
    SECTION("TrimLeft")
    {
        struct
        {
            string str;
            string expected;
        }
        data[] =
        {
            { ""      , ""    },
            { "One"   , "One" },
            { " One"  , "One" },
            { " \tOne", "One" },
            { " One " , "One "},
            { "One "  , "One "},
        };

        for (const auto& item : data)
        {
            string str = item.str;
            string returnStr = Str::TrimLeft(str);

            REQUIRE(str == item.expected); //-V521
            REQUIRE(returnStr == item.expected); //-V521
        }
    }
    SECTION("TrimRight")
    {
        struct
        {
            string str;
            string expected;
        }
        data[] =
        {
            { ""      , ""    },
            { " "     , ""    },
            { "One"   , "One" },
            { "One "  , "One" },
            { "One\t ", "One" },
            { " One " , " One"},
            { " One"  , " One"},
        };

        for (const auto& item : data)
        {
            string str = item.str;
            string returnStr = Str::TrimRight(str);

            REQUIRE(str == item.expected); //-V521
            REQUIRE(returnStr == item.expected); //-V521
        }
    }
    SECTION("Trim")
    {
        struct
        {
            string str;
            string expected;
        }
        data[] =
        {
            { ""         , ""    },
            { "One"      , "One" },
            { "One "     , "One" },
            { " \tOne\t ", "One" }
        };

        for (const auto& item : data)
        {
            string str = item.str;
            string returnStr = Str::Trim(str);

            REQUIRE(str == item.expected); //-V521
            REQUIRE(returnStr == item.expected); //-V521
        }
    }
}

TEST_CASE("Util/Str::StartsWith()")
{
    struct
    {
        const char* pStr;
        const char* pEnd;
        bool ignoreCase;
        bool expected;
    }
    static startsWithData[] =
    {
        { "pin",   "pi"   ,  true,  true },
        { "pin",   "pI"   ,  true,  true },
        { "pIn",   "pi"   ,  true,  true },
        { "pin",   "pi"   , false,  true },
        { "pin",   "pI"   , false, false },
        { "pIn",   "pi"   , false, false },
        { "pin",   "pin"  ,  true,  true },
        { "pin",   "pIn"  ,  true,  true },
        { "pIn",   "pin"  ,  true,  true },
        { "pin",   "pin"  , false,  true },
        { "pin",   "pIn"  , false, false },
        { "pIn",   "pin"  , false, false },
        { "pin",   "pins" , false, false },
        { "pin",   "pIns" , false, false },
        { "pIn",   "pins" , false, false },
        { "pin",   "pins" , false, false },
        { "pin",   "pIns" , false, false },
        { "pIn",   "pins" , false, false },
        { "pin",   ""     , false, false },
        { ""   ,   "pi"   , false, false },
        { ""   ,   ""     , false, false },
    };

    size_t testNum = 0;
    for (const auto& data : startsWithData)
    {
        bool result = Str::StartsWith(data.pStr, data.pEnd, data.ignoreCase);
        if (result != data.expected)
        {
            stringstream ss;
            ss << "test: " << testNum <<
                " (" << data.pStr << ',' << data.pEnd << ',' << boolalpha << data.ignoreCase << ")"
                " expected: " << boolalpha << data.expected <<
                " returned: " << boolalpha << result;

            FAIL(ss.str().c_str());
        }

        ++testNum;
    }
}

TEST_CASE("Util/Str::EndsWith(char)")
{
    struct
    {
        const char* pStr;
        const char* pEnd;
        bool ignoreCase;
        bool expected;
    }
    static endsWithData[] =
    {
        {   "pin",    "in",  true,  true },
        {   "pin",    "In",  true,  true },
        {   "piN",    "in",  true,  true },
        {   "pin",    "in", false,  true },
        {   "pin",    "In", false, false },
        {   "piN",    "in", false, false },
        {   "pin",   "pin",  true,  true },
        {   "pin",   "pIn",  true,  true },
        {   "piN",   "pin",  true,  true },
        {   "pin",   "pin", false,  true },
        {   "pin",   "pIn", false, false },
        {   "piN",   "pin", false, false },
        {   "pin",  "spin",  true, false },
        {   "pin",  "spIn",  true, false },
        {   "piN",  "spin",  true, false },
        {   "pin",  "spin", false, false },
        {   "pin",  "spIn", false, false },
        {   "piN",  "spin", false, false },
        {   "pin",      "", false, false },
        {      "",    "in", false, false },
        {      "",      "", false, false },
    };

    for (const auto data : endsWithData)
    {
        REQUIRE(Str::EndsWith((const char*) data.pStr, (const char*) data.pEnd, //-V521
            data.ignoreCase) == data.expected);
    }
}

TEST_CASE("Util/Str::EndsWith(wchar_t)")
{
    struct
    {
        const wchar_t* pStr;
        const wchar_t* pEnd;
        bool ignoreCase;
        bool expected;
    }
    static endsWithData[] =
    {
        {   L"pin",    L"in",  true,  true },
        {   L"pin",    L"In",  true,  true },
        {   L"piN",    L"in",  true,  true },
        {   L"pin",    L"in", false,  true },
        {   L"pin",    L"In", false, false },
        {   L"piN",    L"in", false, false },
        {   L"pin",   L"pin",  true,  true },
        {   L"pin",   L"pIn",  true,  true },
        {   L"piN",   L"pin",  true,  true },
        {   L"pin",   L"pin", false,  true },
        {   L"pin",   L"pIn", false, false },
        {   L"piN",   L"pin", false, false },
        {   L"pin",  L"spin",  true, false },
        {   L"pin",  L"spIn",  true, false },
        {   L"piN",  L"spin",  true, false },
        {   L"pin",  L"spin", false, false },
        {   L"pin",  L"spIn", false, false },
        {   L"piN",  L"spin", false, false },
        {   L"pin",      L"", false, false },
        {      L"",    L"in", false, false },
        {      L"",      L"", false, false }
    };

    for (const auto data : endsWithData)
    {
        REQUIRE(Str::EndsWith(data.pStr, data.pEnd, data.ignoreCase) == data.expected); //-V521
    }
}

TEST_CASE("Util/Str::GetEolLen")
{
    struct
    {
        const char* pStr;
        int expected;
    }
    static data[] =
    {
        { nullptr   , 0 },
        { ""        , 0 },
        { "\n"      , 1 },
        { "\n\r"    , 1 },
        { "\r\n"    , 2 },
        { "\r\n\n\r", 2 }
    };

    for (const auto& item : data)
    {
        REQUIRE(Str::GetEolLen(item.pStr) == item.expected); //-V521
    }
}

TEST_CASE("Util/Str::CalcLineColumn")
{
    struct
    {
        const char* pStr;
        size_t      offset;
        struct
        {
            size_t  line;
            size_t  column;
        }           expected;
    }
    static data[] =
    {
        { nullptr   , 0, { 0, 0 } },
        { nullptr   , 1, { 0, 0 } },
        { ""        , 0, { 0, 0 } },
        { ""        , 1, { 0, 0 } },

        { "abc\ndef", 0, { 1, 1 } },
        { "abc\ndef", 1, { 1, 2 } },
        { "abc\ndef", 2, { 1, 3 } },
        { "abc\ndef", 3, { 1, 4 } },
        { "abc\ndef", 4, { 2, 1 } },
        { "abc\ndef", 5, { 2, 2 } },
        { "abc\ndef", 6, { 2, 3 } },
        { "abc\ndef", 7, { 2, 4 } },
        { "abc\ndef", 8, { 2, 4 } },

        { "abc\r\ndef", 1, { 1, 2 } },
        { "abc\r\ndef", 2, { 1, 3 } },
        { "abc\r\ndef", 3, { 1, 4 } },
        { "abc\r\ndef", 4, { 1, 4 } },
        { "abc\r\ndef", 5, { 2, 1 } },
        { "abc\r\ndef", 6, { 2, 2 } },
        { "abc\r\ndef", 7, { 2, 3 } },
        { "abc\r\ndef", 8, { 2, 4 } },
    };

    for (const auto& item : data)
    {
        Str::LineColumn lineColumn(item.pStr, item.offset);
        REQUIRE(lineColumn.line   == item.expected.line); //-V521
        REQUIRE(lineColumn.column == item.expected.column); //-V521
    }
}

TEST_CASE("Util/Str::RemoveAll")
{
    struct
    {
        string str;
        const char* pFind;
        string expected;
    }
    static data[] =
    {
        { ""        , "aa"   , ""         },
        { "AAaaAaaA", "aa"   , "AAAA"     },
        { "AAaaAaaA", ""     , "AAaaAaaA" },
        { "aaaaaa"  , "aa"   , ""         },
        { "aaaaaaa" , "aa"   , "a"        }
    };

    for (const auto& item : data)
    {
        string str(item.str);
        string result = Str::RemoveAll(str, item.pFind);
        REQUIRE(str == item.expected); //-V521
        REQUIRE(result == item.expected); //-V521
    }
}

TEST_CASE("Util/Str::ReplaceAll")
{
    struct
    {
        string str;
        const char* pFind;
        const char* pReplace;
        string expected;
    }
    static data[] =
    {
        { ""        , "aa"   , "123"  , ""           },
        { "AAaaAaaA", "aa"   , "123"  , "AA123A123A" },
        { "aaaaaa"  , "aa"   , "123"  , "123123123"  },
        { "aaaaaaa" , "aa"   , "123"  , "123123123a" },
        { "AAaaAaaA", "aa"   , "x"    , "AAxAxA"     },
        { "aaaaaa"  , "aa"   , "x"    , "xxx"        },
        { "aaaaaaa" , "aa"   , "x"    , "xxxa"       },
        { "AAaaAaaA", ""     , ""     , "AAaaAaaA"   },
        { "AAaaAaaA", ""     , "123"  , "AAaaAaaA"   },
        { "AAaaAaaA", "aa"   , ""     , "AAAA"       },
    };

    for (const auto& item : data)
    {
        string str(item.str);
        string result = Str::ReplaceAll(str, item.pFind, item.pReplace);
        REQUIRE(str == item.expected); //-V521
        REQUIRE(result == item.expected); //-V521
    }
}

TEST_CASE("Util/Str::Copy")
{
    struct
    {
        const char* pSrc;
        size_t maxLen;
        bool fill;
        bool getErr;
        struct
        {
            size_t result;
            const char* pResult;
            bool   err;
        } expected;
    }
    static data[] =
    {
        { "123", 1, false, false, { 0, "\0BCDEF" , false } },
        { "123", 2, false, false, { 1, "1\0CDEF" , false } },
        { "123", 3, false, false, { 2, "12\0DEF" , false } },
        { "123", 4, false, false, { 3, "123\0EF" , false } },
        { "123", 5, false, false, { 3, "123\0EF" , false } },
        { "123", 6, false, false, { 3, "123\0EF" , false } },

        { "12",  1, true , false, { 0, "\0BCDEF" , false } },
        { "12",  2, true , false, { 1, "1\0CDEF" , false } },
        { "12",  3, true , false, { 2, "12\0DEF" , false } },
        { "12",  4, true , false, { 2, "12\0\0EF", false } },

        { "12",  1, false,  true, { 0, "\0BCDEF" , true  } },
        { "12",  2, false,  true, { 1, "1\0CDEF" , true  } },
        { "12",  3, false,  true, { 2, "12\0DEF" , false } },
        { "12",  4, false,  true, { 2, "12\0DEF" , false } },

        { "12",  1, true ,  true, { 0, "\0BCDEF" , true  } },
        { "12",  2, true ,  true, { 1, "1\0CDEF" , true  } },
        { "12",  3, true ,  true, { 2, "12\0DEF" , false } },
        { "12",  4, true ,  true, { 2, "12\0\0EF", false } },

        { ""  ,    1, false, false, { 0, "\0BCDEF"  , false } },
        { nullptr, 1, false, false, { 0, "ABCDEF"   , false } },
        { nullptr, 2, false, false, { 0, "ABCDEF"   , false } },
        { nullptr, 3, false, false, { 0, "ABCDEF"   , false } },

        { ""  ,    1, true , false, { 0, "\0BCDEF"  , false } },
        { nullptr, 1, true , false, { 0, "\0BCDEF"  , false } },
        { nullptr, 2, true , false, { 0, "\0\0CDEF" , false } },
        { nullptr, 3, true , false, { 0, "\0\0\0DEF", false } },

        { ""  ,    1, false, true,  { 0, "\0BCDEF"  , false } },
        { nullptr, 1, false, true,  { 0, "ABCDEF"   , false } },
        { nullptr, 2, false, true,  { 0, "ABCDEF"   , false } },
        { nullptr, 3, false, true,  { 0, "ABCDEF"   , false } },

        { ""  ,    1, true , true,  { 0, "\0BCDEF"  , false } },
        { nullptr, 1, true , true,  { 0, "\0BCDEF"  , false } },
        { nullptr, 2, true , true,  { 0, "\0\0CDEF" , false } },
        { nullptr, 3, true , true,  { 0, "\0\0\0DEF", false } },
    };

    SECTION("pDst = valid")
    {
        for (const auto& item : data)
        {
            char dst[7] = "ABCDEF";
            bool err = false;

            REQUIRE(Str::Copy(dst, item.pSrc, item.maxLen, item.fill, item.getErr ? &err : nullptr) == item.expected.result); //-V521
            REQUIRE(memcmp(dst, item.expected.pResult, sizeof(dst)) == 0); //-V521
            if (item.getErr)
            {
                REQUIRE(err == item.expected.err); //-V521
            }
        }
    }
    SECTION("pDst = nullptr")
    {
        for (const auto& item : data)
        {
            char dst[7] = "ABCDEF";
            bool err = false;

            REQUIRE(Str::Copy(nullptr, item.pSrc, item.maxLen, item.fill, item.getErr ? &err : nullptr) == 0); //-V521
            REQUIRE(strcmp(dst, "ABCDEF") == 0); //-V521
            if (item.getErr)
            {
                if (item.maxLen)
                {
                    REQUIRE(err); //-V521
                }
                else
                {
                    REQUIRE_FALSE(err); //-V521
                }
            }
        }
    }
    SECTION("maxLen = 0")
    {
        for (const auto& item : data)
        {
            char dst[7] = "ABCDEF";
            bool err = false;

            REQUIRE(Str::Copy(dst, item.pSrc, 0, item.fill, item.getErr ? &err : nullptr) == 0); //-V521
            REQUIRE(strcmp(dst, "ABCDEF") == 0); //-V521
            if (item.getErr)
            {
                REQUIRE_FALSE(err); //-V521
            }
        }
    }
}

TEST_CASE("Util/Str::Copy[]")
{
    bool err = false;
    char dst[5] = "AB\0D";
    REQUIRE(Str::Copy(dst, "123456", false, &err) == 4); //-V112 //-V521
    REQUIRE(strcmp(dst, "1234") == 0); //-V521
    REQUIRE(err); //-V521
}

TEST_CASE("Util/Str::Right")
{
    struct
    {
        const char* pStr;
        size_t maxChars;
        string expected;
    }
    data[] =
    {
        { "1234", 0, ""     },
        { "1234", 1, "4"    },
        { "1234", 2, "34"   },
        { "1234", 3, "234"  },
        { "1234", 4, "1234" },
        { "1234",99, "1234" },
    };

    for (const auto& item : data)
    {
        string returnStr = Str::Right(item.pStr, item.maxChars);
        REQUIRE(returnStr == item.expected); //-V521
    }
}

TEST_CASE("Util/Str::RightView")
{
    struct
    {
        const char* pStr;
        size_t maxChars;
        string expected;
    }
    data[] =
    {
        { "1234", 0, "" },
        { "1234", 1, "4" },
        { "1234", 2, "34" },
        { "1234", 3, "234" },
        { "1234", 4, "1234" },
        { "1234",99, "1234" },
    };

    for (const auto& item : data)
    {
        string_view returnStr = Str::RightView(item.pStr, item.maxChars);
        REQUIRE(returnStr == item.expected); //-V521
    }
}

TEST_CASE("Util/Str::Concat(const char*)")
{
    SECTION("'ab' + '34'")
    {
        REQUIRE(Str::Concat("ab", "34") == string("ab34")); //-V521
    }
    SECTION("'ab' + ''")
    {
        REQUIRE(Str::Concat("ab", "") == string("ab")); //-V521
    }
    SECTION("'ab' + nullptr")
    {
        REQUIRE(Str::Concat("ab", (const char*) nullptr) == string("ab")); //-V521
    }
    SECTION("'' + ''")
    {
        REQUIRE(Str::Concat("", "").empty()); //-V521
    }
    SECTION("'' + '34'")
    {
        REQUIRE(Str::Concat("", "34") == string("34")); //-V521
    }
    SECTION("nullptr + '34'")
    {
        REQUIRE(Str::Concat((const char*) nullptr, "34") == string("34")); //-V521
    }
    SECTION("nullptr + nullptr")
    {
        REQUIRE(Str::Concat((const char*) nullptr, (const char*) nullptr).empty()); //-V521
    }
}

TEST_CASE("Util/Str::Concat(char*)")
{
    SECTION("'ab' + '34'")
    {
        char buffer[8] = "ab";
        REQUIRE(Str::Concat(buffer, "34", 8) == 4); //-V112 //-V521
        REQUIRE(strcmp(buffer, "ab34") == 0); //-V521
    }
    SECTION("'ab' + ''")
    {
        char buffer[8] = "ab";
        REQUIRE(Str::Concat(buffer, "", 8) == 2); //-V521
        REQUIRE(strcmp(buffer, "ab") == 0); //-V521
    }
    SECTION("'ab' + nullptr")
    {
        char buffer[8] = "ab";
        REQUIRE(Str::Concat(buffer, nullptr, 8) == 2); //-V521
        REQUIRE(strcmp(buffer, "ab") == 0); //-V521
    }
    SECTION("'' + ''")
    {
        char buffer[8] = "";
        REQUIRE(Str::Concat(buffer, nullptr, 8) == 0); //-V521
        REQUIRE(strcmp(buffer, "") == 0); //-V521
    }
    SECTION("'' + '34'")
    {
        char buffer[8] = "";
        REQUIRE(Str::Concat(buffer, "34", 8) == 2); //-V521
        REQUIRE(strcmp(buffer, "34") == 0); //-V521
    }
    SECTION("nullptr + '34'")
    {
        REQUIRE(Str::Concat((char*) nullptr, "34", 8) == 0); //-V521
    }
    SECTION("nullptr + nullptr")
    {
        REQUIRE(Str::Concat((char*) nullptr, nullptr, 8) == 0); //-V521
    }
    SECTION("'ab' + '34' (len = 0)")
    {
        char buffer[8] = "ab";
        REQUIRE(Str::Concat(buffer, "34", 0) == 0); //-V521
        REQUIRE(strcmp(buffer, "ab") == 0); //-V521
    }
    SECTION("'ab' + '34' (len = 1)")
    {
        char buffer[8] = "ab";
        REQUIRE(Str::Concat(buffer, "34", 1) == 0); //-V521
        REQUIRE(strcmp(buffer, "") == 0); //-V521
    }
    SECTION("'ab' + '34' (len = 2)")
    {
        char buffer[8] = "ab";
        REQUIRE(Str::Concat(buffer, "34", 2) == 1); //-V521
        REQUIRE(strcmp(buffer, "a") == 0); //-V521
    }
    SECTION("'ab' + '34' (len = 3)")
    {
        char buffer[8] = "ab";
        REQUIRE(Str::Concat(buffer, "34", 3) == 2); //-V521
        REQUIRE(strcmp(buffer, "ab") == 0); //-V521
    }
    SECTION("'ab' + '34' (len = 4)")
    {
        char buffer[8] = "ab";
        REQUIRE(Str::Concat(buffer, "34", 4) == 3); //-V112 //-V521
        REQUIRE(strcmp(buffer, "ab3") == 0); //-V521
    }
    SECTION("'ab' + '34' (len = 5)")
    {
        char buffer[8] = "ab";
        REQUIRE(Str::Concat(buffer, "34", 5) == 4); //-V112 //-V521
        REQUIRE(strcmp(buffer, "ab34") == 0); //-V521
    }
}

TEST_CASE("Util/Str::Concat[]")
{
    SECTION("'AB' + 'CD'")
    {
        char dst[5] = "AB";
        REQUIRE(Str::ConcatBuffer(dst, "CD") == 4); //-V112 //-V521
        REQUIRE(strcmp(dst, "ABCD") == 0); //-V521
    }
    SECTION("'AB' + 'CDE'")
    {
        char dst[5] = "AB";
        REQUIRE(Str::ConcatBuffer(dst, "CDE") == 4); //-V112 //-V521
        REQUIRE(strcmp(dst, "ABCD") == 0); //-V521
    }
    SECTION("'A' + 'B' + 'C'")
    {
        char dst[5] = "A";
        REQUIRE(Str::ConcatBuffer(dst, "B") == 2); //-V521
        REQUIRE(strcmp(dst, "AB") == 0); //-V521
        REQUIRE(Str::ConcatBuffer(dst, "C") == 3); //-V521
        REQUIRE(strcmp(dst, "ABC") == 0); //-V521
    }
}

TEST_CASE("Util/Str::ToUpper(const char*, trimLeft, trimRight)")
{
    SECTION("false, false")
    {
        REQUIRE(Str::ToUpper(" aB ", false, false) == " AB "); //-V521
    }
    SECTION("true, false")
    {
        REQUIRE(Str::ToUpper(" aB ", true, false) == "AB "); //-V521
    }
    SECTION("false, true")
    {
        REQUIRE(Str::ToUpper(" aB ", false, true) == " AB"); //-V521
    }
    SECTION("true, true")
    {
        REQUIRE(Str::ToUpper(" aB ", true, true) == "AB"); //-V521
    }
}

TEST_CASE("Util/Str::ToUpper()")
{
    REQUIRE(Str::ToUpper(string(" aB ")) == " AB "); //-V521
}

TEST_CASE("Util/Str::ToUpperInPlace()")
{
    string str(" aB ");
    REQUIRE(Str::ToUpperInPlace(str) == " AB "); //-V521
    REQUIRE(str == " AB "); //-V521
}

TEST_CASE("Util/Str::ToLower(const char*, trimLeft, trimRight)")
{
    SECTION("false, false")
    {
        REQUIRE(Str::ToLower(" aB ", false, false) == " ab "); //-V521
    }
    SECTION("true, false")
    {
        REQUIRE(Str::ToLower(" aB ", true, false) == "ab "); //-V521
    }
    SECTION("false, true")
    {
        REQUIRE(Str::ToLower(" aB ", false, true) == " ab"); //-V521
    }
    SECTION("true, true")
    {
        REQUIRE(Str::ToLower(" aB ", true, true) == "ab"); //-V521
    }
}

TEST_CASE("Util/Str::ToLower()")
{
    REQUIRE(Str::ToLower(string(" aB ")) == " ab "); //-V521
}

TEST_CASE("Util/Str::ToLowerInPlace()")
{
    string str(" aB ");
    REQUIRE(Str::ToLowerInPlace(str) == " ab "); //-V521
    REQUIRE(str == " ab "); //-V521
}

TEST_CASE("Util/Str::Compare()")
{
    struct
    { //-V802
        const char* pStr1;
        const char* pStr2;
        bool ignoreCase;
        size_t len;
        int expected;
    }
    static data[] =
    {
        { nullptr, nullptr, false, 0,  0 },
        { nullptr, nullptr, true , 0,  0 },
        { nullptr, nullptr, false, 1,  0 },
        { nullptr, nullptr, true , 1,  0 },

        { nullptr, "abc"  , false, 0, -1 },
        { nullptr, "abc"  , true , 0, -1 },
        { nullptr, "abc"  , false, 1, -1 },
        { nullptr, "abc"  , true , 1, -1 },

        { "abc"  , nullptr, false, 0,  1 },
        { "abc"  , nullptr, true , 0,  1 },
        { "abc"  , nullptr, false, 1,  1 },
        { "abc"  , nullptr, true , 1,  1 },

        { "abc", "abc", false, 0,  0 },
        { "abc", "abc", true , 0,  0 },
        { "abc", "abC", false, 0,  1 },
        { "abc", "abC", true , 0,  0 },
        { "abC", "abc", false, 0, -1 },
        { "abC", "abc", true , 0,  0 },
        { "abc", "ab1", false, 2,  0 },
        { "abc", "ab1", true , 2,  0 },
        { "abc", "aB1", false, 2,  1 },
        { "abc", "aB1", true , 2,  0 },
        { "aBc", "ab1", false, 2, -1 },
        { "aBc", "ab1", true , 2,  0 },
    };

    for (const auto& item : data)
    {
        int result = Str::Compare(item.pStr1, item.pStr2, item.ignoreCase, item.len);
        if (item.expected == 0)
        {
            REQUIRE(result == 0); //-V521
        }
        else if (item.expected < 0)
        {
            REQUIRE(result < 0); //-V521
        }
        else if (item.expected > 0)
        {
            REQUIRE(result > 0); //-V521
        }
    }
}

TEST_CASE("Util/Str::Compare(nullptr)")
{
    SECTION("nullptr, valid")
    {
        REQUIRE(Str::Compare(nullptr, "123") == INT_MIN); //-V521
    }
    SECTION("valid, nullptr")
    {
        REQUIRE(Str::Compare("123", nullptr) == INT_MAX); //-V521
    }
    SECTION("nullptr, nullptr")
    {
        REQUIRE(Str::Compare(reinterpret_cast<const char*>(nullptr), nullptr) == 0); //-V521
    }
}

TEST_CASE("Util/Str::CompareV(char)")
{
    struct
    { //-V802
        const char* pStr1;
        const char* pStr2;
        bool ignoreCase;
        size_t len;
        int expected;
    }
    static data[] =
    {
        { "abc" , "abc" , false, 0,  0 },
        { "abc" , "abcd", false, 0, -1 },
        { "abcd", "abc" , false, 0,  1 },
        { "abc" , "abc" , false, 1,  0 },
        { "abc" , "abcd", false, 1,  0 },
        { "abcd", "abc" , false, 1,  0 },
        { "abc" , "abc" , false, 2,  0 },
        { "abc" , "abcd", false, 2,  0 },
        { "abcd", "abc" , false, 2,  0 },
        { "abc" , "abc" , false, 3,  0 },
        { "abc" , "abcd", false, 3,  0 },
        { "abcd", "abc" , false, 3,  0 },
        { "abc" , "abc" , false, 4,  0 },
        { "abc" , "abcd", false, 4, -1 },
        { "abcd", "abc" , false, 4,  1 },
        { "abc" , "abc" , false, 9,  0 },
        { "abc" , "abcd", false, 9, -1 },
        { "abcd", "abc" , false, 9,  1 },

        { "abc" , "abc" , true , 0,  0 },
        { "abc" , "abC" , false, 0,  1 },
        { "abc" , "abC" , true , 0,  0 },
        { "abC" , "abc" , false, 0, -1 },
        { "abC" , "abc" , true , 0,  0 },
        { "abc" , "ab1" , false, 2,  0 },
        { "abc" , "ab1" , true , 2,  0 },
        { "abc" , "aB1" , false, 2,  1 },
        { "abc" , "aB1" , true , 2,  0 },
        { "aBc" , "ab1" , false, 2, -1 },
        { "aBc" , "ab1" , true , 2,  0 },
    };

    size_t testNum = 0;
    for (const auto& item : data)
    {
        int result = Str::CompareV(item.pStr1, item.pStr2, item.ignoreCase, item.len);
        if (item.expected == 0)
        {
            if (result != 0)
            {
                stringstream ss;
                ss << "test: " << testNum << " expected: " << item.expected << " returned: " << result;
                FAIL(ss.str().c_str());
            }
        }
        else if (item.expected < 0)
        {
            if (result >= 0)
            {
                stringstream ss;
                ss << "test: " << testNum << " expected < 0, returned >= 0";
                FAIL(ss.str().c_str());
            }
        }
        else if (item.expected > 0)
        {
            if (result <= 0)
            {
                stringstream ss;
                ss << "test: " << testNum << " expected > 0, returned <= 0";
                FAIL(ss.str().c_str());
            }

            REQUIRE(result > 0); //-V521
        }

        ++testNum;
    }
}

TEST_CASE("Util/Str::Compare(wchar_t)")
{
    struct
    { //-V802
        const wchar_t* pStr1;
        const wchar_t* pStr2;
        bool ignoreCase;
        size_t len;
        int expected;
    }
    static data[] =
    {
        { nullptr, nullptr, false, 0,  0 },
        { nullptr, nullptr, true , 0,  0 },
        { nullptr, nullptr, false, 1,  0 },
        { nullptr, nullptr, true , 1,  0 },

        { nullptr, L"abc" , false, 0, -1 },
        { nullptr, L"abc" , true , 0, -1 },
        { nullptr, L"abc" , false, 1, -1 },
        { nullptr, L"abc" , true , 1, -1 },

        { L"abc" , nullptr, false, 0,  1 },
        { L"abc" , nullptr, true , 0,  1 },
        { L"abc" , nullptr, false, 1,  1 },
        { L"abc" , nullptr, true , 1,  1 },

        { L"abc" , L"abc" , false, 0,  0 },
        { L"abc" , L"abcd", false, 0, -1 },
        { L"abcd", L"abc" , false, 0,  1 },
        { L"abc" , L"abc" , false, 1,  0 },
        { L"abc" , L"abcd", false, 1,  0 },
        { L"abcd", L"abc" , false, 1,  0 },
        { L"abc" , L"abc" , false, 2,  0 },
        { L"abc" , L"abcd", false, 2,  0 },
        { L"abcd", L"abc" , false, 2,  0 },
        { L"abc" , L"abc" , false, 3,  0 },
        { L"abc" , L"abcd", false, 3,  0 },
        { L"abcd", L"abc" , false, 3,  0 },
        { L"abc" , L"abc" , false, 4,  0 },
        { L"abc" , L"abcd", false, 4, -1 },
        { L"abcd", L"abc" , false, 4,  1 },
        { L"abc" , L"abc" , false, 9,  0 },
        { L"abc" , L"abcd", false, 9, -1 },
        { L"abcd", L"abc" , false, 9,  1 },

        { L"abc" , L"abc" , true , 0,  0 },
        { L"abc" , L"abC" , false, 0,  1 },
        { L"abc" , L"abC" , true , 0,  0 },
        { L"abC" , L"abc" , false, 0, -1 },
        { L"abC" , L"abc" , true , 0,  0 },
        { L"abc" , L"ab1" , false, 2,  0 },
        { L"abc" , L"ab1" , true , 2,  0 },
        { L"abc" , L"aB1" , false, 2,  1 },
        { L"abc" , L"aB1" , true , 2,  0 },
        { L"aBc" , L"ab1" , false, 2, -1 },
        { L"aBc" , L"ab1" , true , 2,  0 },
    };

    for (const auto& item : data)
    {
        int result = Str::Compare(item.pStr1, item.pStr2, item.ignoreCase, item.len);
        if (item.expected == 0)
        {
            REQUIRE(result == 0); //-V521
        }
        else if (item.expected < 0)
        {
            REQUIRE(result < 0); //-V521
        }
        else if (item.expected > 0)
        {
            REQUIRE(result > 0); //-V521
        }
    }
}

TEST_CASE("Util/Str::Compare(nullptr/wchar_t)")
{
    SECTION("nullptr, valid")
    {
        REQUIRE(Str::Compare(nullptr, L"123") == INT_MIN); //-V521
    }
    SECTION("valid, nullptr")
    {
        REQUIRE(Str::Compare(L"123", nullptr) == INT_MAX); //-V521
    }
    SECTION("nullptr, nullptr")
    {
        REQUIRE(Str::Compare(reinterpret_cast<const wchar_t*>(nullptr), nullptr) == 0); //-V521
    }
}

TEST_CASE("Util/Str::CompareV(wchar_t)")
{
    struct
    { //-V802
        const wchar_t* pStr1;
        const wchar_t* pStr2;
        bool ignoreCase;
        size_t len;
        int expected;
    }
    static data[] =
    {
        { L"abc" , L"abc" , false, 0,  0 },
        { L"abc" , L"abcd", false, 0, -1 },
        { L"abcd", L"abc" , false, 0,  1 },
        { L"abc" , L"abc" , false, 1,  0 },
        { L"abc" , L"abcd", false, 1,  0 },
        { L"abcd", L"abc" , false, 1,  0 },
        { L"abc" , L"abc" , false, 2,  0 },
        { L"abc" , L"abcd", false, 2,  0 },
        { L"abcd", L"abc" , false, 2,  0 },
        { L"abc" , L"abc" , false, 3,  0 },
        { L"abc" , L"abcd", false, 3,  0 },
        { L"abcd", L"abc" , false, 3,  0 },
        { L"abc" , L"abc" , false, 4,  0 },
        { L"abc" , L"abcd", false, 4, -1 },
        { L"abcd", L"abc" , false, 4,  1 },
        { L"abc" , L"abc" , false, 9,  0 },
        { L"abc" , L"abcd", false, 9, -1 },
        { L"abcd", L"abc" , false, 9,  1 },

        { L"abc" , L"abc" , true , 0,  0 },
        { L"abc" , L"abC" , false, 0,  1 },
        { L"abc" , L"abC" , true , 0,  0 },
        { L"abC" , L"abc" , false, 0, -1 },
        { L"abC" , L"abc" , true , 0,  0 },
        { L"abc" , L"ab1" , false, 2,  0 },
        { L"abc" , L"ab1" , true , 2,  0 },
        { L"abc" , L"aB1" , false, 2,  1 },
        { L"abc" , L"aB1" , true , 2,  0 },
        { L"aBc" , L"ab1" , false, 2, -1 },
        { L"aBc" , L"ab1" , true , 2,  0 },
    };

    size_t testNum = 0;
    for (const auto& item : data)
    {
        int result = Str::CompareV(item.pStr1, item.pStr2, item.ignoreCase, item.len);
        if (item.expected == 0)
        {
            if (result != 0)
            {
                stringstream ss;
                ss << "test: " << testNum << " expected: " << item.expected << " returned: " << result;
                FAIL(ss.str().c_str());
            }
        }
        else if (item.expected < 0)
        {
            if (result >= 0)
            {
                stringstream ss;
                ss << "test: " << testNum << " expected < 0, returned >= 0";
                FAIL(ss.str().c_str());
            }
        }
        else if (item.expected > 0)
        {
            if (result <= 0)
            {
                stringstream ss;
                ss << "test: " << testNum << " expected > 0, returned <= 0";
                FAIL(ss.str().c_str());
            }

            REQUIRE(result > 0); //-V521
        }

        ++testNum;
    }
}

TEST_CASE("Util/Str::NormalizeString")
{
    SECTION("nullptr")
    {
        REQUIRE(Str::NormalizeString(nullptr).empty()); //-V521
    }
    SECTION("empty")
    {
        REQUIRE(Str::NormalizeString("").empty()); //-V521
    }
    SECTION("only whitespace")
    {
        REQUIRE(Str::NormalizeString(" \t ").empty()); //-V521
    }
    SECTION("data set")
    {
        struct
        {
            const char* pStr;
            const char* pExpected;
        }
        data[] =
        {
            { "1" , "1" },
            { " 1", "1" },
            { "1 ", "1" },
            { " 1 ", "1" },
            { " 1\tb ", "1 b" },
            { "1\t \t b", "1 b" },
            { " \r\n 1\t \t b", "1 b" },
            { " \r\n 12\t \t bc ", "12 bc" }
        };

        for (auto& item : data)
        {
            string result = Str::NormalizeString(item.pStr);
            REQUIRE(result == item.pExpected); //-V521
        }
    }
}

TEST_CASE("Util/Str::NormalizeString(string_view)")
{
    struct
    {
        string_view str;
        const char* pExpected;
    }
    data[] =
    {
        { "", "" },
        { " ", "" },
        { "1" , "1" },
        { " 1", "1" },
        { "1 ", "1" },
        { " 1 ", "1" },
        { " 1\tb ", "1 b" },
        { "1\t \t b", "1 b" },
        { " \r\n 1\t \t b", "1 b" },
        { " \r\n 12\t \t bc ", "12 bc" }
    };

    for (auto& item : data)
    {
        string result = Str::NormalizeString(item.str);
        REQUIRE(result == item.pExpected); //-V521
    }
}

TEST_CASE("Util/Str::ToQuotedString")
{
    struct
    {
        const char* pStr;
        string_view expected;
    }
    data[] =
    {
        { nullptr, "<null>" },
        { ""     , "\"\""       },
        { "abc"  , "\"abc\""    },
    };

    for (auto& item : data)
    {
        auto result = Str::ToQuotedString(item.pStr);
        REQUIRE(result == item.expected); //-V521
    }
}

TEST_CASE("Util/Str::ToQuotedString2")
{
    struct
    {
        const char* pStr;
        string_view expected;
    }
    data[] =
    {
        { ""     , "\"\""       },
        { "abc"  , "\"abc\""    },
    };

    for (auto& item : data)
    {
        REQUIRE(Str::ToQuotedString2(item.pStr) == item.expected); //-V521
    }
}

TEST_CASE("Util/Str::Format")
{
    struct
    {
        string_view formatString;
        const char* pStr0;
        const char* pStr1;
        const char* pStr2;
        const char* pStr3;
        const char* pStr4;
        const char* pStr5;
        string_view expected;
    }
    data[] =
    {
        { "", "A", "B", "C", "D", "E", "F", "" },
        { "{0}{1}{2}{3}{4}{5}{6}", "A", "B", "C", "D", "E", "F", "ABCDEF{6}" },
        { "{2}{1}{4}{2}{6}", "A", "B", "C", "D", "E", "F", "CBEC{6}" },
        { "{{1}}", "A", "B", "C", "D", "E", "F", "{B}" },
        { "{{1}}", "A", nullptr, "C", "D", "E", "F", "{}" },
        { "{{1", "A", "B", "C", "D", "E", "F", "{{1" },
    };

    for (auto& item : data)
    {
        string result = Str::Format(item.formatString,
                                    item.pStr0, item.pStr1, item.pStr2, item.pStr3, item.pStr4, item.pStr5);
        REQUIRE(result == item.expected); //-V521
    }
}

TEST_CASE("Util/Str::Format3")
{
    struct
    {
        string_view formatString;
        string_view str0;
        string_view str1;
        string_view str2;
        string_view expected;
    }
    data[] =
    {
        { "", "A", "B", "C", "" },
        { "{0}{1}{2}{3}", "A", "B", "C", "ABC{3}" },
        { "{2}{1}{2}{3}", "A", "B", "C", "CBC{3}" },
        { "{{1}}", "A", "B", "C", "{B}" },
        { "{{1}}", "A", "", "C", "{}" },
        { "{{1", "A", "B", "C", "{{1" },
    };

    for (auto& item : data)
    {
        string result = Str::Format3(item.formatString, item.str0, item.str1, item.str2);
        REQUIRE(result == item.expected); //-V521
    }
}

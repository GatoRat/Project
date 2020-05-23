// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/ASCII.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("string/ASCII")
{
    //enum Type
    //{
    //    WhiteSpace = 0b00000001,
    //    Digit = 0b00000010,
    //    Upper = 0b00000100,
    //    Lower = 0b00001000,
    //    Alpha = 0b00010000,
    //    Hex   = 0b00100000
    //};

    //static Type types[256] =
    //{
    // //          0            1          2           3           4           5           6           7           8           9          10          11          12          13          14          15
    //    WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace,
    // //         16          17          18          19          20          21          22          23          24          25          26          27          28          29          30          31
    //    WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace, WhiteSpace,

    //    // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
    //};

    SECTION("IsWhitespace")
    {
        SECTION("IsTrue")
        {
            for (char ch = 1; ch <= 32; ch++) //-V112
            {
                REQUIRE(ASCII::IsWhitespace(ch) != false); //-V521
            }
        }
        SECTION("IsFalse")
        {
            for (char ch = -128; ch <= 0; ch++)
            {
                REQUIRE(ASCII::IsWhitespace(ch) == false); //-V521
            }

            for (char ch = 127; ch > 32; ch--) //-V112
            {
                REQUIRE(ASCII::IsWhitespace(ch) == false); //-V521
            }
        }
    }
    SECTION("IsDigit")
    {
        SECTION("IsTrue")
        {
            for (char ch = '0'; ch <= '9'; ch++)
            {
                REQUIRE(ASCII::IsDigit(ch) != false); //-V521
            }
        }
        SECTION("IsFalse")
        {
            for (char ch = -128; ch < '0'; ch++)
            {
                REQUIRE(ASCII::IsDigit(ch) == false); //-V521
            }

            for (char ch = 127; ch > '9'; ch--)
            {
                REQUIRE(ASCII::IsDigit(ch) == false); //-V521
            }
        }
    }
    SECTION("IsUpper")
    {
        SECTION("IsTrue")
        {
            for (char ch = 'A'; ch <= 'Z'; ch++)
            {
                REQUIRE(ASCII::IsUpper(ch) != false); //-V521
            }
        }
        SECTION("IsFalse")
        {
            for (char ch = -128; ch < 'A'; ch++)
            {
                REQUIRE(ASCII::IsUpper(ch) == false); //-V521
            }

            for (char ch = 127; ch > 'Z'; ch--)
            {
                REQUIRE(ASCII::IsUpper(ch) == false); //-V521
            }
        }
    }
    SECTION("IsLower")
    {
        SECTION("IsTrue")
        {
            for (char ch = 'a'; ch <= 'z'; ch++)
            {
                REQUIRE(ASCII::IsLower(ch) != false); //-V521
            }
        }
        SECTION("IsFalse")
        {
            for (char ch = -128; ch < 'a'; ch++)
            {
                REQUIRE(ASCII::IsLower(ch) == false); //-V521
            }

            for (char ch = 127; ch > 'z'; ch--)
            {
                REQUIRE(ASCII::IsLower(ch) == false); //-V521
            }
        }
    }
    SECTION("IsAlpha")
    {
        SECTION("IsTrue")
        {
            for (char ch = 'A'; ch <= 'Z'; ch++)
            {
                REQUIRE(ASCII::IsAlpha(ch) != false); //-V521
            }

            for (char ch = 'a'; ch <= 'z'; ch++)
            {
                REQUIRE(ASCII::IsAlpha(ch) != false); //-V521
            }
        }
        SECTION("IsFalse")
        {
            for (char ch = -128; ch < 'A'; ch++)
            {
                REQUIRE(ASCII::IsAlpha(ch) == false); //-V521
            }

            for (char ch = 'Z' + 1; ch < 'a'; ch++)
            {
                REQUIRE(ASCII::IsAlpha(ch) == false); //-V521
            }

            for (char ch = 127; ch > 'z'; ch--)
            {
                REQUIRE(ASCII::IsAlpha(ch) == false); //-V521
            }
        }
    }
    SECTION("IsAlphaNumeric")
    {
        SECTION("IsTrue")
        {
            for (char ch = '0'; ch <= '9'; ch++)
            {
                REQUIRE(ASCII::IsAlphaNumeric(ch) != false); //-V521
            }

            for (char ch = 'A'; ch <= 'Z'; ch++)
            {
                REQUIRE(ASCII::IsAlphaNumeric(ch) != false); //-V521
            }

            for (char ch = 'a'; ch <= 'z'; ch++)
            {
                REQUIRE(ASCII::IsAlphaNumeric(ch) != false); //-V521
            }
        }
        SECTION("IsFalse")
        {
            for (char ch = -128; ch < '0'; ch++)
            {
                REQUIRE(ASCII::IsAlphaNumeric(ch) == false); //-V521
            }

            for (char ch = '9' + 1; ch < 'A'; ch++)
            {
                REQUIRE(ASCII::IsAlphaNumeric(ch) == false); //-V521
            }

            for (char ch = 'Z' + 1; ch < 'a'; ch++)
            {
                REQUIRE(ASCII::IsAlphaNumeric(ch) == false); //-V521
            }

            for (char ch = 127; ch > 'z'; ch--)
            {
                REQUIRE(ASCII::IsAlphaNumeric(ch) == false); //-V521
            }
        }
    }
    SECTION("IsHex")
    {
        SECTION("IsTrue")
        {
            for (char ch = '0'; ch <= '9'; ch++)
            {
                REQUIRE(ASCII::IsHex(ch) != false); //-V521
            }

            for (char ch = 'A'; ch <= 'F'; ch++)
            {
                REQUIRE(ASCII::IsHex(ch) != false); //-V521
            }

            for (char ch = 'a'; ch <= 'f'; ch++)
            {
                REQUIRE(ASCII::IsHex(ch) != false); //-V521
            }
        }
        SECTION("IsFalse")
        {
            for (char ch = -128; ch < '0'; ch++)
            {
                REQUIRE(ASCII::IsHex(ch) == false); //-V521
            }

            for (char ch = '9' + 1; ch < 'A'; ch++)
            {
                REQUIRE(ASCII::IsHex(ch) == false); //-V521
            }

            for (char ch = 'G'; ch < 'a'; ch++)
            {
                REQUIRE(ASCII::IsHex(ch) == false); //-V521
            }

            for (char ch = 127; ch > 'f'; ch--)
            {
                REQUIRE(ASCII::IsHex(ch) == false); //-V521
            }
        }
    }
    SECTION("ToUpper")
    {
        REQUIRE(ASCII::ToUpper('1') == '1'); //-V521
        REQUIRE(ASCII::ToUpper('A') == 'A'); //-V521
        REQUIRE(ASCII::ToUpper('Z') == 'Z'); //-V521
        REQUIRE(ASCII::ToUpper('a') == 'A'); //-V521
        REQUIRE(ASCII::ToUpper('z') == 'Z'); //-V521
        REQUIRE(ASCII::ToUpper(-5)  == -5); //-V521
    }
    SECTION("ToLower")
    {
        REQUIRE(ASCII::ToLower('1') == '1'); //-V521
        REQUIRE(ASCII::ToLower('A') == 'a'); //-V521
        REQUIRE(ASCII::ToLower('Z') == 'z'); //-V521
        REQUIRE(ASCII::ToLower('a') == 'a'); //-V521
        REQUIRE(ASCII::ToLower('z') == 'z'); //-V521
        REQUIRE(ASCII::ToLower(-5) == -5); //-V521
    }
}

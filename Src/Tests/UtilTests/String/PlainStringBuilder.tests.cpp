// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/PlainStringBuilder.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("string/PlainStringBuilder::PlainStringBuilder")
{
    SECTION("ctor")
    {
        char buffer[6] = { '1', '2', '3', '4', '5', '6' };

        SECTION("char*")
        {
            PlainStringBuilder plainStringBuilder(buffer, 6);
            REQUIRE(plainStringBuilder.getMaxLen() == 5); //-V521
            REQUIRE(plainStringBuilder.getLen() == 0); //-V521
            REQUIRE(buffer[0] == 0); //-V521
        }
        SECTION("char[]")
        {
            PlainStringBuilder plainStringBuilder(buffer);
            REQUIRE(plainStringBuilder.getMaxLen() == 5); //-V521
            REQUIRE(plainStringBuilder.getLen() == 0); //-V521
            REQUIRE(buffer[0] == 0); //-V521
        }
        SECTION("nullptr, 6")
        {
            PlainStringBuilder plainStringBuilder(nullptr, 6);
            REQUIRE(plainStringBuilder.getMaxLen() == 0); //-V521
            REQUIRE(plainStringBuilder.getLen() == 0); //-V521
            REQUIRE(buffer[0] == '1'); //-V521
        }
        SECTION("char*, 0")
        {
            PlainStringBuilder plainStringBuilder(buffer, 0);
            REQUIRE(plainStringBuilder.getMaxLen() == 0); //-V521
            REQUIRE(plainStringBuilder.getLen() == 0); //-V521
            REQUIRE(buffer[0] == '1'); //-V521
        }
    }
    //SECTION("int[]")
    //{
    //    int intBuffer[6] = { 1, 2, 3, 4, 5, 6 };
    //    PlainStringBuilder plainStringBuilder(intBuffer);
    //}

    SECTION("append (1)")
    {
        char buffer[6] = { '1', '2', '3', '4', '5', '6' };
        PlainStringBuilder plainStringBuilder(buffer);

        //    struct
        //    {
        //        const char* pStr;
        //        struct
        //        {
        //            bool rval;
        //            int32_t result;
        //        } expected;
        //    }
        //    static data[] =
        //    {
        //        { "2147483647",   { true , 2147483647  }},
        //        { " 2147483647",  { true , 2147483647  }},
        //        { "-2147483248",  { true , -2147483248 }},
        //        { " -2147483248", { true , -2147483248 }},
        //        { " 0",           { true , 0           }},
        //        { nullptr,        { false, 0           }}
        //    };
        //
        //    for (const auto item : data)
        //    {
        //        int32_t result;
        //        REQUIRE(Convert::ToInt32(item.pStr, result) == item.expected.rval);
        //        REQUIRE(result == item.expected.result);
        //    }
        SECTION("char[]")
        {
            constexpr char data[] = "A2";

            REQUIRE(strcmp(plainStringBuilder.append(data), "A2") == 0); //-V521
        }
        SECTION("uint8_t[]")
        {
            constexpr uint8_t data[] = { 'A', '2' };

            REQUIRE(strcmp(plainStringBuilder.append(data), "A2") == 0); //-V521
        }
    }
    SECTION("setLen")
    {
        char buffer[256];
        PlainStringBuilder psb(buffer);
        psb += "1234";
        psb.setLen(2);
        REQUIRE(string_view(psb, psb.getLen()) == "12"); //-V521
    }
    SECTION("append")
    {
        char buffer[256];
        PlainStringBuilder psb(buffer);

        SECTION("char*, int")
        {
            constexpr char* pData = "12345";
            psb.append(pData, 3);
            REQUIRE(psb.getLen() == 3); //-V521
            string s(psb.str());
            REQUIRE(s == "123"); //-V521
        }
        SECTION("char*")
        {
            constexpr char* pData = "12345";
            psb += pData;
            REQUIRE(psb.getLen() == 5); //-V521
            REQUIRE(string_view(psb, psb.getLen()) == "12345"); //-V521
        }
    }
}

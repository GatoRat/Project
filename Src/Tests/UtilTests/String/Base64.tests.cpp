// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/Base64.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("string/Base64")
{
    SECTION("Base64Encode")
    {
        string result = Base64Encode("1234567", 7);
        REQUIRE(result == "MTIzNDU2Nw=="); //-V521

        result = Base64Encode("1234567", 7, 8);
        REQUIRE(result == "MTIzNDU2\r\nNw=="); //-V521
    }
    SECTION("Base64Decode")
    {
        auto result = Base64Decode("MTIzNDU2Nw==");
        REQUIRE(memcmp(result.data(), "1234567", 7) == 0); //-V521

        result = Base64Decode("MTIzNDU\r\n2Nw==");
        REQUIRE(memcmp(result.data(), "1234567", 7) == 0); //-V521
    }
}
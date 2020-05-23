// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Misc/ByteOrder.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Misc/ByteOrder")
{
    SECTION("ChangeEndianness16")
    {
        REQUIRE(ChangeEndianness16(0x1234) == 0x3412); //-V521
    }
    SECTION("ChangeEndianness32")
    {
        REQUIRE(ChangeEndianness32(0x12345678) == 0x78563412); //-V521
    }
    SECTION("ChangeEndianness64")
    {
        REQUIRE(ChangeEndianness64(0x0102030405060708) == 0x0807060504030201); //-V521
    }
}

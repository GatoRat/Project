// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/Crc32.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Secure/Crc32")
{
    SECTION("nullptr")
    {
        REQUIRE(Crc32(nullptr, 0) == 0); //-V521
        REQUIRE(Crc32(nullptr, 0) == 0); //-V521
    }
    SECTION("cpp")
    {
        REQUIRE(Crc32("123456789") == 0xCBF43926); //-V521
    }
}

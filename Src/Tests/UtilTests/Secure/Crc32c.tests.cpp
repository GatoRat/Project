// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/Crc32c.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Secure/Crc32c")
{
    SECTION("nullptr")
    {
        REQUIRE(Crc32c(nullptr, false) == 0); //-V521
        REQUIRE(Crc32c(nullptr, true) == 0); //-V521
    }
    SECTION("cpp")
    {
        REQUIRE(Crc32c("123456789", true) == 0xe3069283); //-V521
    }
    SECTION("SSE42")
    {
        REQUIRE(Crc32c("123456789", false) == 0xe3069283); //-V521
    }
}

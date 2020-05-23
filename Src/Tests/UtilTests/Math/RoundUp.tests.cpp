// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Math/RoundUp.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Math/RoundUp")
{
    SECTION("uint32_t")
    {
        REQUIRE(RoundUp(static_cast<uint32_t>(0), 5) == 5); //-V521
        REQUIRE(RoundUp(static_cast<uint32_t>(3), 5) == 5); //-V521
        REQUIRE(RoundUp(static_cast<uint32_t>(5), 5) == 5); //-V521
        REQUIRE(RoundUp(static_cast<uint32_t>(8), 5) == 10); //-V521
        REQUIRE(RoundUp(static_cast<uint32_t>(15), 5) == 15); //-V521
    }
    SECTION("uint64_t")
    {
        REQUIRE(RoundUp(static_cast<uint64_t>(0), 5) == 5); //-V521
        REQUIRE(RoundUp(static_cast<uint64_t>(3), 5) == 5); //-V521
        REQUIRE(RoundUp(static_cast<uint64_t>(5), 5) == 5); //-V521
        REQUIRE(RoundUp(static_cast<uint64_t>(8), 5) == 10); //-V521
        REQUIRE(RoundUp(static_cast<uint64_t>(15), 5) == 15); //-V521
    }
}

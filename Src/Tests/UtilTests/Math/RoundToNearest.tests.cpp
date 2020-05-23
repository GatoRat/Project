// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Math/RoundToNearest.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Math/RoundToNearest")
{
    SECTION("float")
    {
        REQUIRE(RoundToNearest(0.0f, 0.0f) == 0.0f); //-V550 //-V521
        REQUIRE(RoundToNearest(1.0f, 0.0f) == 0.0f); //-V550 //-V521
        REQUIRE(RoundToNearest(0.0f, 0.5f) == 0.0f); //-V550 //-V521
        REQUIRE(RoundToNearest(0.4f, 0.5f) == 0.5f); //-V550 //-V521
        REQUIRE(RoundToNearest(9.2f, 0.3f) == 9.3f); //-V550 //-V521
    }
}

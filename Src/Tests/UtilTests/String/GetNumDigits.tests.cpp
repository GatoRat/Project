// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/GetNumDigits.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("String/GetNumDigits")
{
    REQUIRE(GetNumDigits(0) == 1); //-V521
    REQUIRE(GetNumDigits(1) == 1); //-V521
    REQUIRE(GetNumDigits(10) == 2); //-V521
    REQUIRE(GetNumDigits(0xffffffff) == 10); //-V112 //-V521
    REQUIRE(GetNumDigits(0xffffffffffffffff) == 20); //-V521
}

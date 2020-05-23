// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Error/ErrString.h"
#include "TestHelper/catch.hpp"
#include <sstream>

using namespace std;

TEST_CASE("Misc/ErrString")
{
    SECTION("GetErrStr")
    {
        REQUIRE(GetErrStr(0).has_value()); //-V521
    }
    SECTION("GetErrStrErrno")
    {
        REQUIRE(GetErrStrErrno(0).has_value()); //-V521
    }
    SECTION("GetErrStrOS")
    {
        REQUIRE(GetErrStrOS(0).has_value()); //-V521
    }
}

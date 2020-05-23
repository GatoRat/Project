// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Misc/MakeFourCC.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Misc/MakeFourCC")
{
    REQUIRE(MakeFourCC('1', '2', '3', '4') == 0x34333231); //-V521
}

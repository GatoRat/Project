// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Misc/countof.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Misc/countof")
{
    SECTION("array")
    {
        int data[3] = { 2, 4, 6 }; //-V808
        REQUIRE(_countof(data) == 3); //-V521
    }
    SECTION("structs")
    {
        struct
        {
            int x;
        }
        data[] = //-V808
        {
            { 1 },
            { 2 },
            { 3 },
            { 4 }
        };

        REQUIRE(_countof(data) == 4); //-V112 //-V521
    }
}

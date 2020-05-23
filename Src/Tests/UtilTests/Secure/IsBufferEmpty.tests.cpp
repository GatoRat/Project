// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Misc/IsBufferEmpty.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Misc/IsBufferEmpty")
{
    struct
    {
        const char* pData;
        bool expected;
    }
    data[] =
    {
        { "\0\0\0\0\0", true  },
        { " \0\0\0\0" , false },
        { "\0\0 \0\0" , false },
        { "\0\0\0 \0" , false },
        { "\0\0\0\0 " , false },
        { "     "     , false },
    };

    for (const auto& item : data)
    {
        REQUIRE(IsBufferEmpty(item.pData, 5) == item.expected); //-V521
    }
}
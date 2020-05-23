// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Math/Random.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Misc/Random")
{
    SECTION("Seed")
    {
        RandSeed();
    }
    SECTION("RandBetween(float)")
    {
        float result = RandBetween(1.0f, 2.0f);
        REQUIRE((result >= 1.0f && result < 2.0f)); //-V521
    }
    SECTION("RandBetween(int)")
    {
        int result = RandBetween(-2, 2);
        REQUIRE((result >= -2 && result <= 2)); //-V521
    }
    SECTION("RandBetween(uint32_t)")
    {
        uint32_t result = RandBetweenUint32(UINT32_MAX - 2, UINT32_MAX);
        REQUIRE(result >= (UINT32_MAX - 2)); //-V521
    }
    SECTION("FisherYatesShuffle(uint32_t)")
    {
        for (int i = 0; i < 100; ++i)
        {
            int data[5]{ 0, 1, 2, 3, 4 };
            FisherYatesShuffle(data, 5);
            bool check[5]{ false, false, false, false, false };
            for (int val : data)
            {
                check[val] = true;
            }

            for (bool val : check)
            {
                REQUIRE(val); //-V521
            }
        }
    }
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Math/Trig.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Math/Trig")
{
    SECTION("DegreesToRadians")
    {
        struct
        {
            float degrees;
            float expected;
        }
        data[] = //-V808
        {
            {    0.0f,  0.0f        },
            {   90.0f,  (PI / 2.0f) },
            {  180.0f,  PI          },
            {  360.0f,  (PI * 2.0f) },
            {  -90.0f, -(PI / 2.0f) },
            { -180.0f, -PI          },
            { -360.0f, -(PI * 2.0f) }
        };

        for (const auto& item : data)
        {
            REQUIRE(DegreesToRadians(item.degrees) == Approx(item.expected)); //-V550 //-V521
        }
    }
    SECTION("RadiansToDegrees")
    {
        struct
        {
            float radians;
            float expected;
        }
        data[] = //-V808
        {
            {  0.0f       ,    0.0f },
            {  (PI / 2.0f),   90.0f },
            {  PI         ,  180.0f },
            {  (PI * 2.0f),  360.0f },
            { -(PI / 2.0f),  -90.0f },
            { -PI         , -180.0f },
            { -(PI * 2.0f), -360.0f }
        };

        for (const auto& item : data)
        {
            REQUIRE(RadiansToDegrees(item.radians) == Approx(item.expected)); //-V550 //-V521
        }
    }
}

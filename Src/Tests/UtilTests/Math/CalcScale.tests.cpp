// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Math/CalcScale.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Math/CalcScale")
{
    struct
    {
        float srcWidth;
        float srcHeight;
        float targetWidth;
        float targetHeight;
        float expected;
    }
    data[] = //-V808
    {
        { 10.0f, 10.0f,  5.0f,  5.0f, 0.50f },
        { 20.0f, 10.0f,  5.0f, 10.0f, 0.25f },
        { 10.0f, 20.0f, 10.0f,  5.0f, 0.25f },
        { 10.0f, 20.0f, 10.0f,  0.0f, 1.0f  },
        { 10.0f, 20.0f,  0.0f,  5.0f, 1.0f  },
        { 10.0f, 20.0f,  0.0f,  0.0f, 1.0f  },
        { 10.0f,  0.0f, 10.0f, 20.0f, 0.0f  },
        {  0.0f,  5.0f, 10.0f, 20.0f, 0.0f  },
        {  0.0f,  0.0f, 10.0f, 20.0f, 0.0f  },
        {  0.0f,  0.0f,  0.0f,  0.0f, 0.0f  },
        { 99.0f, 99.0f,  0.01f,  0.01f, CalcScaleMin },
    };

    for (const auto& item : data)
    {
        REQUIRE(CalcScale(item.srcWidth, item.srcHeight, item.targetWidth, item.targetHeight) == Approx(item.expected)); //-V550 //-V521
    }
}

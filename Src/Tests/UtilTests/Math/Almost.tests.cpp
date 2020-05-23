// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Math/Almost.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Math/Almost")
{
    SECTION("Equal")
    {
        struct
        {
            float val1;
            float val2;
            float tolerance;
            bool  expected;
        }
        data[] = //-V808
        {
            { 1.0f   ,  1.0f   , Almost::Tolerance, true  },
            { 1.1f   ,  1.1f   , Almost::Tolerance, true  }, // 1.1f is expressed as ~1.10000002
            { 0.9999f,  1.0000f, Almost::Tolerance, true  }, // 0.9999f is expressed as ~0.999899983
            { 1.0001f,  1.0000f, Almost::Tolerance, true  },
            { 0.9f   ,  1.0f   , Almost::Tolerance, false }, // 0.9f is expressed as ~0.899999976
            { 1.1f   ,  1.0f   , Almost::Tolerance, false },
        };

        for (const auto& item : data)
        {
            REQUIRE(Almost::Equal(item.val1, item.val2, item.tolerance) == item.expected); //-V521
        }
    }
    SECTION("Zero")
    {
        struct
        {
            float val;
            float tolerance;
            bool  expected;
        }
        data[] = //-V808
        {
            { 0.0f   , Almost::Tolerance, true  },
            { 0.0001f, Almost::Tolerance, true  },
            {-0.0001f, Almost::Tolerance, true  },
            { 1.002f , Almost::Tolerance, false }, // 1.002 is expressed as ~1.00199997
            {-1.002f , Almost::Tolerance, false },
        };

        for (const auto& item : data)
        {
            REQUIRE(Almost::Zero(item.val, item.tolerance) == item.expected); //-V521
        }
    }
    SECTION("EqualGetSecond")
    {
        struct
        {
            float val;
            float targetVal;
            float tolerance;
            float expected;
        }
        data[] = //-V808
        {
            { 1.0f   ,  1.0f, Almost::Tolerance, 1.0f },
            { 0.9999f,  1.0f, Almost::Tolerance, 1.0f }, // 0.9999f is expressed as ~0.999899983
            { 1.0001f,  1.0f, Almost::Tolerance, 1.0f },
            { 0.9f   ,  1.0f, Almost::Tolerance, 0.9f }, // 0.9f is expressed as ~0.899999976
            { 1.1f   ,  1.0f, Almost::Tolerance, 1.1f },
        };

        for (const auto& item : data)
        {
            REQUIRE(Almost::EqualGetSecond(item.val, item.targetVal, item.tolerance) == Approx(item.expected)); //-V550 //-V521
        }
    }
    SECTION("ZeroGetZero")
    {
        struct
        {
            float val;
            float tolerance;
            float expected;
        }
        data[] = //-V808
        {
            { 0.0f   , Almost::Tolerance,  0.0f   },
            { 0.0001f, Almost::Tolerance,  0.0f   },
            {-0.0001f, Almost::Tolerance,  0.0f   },
            { 1.002f , Almost::Tolerance,  1.002f },
        };

        for (const auto& item : data)
        {
            REQUIRE(Almost::ZeroGetZero(item.val, item.tolerance) == item.expected); //-V521
        }
    }
    SECTION("ForceToRange")
    {
        struct
        {
            float val;
            float minVal;
            float maxVal;
            float expected;
        }
        data[] = //-V808
        {
            {  0.0f   , 0.0f, 1.0f,  0.0f    },
            {  0.0001f, 0.0f, 1.0f,  0.0001f },
            { -0.0001f, 0.0f, 1.0f,  0.0f    },
            {  1.002f , 0.0f, 1.0f,  1.0f    },
            { -1.002f , 0.0f, 1.0f,  0.0f    },
        };

        for (const auto& item : data)
        {
            REQUIRE(ForceToRange(item.val, item.minVal, item.maxVal) == Approx(item.expected)); //-V550 //-V521
        }
    }
}

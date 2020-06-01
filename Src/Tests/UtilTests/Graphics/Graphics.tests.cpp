// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Graphics/Graphics.h"
#include "TestHelper/catch.hpp"

using namespace std;
using namespace Graphics;

TEST_CASE("Graphics")
{
    SECTION("Rect")
    {
        SECTION("ctor")
        {
            Rect rect;
            REQUIRE(rect.left == 0); //-V521
            REQUIRE(rect.top == 0); //-V521
            REQUIRE(rect.width == 0); //-V521
            REQUIRE(rect.height == 0); //-V521
        }
        SECTION("ctor(left, top, width, height)")
        {
            Rect rect(1, 2, 3, 4); //-V112
            REQUIRE(rect.left == 1); //-V521
            REQUIRE(rect.top == 2); //-V521
            REQUIRE(rect.width == 3); //-V521
            REQUIRE(rect.height == 4); //-V112 //-V521
        }
        SECTION("ctor(pos, size)")
        {
            Rect rect(Position(5, 6), Size(7, 8));
            REQUIRE(rect.left == 5); //-V521
            REQUIRE(rect.top == 6); //-V521
            REQUIRE(rect.width == 7); //-V521
            REQUIRE(rect.height == 8); //-V521
        }
        SECTION("getPosition")
        {
            Rect rect(1, 2, 5, 7);
            SECTION("TopLeft")
            {
                Position pos = rect.get(Rect::TopLeft);
                REQUIRE(pos.x == 1); //-V521
                REQUIRE(pos.y == 2); //-V521
            }
            SECTION("TopRight")
            {
                Position pos = rect.get(Rect::TopRight);
                REQUIRE(pos.x == 6); //-V521
                REQUIRE(pos.y == 2); //-V521
            }
            SECTION("BottomLeft")
            {
                Position pos = rect.get(Rect::BottomLeft);
                REQUIRE(pos.x == 1); //-V521
                REQUIRE(pos.y == 9); //-V521
            }
            SECTION("BottomRight")
            {
                Position pos = rect.get(Rect::BottomRight);
                REQUIRE(pos.x == 6); //-V521
                REQUIRE(pos.y == 9); //-V521
            }
        }
        SECTION("operator Position")
        {
            Rect rect(1, 2, 3, 4); //-V112
            Position pos(rect);
            REQUIRE(pos.x == 1); //-V521
            REQUIRE(pos.y == 2); //-V521
        }
        SECTION("isEmpty")
        {
            struct
            {
                Rect rect;
                bool expected;
            }
            static testData[]
            {
                { { 0, 0, 0, 0 }, true  },
                { { 1, 2, 0, 4 }, true  },
                { { 1, 2, 3, 0 }, true  },
                { { 1, 2, 3, 4 }, false }
            };

            for (const auto& test : testData)
            {
                REQUIRE(test.rect.isEmpty() == test.expected); //-V521
            }
        }
        SECTION("equality")
        {
            struct
            {
                Rect rect1;
                Rect rect2;
                bool expected;
            }
            static testData[]
            {
                { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, true },
                { { 1, 2, 0, 4 }, { 1, 2, 0, 4 }, true },
                { { 1, 2, 3, 0 }, { 1, 2, 3, 0 }, true },
                { { 1, 2, 3, 4 }, { 1, 2, 3, 4 }, true },
                { { 0, 1, 0, 0 }, { 1, 0, 0, 0 }, false },
                { { 0, 0, 1, 0 }, { 0, 0, 0, 1 }, false }
            };

            for (const auto& test : testData)
            {
                bool result = test.rect1 == test.rect2;
                REQUIRE(result == test.expected); //-V521
            }
        }
    }
    SECTION("RectF")
    {
        SECTION("ctor")
        {
            RectF rect;
            REQUIRE(rect.left == Approx(0.0f)); //-V550 //-V521
            REQUIRE(rect.top == Approx(0.0f)); //-V550 //-V521
            REQUIRE(rect.width == Approx(0.0f)); //-V550 //-V521
            REQUIRE(rect.height == Approx(0.0f)); //-V550 //-V521
        }
        SECTION("ctor(left, top, width, height)")
        {
            RectF rect(1.0f, 2.0f, 3.0f, 4.0f);
            REQUIRE(rect.left == Approx(1.0f)); //-V550 //-V521
            REQUIRE(rect.top == Approx(2.0f)); //-V550 //-V521
            REQUIRE(rect.width == Approx(3.0f)); //-V550 //-V521
            REQUIRE(rect.height == Approx(4.0f)); //-V550 //-V521
        }
        SECTION("ctor(pos, size)")
        {
            RectF rect(PositionF(5.0f, 6.0f), SizeF(7.0f, 8.0f));
            REQUIRE(rect.left == Approx(5.0f)); //-V550 //-V521
            REQUIRE(rect.top == Approx(6.0f)); //-V550 //-V521
            REQUIRE(rect.width == Approx(7.0f)); //-V550 //-V521
            REQUIRE(rect.height == Approx(8.0f)); //-V550 //-V521
        }
        SECTION("getPosition")
        {
            RectF rect(1.0f, 2.0f, 5.0f, 7.0f);
            SECTION("TopLeft")
            {
                PositionF pos = rect.get(Rect::TopLeft);
                REQUIRE(pos.x == Approx(1.0f)); //-V550 //-V521
                REQUIRE(pos.y == Approx(2.0f)); //-V550 //-V521
            }
            SECTION("TopRight")
            {
                PositionF pos = rect.get(Rect::TopRight);
                REQUIRE(pos.x == Approx(6.0f)); //-V550 //-V521
                REQUIRE(pos.y == Approx(2.0f)); //-V550 //-V521
            }
            SECTION("BottomLeft")
            {
                PositionF pos = rect.get(Rect::BottomLeft);
                REQUIRE(pos.x == Approx(1.0f)); //-V550 //-V521
                REQUIRE(pos.y == Approx(9.0f)); //-V550 //-V521
            }
            SECTION("BottomRight")
            {
                PositionF pos = rect.get(Rect::BottomRight);
                REQUIRE(pos.x == Approx(6.0f)); //-V550 //-V521
                REQUIRE(pos.y == Approx(9.0f)); //-V550 //-V521
            }
        }
        SECTION("operator Position")
        {
            RectF rect(1.0f, 2.0f, 3.0f, 4.0f);
            PositionF pos(rect);
            REQUIRE(pos.x == Approx(1.0f)); //-V550 //-V521
            REQUIRE(pos.y == Approx(2.0f)); //-V550 //-V521
        }
        SECTION("isEmpty")
        {
            struct
            {
                RectF rect;
                bool expected;
            }
            static testData[]
            {
                { { 0.0f, 0.0f, 0.0f, 0.0f }, true },
                { { 1.0f, 2.0f, 0.0f, 4.0f }, true },
                { { 1.0f, 2.0f, 3.0f, 0.0f }, true },
                { { 1.0f, 2.0f, 3.0f, 4.0f }, false }
            };

            for (const auto& test : testData)
            {
                REQUIRE(test.rect.isEmpty() == test.expected); //-V521
            }
        }
        SECTION("equality")
        {
            struct
            {
                RectF rect1;
                RectF rect2;
                bool expected;
            }
            static testData[]
            {
                { { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, true },
                { { 1.0f, 2.0f, 0.0f, 4.0f }, { 1.0f, 2.0f, 0.0f, 4.0f }, true },
                { { 1.0f, 2.0f, 3.0f, 0.0f }, { 1.0f, 2.0f, 3.0f, 0.0f }, true },
                { { 1.0f, 2.0f, 3.0f, 4.0f }, { 1.0f, 2.0f, 3.0f, 4.0f }, true },
                { { 0.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 0.0f }, false },
                { { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, false }
            };

            for (const auto& test : testData)
            {
                bool result = test.rect1 == test.rect2;
                REQUIRE(result == test.expected); //-V521
            }
        }
    }
}

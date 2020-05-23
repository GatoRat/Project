// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/SplitText.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Util/SplitText")
{
    SECTION("Simple")
    {
        string line1;
        string line2;
        SplitText("12345678", line1, line2);
        REQUIRE(line1 == "12345678"); //-V521
        REQUIRE(line2.empty()); //-V521
    }
    SECTION("Middle")
    {
        string line1;
        string line2;
        SplitText("1234 5678", line1, line2);
        REQUIRE(line1 == "1234"); //-V521
        REQUIRE(line2 == "5678"); //-V521
    }
    SECTION("Left")
    {
        string line1;
        string line2;
        SplitText("123 45678", line1, line2);
        REQUIRE(line1 == "123"); //-V521
        REQUIRE(line2 == "45678"); //-V521
    }
    SECTION("Right")
    {
        string line1;
        string line2;
        SplitText("12345 678", line1, line2);
        REQUIRE(line1 == "12345"); //-V521
        REQUIRE(line2 == "678"); //-V521
    }
    SECTION("Tricky")
    {
        string line1;
        string line2;
        SplitText("1 23 45 6 78", line1, line2);
        REQUIRE(line1 == "1 23 45"); //-V521
        REQUIRE(line2 == "6 78"); //-V521
    }
}

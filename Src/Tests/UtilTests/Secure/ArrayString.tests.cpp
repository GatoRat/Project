// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/ArrayString.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Secure/ArrayString")
{
    SECTION("ctor")
    {
        ArrayString<8> buffer;
        REQUIRE(buffer.size() == 0); //-V521
    }
    SECTION("ctor(const char*)")
    {
        ArrayString<8> buffer("123");
        REQUIRE(buffer.size() == 3); //-V521
        REQUIRE(buffer.getView() == "123"); //-V521
    }
    SECTION("ctor(const char*, size_t)")
    {
        ArrayString<8> buffer("1234", 3);
        REQUIRE(buffer.size() == 3); //-V521
        REQUIRE(buffer.getView() == "123"); //-V521
    }
    SECTION("cat(const char*, size_t)")
    {
        ArrayString<8> buffer("12");
        REQUIRE(buffer.cat("cdef", 3) == string_view("12cde")); //-V521
        REQUIRE(buffer.size() == 5); //-V521
        REQUIRE(buffer.getView() == "12cde"); //-V521
    }
    SECTION("insertAt(size_t, const char*, size_t)")
    {
        ArrayString<8> buffer("1234");
        REQUIRE(buffer.insertAt(2, "cdef", 3) == string_view("12cde")); //-V521
        REQUIRE(buffer.size() == 5); //-V521
        REQUIRE(buffer.getView() == "12cde"); //-V521
    }
    SECTION("insertAt(<too big>, const char*, size_t)")
    {
        ArrayString<8> buffer("1234");
        REQUIRE(buffer.insertAt(6, "cdef", 3) == string_view("1234")); //-V521
        REQUIRE(buffer.size() == 4); //-V112 //-V521
        REQUIRE(buffer.getView() == "1234"); //-V521
    }
/*
    SECTION("copy at")
    {
        ArrayString<4> buffer("1234", 8); //-V112
        REQUIRE(buffer.size() == 4); //-V112

        buffer.copyAt(2, "abc", 3);

        REQUIRE(static_cast<char>(buffer[0]) == '1');
        REQUIRE(static_cast<char>(buffer[1]) == '2');
        REQUIRE(static_cast<char>(buffer[2]) == 'a');
        REQUIRE(static_cast<char>(buffer[3]) == 'b');
    }
    SECTION("erase")
    {
        uint8_t data[] = { 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xa5, 0xa6 };
        ArrayString<8> buffer(data, sizeof(data));

        REQUIRE(buffer.size() == 8);

        buffer.erase();

        for (size_t i = 0; i < buffer.size(); ++i)
        {
            REQUIRE(buffer[i] == 0);
        }
    }
    SECTION("erase(val)")
    {
        uint8_t data[] = { 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xa5, 0xa6 };
        ArrayString<8> buffer(data, sizeof(data));

        REQUIRE(buffer.size() == 8);

        buffer.erase(0xa5);

        for (size_t i = 0; i < buffer.size(); ++i)
        {
            REQUIRE(buffer[i] == 0xa5);
        }
    }
*/
}

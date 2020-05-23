// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/SecureBuffer.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Secure/SecureArray")
{
    SECTION("ctor")
    {
        SecureBuffer<8> buffer;

        REQUIRE(buffer.size() == 8); //-V521

        for (size_t i = 0; i < buffer.size(); ++i)
        {
            REQUIRE(buffer[i] == 0); //-V521
        }
    }
    SECTION("ctor fill")
    {
        SecureBuffer<8> buffer(0x5a);

        REQUIRE(buffer.size() == 8); //-V521

        for (size_t i = 0; i < buffer.size(); ++i)
        {
            REQUIRE(buffer[i] == 0x5a); //-V521
        }
    }
    SECTION("ctor copy")
    {
        uint8_t data[] = { 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xa5, 0xa6 };
        SecureBuffer<8> buffer(data, sizeof(data));

        REQUIRE(buffer.size() == 8); //-V521

        uint8_t val = 0x53;
        for (size_t i = 0; i < buffer.size(); ++i, ++val)
        {
            REQUIRE(buffer[i] == val); //-V521
        }
    }
    SECTION("copy at")
    {
        SecureBuffer<4> buffer("1234", 8); //-V112
        REQUIRE(buffer.size() == 4); //-V112 //-V521

        buffer.copyAt(2, "abc", 3);

        REQUIRE(static_cast<char>(buffer[0]) == '1'); //-V521
        REQUIRE(static_cast<char>(buffer[1]) == '2'); //-V521
        REQUIRE(static_cast<char>(buffer[2]) == 'a'); //-V521
        REQUIRE(static_cast<char>(buffer[3]) == 'b'); //-V521
    }
    SECTION("erase")
    {
        uint8_t data[] = { 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xa5, 0xa6 };
        SecureBuffer<8> buffer(data, sizeof(data));

        REQUIRE(buffer.size() == 8); //-V521

        buffer.erase();

        for (size_t i = 0; i < buffer.size(); ++i)
        {
            REQUIRE(buffer[i] == 0); //-V521
        }
    }
    SECTION("erase(val)")
    {
        uint8_t data[] = { 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xa5, 0xa6 };
        SecureBuffer<8> buffer(data, sizeof(data));

        REQUIRE(buffer.size() == 8); //-V521

        buffer.erase(0xa5);

        for (size_t i = 0; i < buffer.size(); ++i)
        {
            REQUIRE(buffer[i] == 0xa5); //-V521
        }
    }
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/AES_GCM.h"
#include "TestHelper/catch.hpp"
#include "Util/Error/Xception.h"

using namespace std;

TEST_CASE("Secure/AES_GCM")
{
    static uint8_t key[]
    {
         1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
        31, 32
    };

    static uint8_t iv[]
    {
        101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
        111, 112
    };

    static uint8_t data[]
    {
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57
    };

    SECTION("success")
    {
        uint8_t buffer[10];
        uint8_t tag[16];

        int result = AES_GCM::encrypt(key, iv, data, buffer, sizeof(data), tag);
        REQUIRE(result == 10); //-V521

        uint8_t outbuffer[10];
        result = AES_GCM::decrypt(key, iv, buffer, outbuffer, sizeof(outbuffer), tag);
        REQUIRE(result == 10); //-V521

        REQUIRE(memcmp(outbuffer, data, 10) == 0); //-V521
    }
    SECTION("success (additional data)")
    {
        uint8_t buffer[10];
        uint8_t tag[16];
        uint8_t additionalData[4]{ 200, 201, 202, 203 }; //-V112

        int result = AES_GCM::encrypt(key, iv, data, buffer, sizeof(data), tag, additionalData, sizeof(additionalData));
        REQUIRE(result == 10); //-V521

        uint8_t outbuffer[10];
        result = AES_GCM::decrypt(key, iv, buffer, outbuffer, sizeof(outbuffer), tag, additionalData, sizeof(additionalData));
        REQUIRE(result == 10); //-V521

        REQUIRE(memcmp(outbuffer, data, 10) == 0); //-V521
    }
}

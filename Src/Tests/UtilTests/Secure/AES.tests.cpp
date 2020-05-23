// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/AES.h"
#include "TestHelper/catch.hpp"
#include "Util/Error/Xception.h"

using namespace std;

TEST_CASE("Secure/AES")
{
    static const char* pKey = "12345678901234567890123456789012";

    SECTION("ctor")
    {
        try
        {
            AES aes(nullptr, 16);
            FAIL("An exception should have been thrown.");
        }
        catch (Xception&)
        { //-V565
        }

        try
        {
            AES aes(nullptr, 0);
            FAIL("An exception should have been thrown.");
        }
        catch (Xception&)
        { //-V565
        }

        try
        {
            AES aes(pKey, 31);
            FAIL("An exception should have been thrown.");
        }
        catch (Xception&)
        { //-V565
        }

        try
        {
            AES aes(pKey, 32); //-V112
        }
        catch (Xception&)
        {
            FAIL("An exception should NOT have been thrown.");
        }
    }
    SECTION("Bad params")
    {
        AES aes(pKey, strlen(pKey));

        uint8_t buffer[16];

        REQUIRE_FALSE(aes.encrypt(nullptr, buffer, sizeof(buffer))); //-V521
        REQUIRE_FALSE(aes.encrypt(buffer, nullptr, sizeof(buffer))); //-V521
        REQUIRE_FALSE(aes.encrypt(buffer, buffer, sizeof(buffer) - 1)); //-V521
        REQUIRE_FALSE(aes.encrypt(buffer, buffer, sizeof(buffer) - 1)); //-V521

        REQUIRE_FALSE(aes.decrypt(nullptr, buffer, sizeof(buffer))); //-V521
        REQUIRE_FALSE(aes.decrypt(buffer, nullptr, sizeof(buffer))); //-V521
        REQUIRE_FALSE(aes.decrypt(buffer, buffer, sizeof(buffer) - 1)); //-V521
        REQUIRE_FALSE(aes.decrypt(buffer, buffer, sizeof(buffer) - 1)); //-V521

    }
    SECTION("RoundTrip")
    {
        const char* pData = "heather heather ";
        uint8_t output[16];
        memset(output, 0, sizeof(output));

        SECTION("encrypt/decrypt separate")
        {
            uint8_t buffer[16];
            {
                AES aes(pKey, strlen(pKey), true);
                REQUIRE(aes.encrypt(pData, buffer, sizeof(buffer))); //-V521
            }
            {
                AES aes(pKey, strlen(pKey));
                REQUIRE(aes.decrypt(buffer, output, sizeof(buffer))); //-V521
            }
        }
        SECTION("encrypt/decrypt combined")
        {
            AES aes(pKey, strlen(pKey));
            uint8_t buffer[16];
            REQUIRE(aes.encrypt(pData, buffer, sizeof(buffer))); //-V521
            REQUIRE(aes.decrypt(buffer, output, sizeof(buffer))); //-V521
        }
        SECTION("encrypt/decrypt same buffer")
        {
            AES aes(pKey, strlen(pKey));

            memcpy(output, pData, sizeof(output));
            REQUIRE(aes.encrypt(output, output, sizeof(output))); //-V521
            REQUIRE(aes.decrypt(output, output, sizeof(output))); //-V521
        }

        REQUIRE(memcmp(pData, output, sizeof(output)) == 0); //-V521
    }
}

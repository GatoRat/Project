// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/CryptGenRandom.h"
#include "TestHelper/catch.hpp"
#include "Util/Error/Xception.h"
#include "Util/Secure/CryptGenRandom.OpenSSL.h"
#include "Util/Secure/CryptGenRandom.Win32.h"

using namespace std;

TEST_CASE("Secure/CryptGenRandom")
{
    SECTION("Win32")
    {
        uint8_t buffer[4]{ 0, 0, 0, 0 }; //-V112
        REQUIRE(CryptGenRandom_Win32(buffer, sizeof(buffer))); //-V521
    }
    SECTION("OpenSSL")
    {
        uint8_t buffer[4]{ 0, 0, 0, 0 }; //-V112
        REQUIRE(CryptGenRandom_OpenSSL(buffer, sizeof(buffer))); //-V521
    }
}

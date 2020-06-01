// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/CryptHashPassword.h"
#include "TestHelper/catch.hpp"
#include "Util/Error/Xception.h"

using namespace std;

TEST_CASE("Secure/CryptHashPassword")
{
    SECTION("OpenSSL")
    {
        try
        {
            CryptHashPassword2 hash("test", 10);

            bool foundNonZero = false;

            for (auto b : hash.salt)
            {
                if (b)
                {
                    foundNonZero = true;
                    break;
                }
            }

            for (auto b : hash.hash)
            {
                if (b)
                {
                    foundNonZero = true;
                    break;
                }
            }

            REQUIRE(foundNonZero); //-V521
        }
        catch (const Xception&)
        {
            FAIL("Unexpected Exception");
        }
    }
}

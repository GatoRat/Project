// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/SHA-256.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Secure/SHA-256")
{
    SHA256::Digest digest;
    SHA256::calc("heather", 7, digest);

    uint8_t expected[SHA256::DigestLen] = { 0xa0, 0xb7, 0xc2, 0x45, 0xfa, 0xb3, 0x34, 0x46,
                                            0x7e, 0x95, 0x08, 0x38, 0x6b, 0xdc, 0xcb, 0xf9,
                                            0x53, 0x95, 0x7d, 0x01, 0x04, 0x3e, 0xc0, 0x43,
                                            0xda, 0x91, 0x5a, 0xd9, 0x93, 0x76, 0x6a, 0x48 };
    REQUIRE(memcmp(digest.digest, expected, sizeof(expected)) == 0); //-V521
}

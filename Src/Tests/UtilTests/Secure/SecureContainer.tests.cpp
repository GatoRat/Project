// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Secure/SecureContainer.h"
#include "TestHelper/catch.hpp"
#include "Util/Misc/MakeFourCC.h"

using namespace std;

TEST_CASE("Secure/SecureContainer")
{
    SECTION("CopyCompressor")
    {
        SecureContainer::CopyCompressor copyCompressor;

        REQUIRE(copyCompressor.getType() == SecureContainer::NoCompression); //-V521
        REQUIRE(copyCompressor.calcMaxOutputSize(7) == 7); //-V521

        char srcData[3]{ 1, 2, 3 };
        char compressedData[3]{ 0, 0, 0 };

        REQUIRE(copyCompressor.compress(srcData, 3, compressedData, 3) == 3); //-V521
        REQUIRE(memcmp(srcData, compressedData, 3) == 0); //-V521

        char uncompressedData[3]{ 0, 0, 0 };

        REQUIRE(copyCompressor.Decompress(compressedData, 3, uncompressedData, 3) == 3); //-V521
        REQUIRE(memcmp(srcData, uncompressedData, 3) == 0); //-V521
    }
    SECTION("LZ4Compressor")
    {
        SecureContainer::LZ4Compressor compressor;

        REQUIRE(compressor.getType() == SecureContainer::LZ4Compressed); //-V521
        REQUIRE(compressor.calcMaxOutputSize(32) >= 32); //-V112 //-V521

        char srcData[32];
        memset(srcData, 'a', 32); //-V112

        char compressedData[128];
        int32_t compressedLen = compressor.compress(srcData, 32, compressedData, 128); //-V112

        REQUIRE(compressedLen > 0); //-V521

        char uncompressedData[48];

        REQUIRE(compressor.Decompress(compressedData, compressedLen, uncompressedData, 48) == 32); //-V112 //-V521
        REQUIRE(memcmp(uncompressedData, srcData, 32) == 0); //-V112 //-V521
    }
    SECTION("Encryption")
    {
        static uint8_t key[]
        {
             1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
            11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
            31, 32
        };

        SECTION("no compression")
        {
            vector<uint8_t> container = SecureContainer::Encode("data", 4, key, nullptr); //-V112

            try
            {
                vector<uint8_t> data = SecureContainer::Decode(container.data(), container.size(), 256, key);
                REQUIRE_FALSE(data.empty()); //-V521
                REQUIRE(memcmp(data.data(), "data", 4) == 0); //-V112 //-V521
            }
            catch (exception&)
            {
                FAIL("unexpected exception");
            }
        }
        SECTION("LZ4")
        {
            SecureContainer::LZ4Compressor compressor;
            vector<uint8_t> container = SecureContainer::Encode("data", 4, key, &compressor, 0); //-V112

            try
            {
                vector<uint8_t> data = SecureContainer::Decode(container.data(), container.size(), 256, key);
                REQUIRE_FALSE(data.empty()); //-V521
                REQUIRE(memcmp(data.data(), "data", 4) == 0); //-V112 //-V521
            }
            catch (exception&)
            {
                FAIL("unexpected exception");
            }
        }
    }
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "VerifyFormat.Image.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include "../File/FastFile.h"
#include "../Misc/ByteOrder.h"
#include "../Misc/MakeFourCC.h"
#include "../Secure/Crc32.h"

using namespace std;

#pragma pack(push, 1)

// TODO: Implement ImageCache verify
// Verify file exists and has size
// Should it check for PNG header? ({ 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a }
// For PNG could also check the IHDR: IHDR must be the first chunk; it contains (in this order) the image's width, height, bit depth, _color type, compression method, filter method, and interlace method (13 data bytes total)

struct PNG
{
    uint8_t  header[8];   // must be { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a }
    uint32_t len;         // big-endian: must be 13
    uint32_t fourcc;      // Must be "IHDR"
    uint32_t width;       // big-endian: Zero is invalid; BingoAV max for plain images: 4k, for textures: 16k
    uint32_t height;      // big-endian: Zero is invalid; BingoAV max for plain images: 4k, for textures: 16k
    uint8_t  bitDepth;    // Valid values: 1,2,4,8,16 (see chart below)
    uint8_t  colorType;   // Valid values: 0,2,3,4,6 (see chart below)
    uint8_t  compressionMethod;  // Must be zero
    uint8_t  filterMethod;      // Must be zero
    uint8_t  interlaceMethod;   // Valid values: 0, 1
    uint32_t crc;         // big-endian: CRC-32 includes everything after length and before crc.

    bool verifyHeader() const
    {
        static uint8_t expectedHeader[] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
        return memcmp(header, expectedHeader, sizeof(header)) == 0;
    }

    bool verify() const
    {
        constexpr uint32_t expectedLen = 0x0d000000;

        return verifyHeader() &&
            len == expectedLen &&
            fourcc == MakeFourCC('I', 'H', 'D', 'R') &&
            width > 0 &&
            height > 0 &&
            verifyBitDepth() &&
            compressionMethod == 0 &&
            filterMethod == 0 &&
            interlaceMethod <= 1 &&
            verifyCRC();
    }

    bool verifyBitDepth() const
    {
        bool success = false;

        switch (colorType)
        {
            case 0:
                success = bitDepth == 1 || bitDepth == 2 || bitDepth == 4 || bitDepth == 8 || bitDepth == 16; //-V112
                break;

            case 2:
            case 4:
            case 6:
                success = bitDepth == 8 || bitDepth == 16;
                break;

            case 3:
                success = bitDepth == 1 || bitDepth == 2 || bitDepth == 4 || bitDepth == 8; //-V112
                break;
        }

        return success;
    }

    bool verifyCRC() const
    {
        const void* p = &fourcc;
        const uint32_t calculatedCrc32 = Crc32(p, sizeof(*this) - sizeof(header) - sizeof(len) - sizeof(crc));
        return crc == ChangeEndianness32(calculatedCrc32);
    }
};

#pragma pack(pop)

bool VerifyPNG(const fs::path& pathname, bool /*quick*/)
{
    bool success = false;

    PNG png;
    const size_t lenRead = FastReadFile(pathname, &png, sizeof(png)); //-V101
    if (lenRead == sizeof(png))
    {
        success = png.verify();
    }

    return success;
}

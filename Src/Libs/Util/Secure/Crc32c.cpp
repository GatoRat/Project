// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
    The author disclaims copyright to this source code.

    Permission to use, copy, modify, and/or distribute this software for any purpose
    with or without fee is hereby granted, provided that the above copyright notice
    and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
    REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
    FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT,
    OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
    DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
    ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "Crc32c.h"
#include "../System/SysInfo.h"

#include "Crc32cTable.h"

static uint32_t Crc32c_Cpp(const void* pData, uint32_t dataLen, uint32_t crc)
{
    union
    {
        const uint8_t*  pData8;
        const uint32_t* pData32;
    };

    pData8 = static_cast<const uint8_t*>(pData);

    crc = ~crc;

    while (dataLen && (reinterpret_cast<ptrdiff_t>(pData8) & 3))
    {
        crc = (crc >> 8) ^ crc32cTable[0][(crc ^ *pData8++) & 0xff];
        dataLen--;
    }

    constexpr uint32_t BytesToProcessAtOnce = 8;
    while (dataLen >= BytesToProcessAtOnce)
    {
        const uint32_t one = *pData32++ ^ crc;
        const uint32_t two = *pData32++;

        crc = crc32cTable[7][one & 0xff] ^ crc32cTable[6][(one >> 8) & 0xff] ^ crc32cTable[5][(one >> 16) & 0xff] ^ crc32cTable[4][one >> 24]
            ^ crc32cTable[3][two & 0xff] ^ crc32cTable[2][(two >> 8) & 0xff] ^ crc32cTable[1][(two >> 16) & 0xff] ^ crc32cTable[0][two >> 24];

        dataLen -= 8;
    }

    while (dataLen--)
    {
        crc = (crc >> 8) ^ crc32cTable[0][(crc & 0xff) ^ *pData8++];
    }

    return ~crc;
}

#if defined(__MSC__)

    #define __SSE4_2__
    #include <nmmintrin.h>

    static uint32_t Crc32c_intrinsic(const void* pData, uint32_t dataLen, uint32_t crc)
    {
        crc = ~crc;

        union
        {
            const uint8_t*  pData8;
            const uint32_t* pData32;
            const uint64_t* pData64;
        };

        pData8 = static_cast<const uint8_t*>(pData);

        if (dataLen && (reinterpret_cast<ptrdiff_t>(pData8) & 7))
        {
            crc = _mm_crc32_u8(crc, *pData8++);
            dataLen--;
        }

    #ifdef ARCH_64

        uint64_t crc64 = static_cast<uint64_t>(crc);

        while (dataLen >= sizeof(uint64_t))
        {
            crc64 = _mm_crc32_u64(crc64, *pData64++);
            dataLen -= sizeof(uint64_t);
        }

        crc = static_cast<uint32_t>(crc64);

    #else

        while (dataLen >= sizeof(uint32_t))
        {
            crc = _mm_crc32_u32(crc, *pData32++);
            dataLen -= sizeof(uint32_t);
        }

    #endif

        while (dataLen--)
        {
            crc = _mm_crc32_u8(crc, *pData8++);
        }

        return ~crc;
    }

    uint32_t Crc32c(const void* pData, uint32_t dataLen, uint32_t crc, bool useCppVersion)
    {
        if (pData && dataLen)
        {
            if (useCppVersion || !IsSSE42Supported())
            {
                crc = Crc32c_Cpp(pData, dataLen, crc);
            }
            else
            {
                crc = Crc32c_intrinsic(pData, dataLen, crc);
            }
        }

        return crc;
    }

#else

    uint32_t Crc32c(const void* pData, uint32_t dataLen, uint32_t crc, bool)
    {
        if (pData && dataLen)
        {
            crc = Crc32c_Cpp(pData, dataLen, crc);
        }
        return crc;
    }

#endif

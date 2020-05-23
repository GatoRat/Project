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

#include "Crc32.h"

struct Crc32Table
{
    uint32_t data[256];

    Crc32Table()
    {
        constexpr uint32_t Polynomial = 0xedb88320L;

        for (uint32_t i = 0; i < 256; ++i)
        {
            uint32_t remainder = i;

            for (int bit = 0; bit < 8; bit++)
            {
                if (remainder & 1)
                {
                    remainder = Polynomial ^ (remainder >> 1);
                }
                else
                {
                    remainder = remainder >> 1;
                }
            }

            data[i] = remainder;
        }
    }
};

static uint32_t Crc32Update(uint32_t crc, const uint8_t* pData, uint32_t dataLen)
{
    static Crc32Table table;

    for (uint32_t i = 0; i < dataLen; i++)
    {
        crc = table.data[(crc ^ pData[i]) & 0xff] ^ (crc >> 8); //-V108
    }

    return crc;
}

uint32_t Crc32(const void* pData, uint32_t dataLen, uint32_t initialCrc)
{
    return pData && dataLen ? Crc32Update(initialCrc, static_cast<const uint8_t*>(pData), dataLen) ^ initialCrc : 0;
}

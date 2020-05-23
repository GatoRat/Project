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

#pragma once

#include <cstdint>
#include <cstring>
#include <string_view>

/// <summary>
/// Calculates the CRC-32 of data.
/// </summary>
/// <param name="pData">     The data.</param>
/// <param name="dataLen">   Length of the data.</param>
/// <param name="initialCrc">The initial CRC.</param>
/// <returns>The Crc32 of the the data.</returns>
uint32_t Crc32(const void* pData, uint32_t dataLen, uint32_t initialCrc = 0xffffffff);

/// <summary>
/// Calculates a CRC-32 of a string.
/// </summary>
/// <param name="pStr">The string.</param>
/// <returns>The Crc32 of the the data.</returns>
inline
uint32_t Crc32(const char* pStr)
{
    return pStr ? Crc32(pStr, static_cast<uint32_t>(strlen(pStr))) : 0;
}

/// <summary>
/// Calculates a CRC-32 of a string_view.
/// </summary>
/// <param name="str">The string view.</param>
/// <returns>The Crc32 of the the data.</returns>
inline
uint32_t Crc32(std::string_view str)
{
    return str.size() ? Crc32(str.data(), static_cast<uint32_t>(str.size())) : 0;
}

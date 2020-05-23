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
/// Calculates the CRC-32c of data.
/// </summary>
/// <param name="pData">The data.</param>
/// <param name="dataLen">Length of the data.</param>
/// <param name="initialCrc">The initial CRC.</param>
/// <param name="forceCppVersion">If <b>true</b>, force using C++ version.</param>
/// <returns>The Crc32c of the the data.</returns>
uint32_t Crc32c(const void* pData, uint32_t dataLen, uint32_t initialCrc = 0, bool useCppVersion = false);

/// <summary>
/// Calculates a CRC-32c of a string.
/// </summary>
/// <param name="pStr">The string.</param>
/// <param name="forceCppVersion">If <b>true</b>, force using C++ version.</param>
/// <returns>The Crc32c of the the data.</returns>
inline
uint32_t Crc32c(const char* pStr, bool forceCppVersion = false)
{
    return pStr ? Crc32c(pStr, static_cast<uint32_t>(strlen(pStr)), 0, forceCppVersion) : 0;
}

/// <summary>
/// Calculates a CRC-32c of a string_view.
/// </summary>
/// <param name="str">The string view.</param>
/// <param name="forceCppVersion">If <b>true</b>, force using C++ version.</param>
/// <returns>The Crc32c of the the data.</returns>
inline
uint32_t Crc32c(const std::string_view str, bool forceCppVersion = false)
{
    return Crc32c(str.data(), static_cast<uint32_t>(str.size()), 0, forceCppVersion);
}

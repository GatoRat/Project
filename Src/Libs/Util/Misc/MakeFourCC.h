// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>

//#define MAKE_FOURCC(a,b,c,d) ((uint32_t)(uint8_t)(a) | ((uint32_t)(uint8_t)(b) << 8) | ((uint32_t)(uint8_t)(c) << 16) | ((uint32_t)(uint8_t)(d) << 24 ))

/// <summary>
/// Makes a FourCC.
/// </summary>
/// <param name="a">The first character.</param>
/// <param name="b">The second character.</param>
/// <param name="c">The third character.</param>
/// <param name="d">The fourth character.</param>
/// <returns>A FourCC of the passed values.</returns>
/// <remarks>
/// A FourCC is a sequence of four case sensitive ASCII printable characters often
/// used in media formats.
/// </remarks>
constexpr uint32_t MakeFourCC(char a, char b, char c, char d)
{
    return
         static_cast<uint32_t>(static_cast<uint8_t>(a))        |
        (static_cast<uint32_t>(static_cast<uint8_t>(b)) <<  8) |
        (static_cast<uint32_t>(static_cast<uint8_t>(c)) << 16) |
        (static_cast<uint32_t>(static_cast<uint8_t>(d)) << 24);
}

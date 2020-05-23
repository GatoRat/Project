// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>

/** Gets digits required to represent a value.
   @param val The value.
   @returns The maximum digits required to represent the passed value.
 */
constexpr size_t GetNumDigits(uint64_t val)
{
    size_t numDigits = 0;

    do
    {
        numDigits++;
        val /= 10;

    } while (val);

    return numDigits;
}

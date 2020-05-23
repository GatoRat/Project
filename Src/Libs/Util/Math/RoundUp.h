// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>

/**
    Rounds a value up to the nearest multiple.
    @param num The value to round up.
    @param multiple The target multiple.
    @remarks If \p multiple is zero, this function will return 0.
*/
constexpr uint32_t RoundUp(uint32_t num, uint32_t multiple)
{
    return num ? (multiple ? (num - 1 - (num - 1) % multiple + multiple) : 0) : multiple;
}

/**
    Rounds a value up to the nearest multiple.
    @param num The value to round up.
    @param multiple The target multiple.
    @remarks If \p multiple is zero, this function will return 0.
*/
constexpr uint64_t RoundUp(uint64_t num, uint64_t multiple)
{
    return num ? (multiple ? (num - 1 - (num - 1) % multiple + multiple) : 0) : multiple;
}

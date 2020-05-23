// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>

/**
    This is a pseudo-random number generator which should only be used when trying
    to generate a sequence of pseudo-random numbers which needs to be super fast
    with low overhead.

    Also note that despite the parameter, the range is actually 0 - 0x7fffffff.
*/

uint32_t FastRand(uint32_t lastrandom);

template<typename T>
T FastRandGetBetween(uint32_t seed, T min, T max)
{
    double x = seed * (2.0 / 4294967296.0);
    return min + static_cast<T>((max - min + 1) * x);
}

template<typename T>
T FastRandBetween(uint32_t& seed, T min, T max)
{
    seed = FastRand(seed);
    return FastRandGetBetween(seed, min, max);
}

/**
    \param pBuffer Buffer into which to write data.
    \param bufferLen Length of buffer in 'T' items \b NOT sizeof().
*/
template<typename T>
uint32_t FastRandBuffer(uint32_t& seed, T* pBuffer, size_t bufferLen, int min, int max)
{
    while (bufferLen--)
    {
        pBuffer[bufferLen] = static_cast<T>(FastRandBetween(seed, min, max));
    }

    return seed;
}

template<typename T>
uint32_t FastRandBuffer2(uint32_t& seed, T* pBuffer, size_t bufferLen, T min, T max)
{
    while (bufferLen--)
    {
        pBuffer[bufferLen] = FastRandBetween(seed, min, max);
    }

    return seed;
}

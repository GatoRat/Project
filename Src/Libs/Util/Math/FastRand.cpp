// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/**************************************************************************
generate 2**31-2 random numbers
based on "Park-Miller-Carta Pseudo-Random Number Generator",
http://www.firstpr.com.au/dsp/rand31/

The following code is public domain.

For reference; with a seed of 1, the 10,000th iteration
should return 1043618065
**************************************************************************/

#include "FastRand.h"

static uint32_t NextRand(uint32_t seed)
{
    uint32_t lo = 16807 * static_cast<uint32_t>(seed & 0xffff);
    const uint32_t hi = 16807 * static_cast<uint32_t>(seed >> 16);
    lo += (hi & 0x7fff) << 16;
    lo += hi >> 15;

    if (lo > 0x7fffffff) //-V112
    {
        lo -= 0x7fffffff; //-V112
    }

    return lo;
}

uint32_t FastRand(uint32_t lastrandom)
{
    return NextRand(lastrandom);
}

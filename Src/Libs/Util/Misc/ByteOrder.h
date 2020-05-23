// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include "../Detect/DetectCompiler.h"

#ifdef __MSC__

    #include <cstdlib>

    inline
    uint16_t ChangeEndianness16(uint16_t val)
    {
        return _byteswap_ushort(val);
    }

    inline
    uint32_t ChangeEndianness32(uint32_t val)
    {
        return _byteswap_ulong(val);
    }

    inline
    uint64_t ChangeEndianness64(uint64_t val)
    {
        return _byteswap_uint64(val);
    }

#endif // __MSC__

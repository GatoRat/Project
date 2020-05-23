// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

// clang-format off
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #ifndef BIG_ENDIAN
        #define BIG_ENDIAN
    #endif
    #ifdef LITTLE_ENDIAN
        #undef LITTLE_ENDIAN
    #endif
#else
    #ifndef LITTLE_ENDIAN
        #define LITTLE_ENDIAN
    #endif
    #ifdef BIG_ENDIAN
        #undef BIG_ENDIAN
    #endif
#endif
// clang-format on

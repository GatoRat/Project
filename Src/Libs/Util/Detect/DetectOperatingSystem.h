// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "DetectArchitecture.h"

/*
    The predefined constants for operating systems is a growing standard.
    However, there are some subtle differences with compilers.
    This ensures that the most common constants can be used:

    Mac: __APPLE__
    Linux: __linux__
    Windows: _WIN32   (For 64-bit Windows, both _WIN32 and _WIN64 will be defined.)
    //Android: ????
    //iOS: ????
*/

// clang-format off
#if defined(__APPLE__)
    #ifdef linux
        #undef linux
    #endif
    #ifdef __linux
        #undef __linux
    #endif
    #ifdef __linux__
        #undef __linux__
    #endif
    #ifdef __gnu_linux
        #undef __gnu_linux
    #endif
    #ifdef WIN32
        #undef WIN32
    #endif
    #ifdef _WIN32
        #undef _WIN32
    #endif
    #ifdef WIN64
        #undef WIN64
    #endif
    #ifdef _WIN64
        #undef _WIN64
    #endif
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux)
    #ifndef __linux__
        #define __linux__
    #endif
    #ifdef __APPLE__
        #undef __APPLE__
    #endif
    #ifdef WIN32
        #undef WIN32
    #endif
    #ifdef _WIN32
        #undef _WIN32
    #endif
    #ifdef WIN64
        #undef WIN64
    #endif
    #ifdef _WIN64
        #undef _WIN64
    #endif
#elif defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
    #ifndef WIN32
        #define WIN32
    #endif
    #ifndef _WIN32
        #define _WIN32
    #endif
    #ifdef ARCH_64
        #ifndef WIN64
            #define WIN64
        #endif
        #ifndef _WIN64
            #define _WIN64
        #endif
    #endif
    #ifdef __APPLE__
        #undef __APPLE__
    #endif
    #ifdef linux
        #undef linux
    #endif
    #ifdef __linux
        #undef __linux
    #endif
    #ifdef __linux__
        #undef __linux__
    #endif
    #ifdef __gnu_linux
        #undef __gnu_linux
    #endif
#endif
// clang-format on

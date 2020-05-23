// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "DetectCompiler.h"

/*
    Intel/AMD 64: ARCH_X64 and ARCH_64
    Intel/AMD 32: ARCH_X86 and ARCH_32
    Arm 64: ARCH_ARM and ARCH_64
    Arm 32: ARCH_ARM and ARCH_32
*/

// clang-format off
#if defined(__clang__)

    #if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64) || defined(_M_X64)
        #ifndef X64
            #define ARCH_X64
        #endif
        #ifndef ARCH_64
            #define ARCH_64
        #endif
    #elif defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(_X86_)
        #ifndef X86
            #define ARCH_X86
        #endif
        #ifndef ARCH_32
            #define ARCH_32
        #endif
    #elif defined(__ARM_ARCH) || defined(__ARM_ARCH_ISA_A64) || defined(__arm__) || defined(__arm) || defined(__thumb__) || defined(__thumb)
        #ifndef ARCH_ARM
            #define ARCH_ARM
        #endif
        #ifdef __ARM_ARCH_ISA_A64
            #ifndef ARCH_64
                #define ARCH_64
            #endif
        #else
            #ifndef ARCH_32
                #define ARCH_32
            #endif
        #endif
    #else
        #error Unknown Architecture
    #endif

#elif defined(__MSC__)

    #if defined (_M_AMD64) || defined(_M_X64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
        #ifndef X64
            #define ARCH_X64
        #endif
        #ifndef ARCH_64
            #define ARCH_64
        #endif
    #elif defined(_M_IX86)
        #ifndef ARCH_X86
            #define ARCH_X86
        #endif
        #ifndef ARCH_32
            #define ARCH_32
        #endif
    #elif defined(_M_ARM) || defined(_M_ARMT)
        #ifndef ARCH_ARM
            #define ARCH_ARM
        #endif
        #ifdef defined(_M_ARM64) || defined(_M_ARMT64) || defined(_M_ARM64T)
            #ifndef ARCH_64
                #define ARCH_64
            #endif
        #else
            #ifndef ARCH_32
                #define ARCH_32
            #endif
        #endif
    #else
        #error Unknown Architecture
    #endif

#elif defined(__GNUC__)

    #if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64) || defined(_M_X64)
        #ifndef ARCH_X64
            #define ARCH_X64
        #endif
        #ifndef ARCH_64
            #define ARCH_64
        #endif
    #elif defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(_X86_)
        #ifndef ARCH_X86
            #define ARCH_X86
        #endif
        #ifndef ARCH_32
            #define ARCH_32
        #endif
    #elif defined(__ARM_ARCH) || defined(__ARM_ARCH_ISA_A64) || defined(__arm__) || defined(__arm) || defined(__thumb__) || defined(__thumb)
        #ifndef ARCH_ARM
            #define ARCH_ARM
        #endif
        #ifdef __ARM_ARCH_ISA_A64
            #ifndef ARCH_64
                #define ARCH_64
            #endif
        #else
            #ifndef ARCH_32
                #define ARCH_32
            #endif
        #endif
    #else
        #error Unknown Architecture
    #endif
#endif
// clang-format on

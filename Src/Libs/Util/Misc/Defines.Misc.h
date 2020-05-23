// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

// clang-format off
// These are defines rather than constexpr so they can be used with any type

#define BASE_TWO        2
#define BASE_TEN        10
#define BASE_SIXTEEN    16

#define TEN             10
#define ONE_HUNDRED     (TEN * TEN)
#define ONE_THOUSAND    (TEN * ONE_HUNDRED)
#define ONE_MILLION     (ONE_THOUSAND * ONE_THOUSAND)

#define ONE_KILOBYTE    1024
#define ONE_MEGABYTE    (ONE_KILOBYTE * ONE_KILOBYTE)
#define ONE_GIGABYTE    (ONE_KILOBYTE * ONE_MEGABYTE)

#define STRINGIFY(x)    #x
#define TOSTRING(x)     STRINGIFY(x)

#define WSTRINGIFY(x)   L ## x
#define TOWSTRING(x)    WSTRINGIFY(x)

#define ERROR_STRING(errStr)                __FILE__ "(" TOSTRING(__LINE__)  "): " errStr
#define ERROR_STRING_NUM(errStr, errNum)    __FILE__ "(" TOSTRING(__LINE__)  ")[" TOSTRING(errNum) "]: " errStr

// clang-format on

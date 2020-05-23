// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "../Misc/Defines.Misc.h"

// clang-format off

#ifdef NDEBUG
    #ifdef _DEBUG
        #undef _DEBUG
    #endif
#else
    #ifndef _DEBUG
        #define _DEBUG
    #endif
    #ifdef NDEBUG
        #undef NDEBUG
    #endif
    #define OPTIMIZE_OFF
    #define OPTIMIZE_ON
#endif

#define DISABLE_EXPRESSION_BEFORE_COMMA_WARNING_START
#define DISABLE_EXPRESSION_BEFORE_COMMA_WARNING_END

// Clang detection must be first
#if defined(__clang__)

    #if ((__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__) < 30800)
        #pragma message("Clang compiler version: " TOSTRING(__clang_major__) "." TOSTRING(__clang_minor__) "." TOSTRING(__clang_patchlevel__))
        #error Clang 3.8.0 or later must be used.
    #endif

    // These resolve warnings in various headers. These warnings should be enabled for most local code.
    #define DISABLE_3RD_PARTY_WARNINGS_START \
        _Pragma("clang diagnostic push")     \
        _Pragma("clang diagnostic ignored \"-Wc++11-narrowing\"")

    #define DISABLE_3RD_PARTY_WARNINGS_END \
        _Pragma("clang diagnostic pop")

    #define DISABLE_CONVERSION_WARNINGS_START
    #define DISABLE_CONVERSION_WARNINGS_END

    #define DISABLE_QT_WARNINGS_START
    #define DISABLE_QT_WARNINGS_END

    #define DISABLE_UNREACHABLE_CODE_START
    #define DISABLE_UNREACHABLE_CODE_END

    #define SUPPRESS_SECURITY_WARNING
    #define SUPPRESS_EXPRESSION_BEFORE_COMMA_WARNING

    #define DISABLE_WARNING_FORMAT_SECURITY_START \
            _Pragma("clang diagnostic push")     \
            _Pragma("clang diagnostic ignored \"-Wformat-security\"")

    #define DISABLE_WARNING_FORMAT_SECURITY_END \
            _Pragma("clang diagnostic pop")

    #define DISABLE_WARNING_ZERO_AS_NULL_POINTER_START \
            _Pragma("clang diagnostic push")     \
            _Pragma("clang diagnostic ignored \"-Wzero-as-null-pointer-constant\"")

    #define DISABLE_WARNING_ZERO_AS_NULL_POINTER_END \
            _Pragma("clang diagnostic pop")

    #define DISABLE_WARNING_SHIFT_NEGATIVE_VALUE_START \
            _Pragma("clang diagnostic ignored \"-Wshift-negative-value\"")

    #define DISABLE_WARNING_SHIFT_NEGATIVE_VALUE_END \
            _Pragma("clang diagnostic pop")

    #ifdef NDEBUG
        #define OPTIMIZE_OFF   _Pragma("clang optimize off")
        #define OPTIMIZE_ON    _Pragma("clang optimize on")
    #endif

    #define DISABLE_WARNING_SUGGEST_ATTRIBUTE_FORMAT_START
    #define DISABLE_WARNING_SUGGEST_ATTRIBUTE_FORMAT_END

    #define DISABLE_PUGI_WARNINGS_START \
            _Pragma("clang diagnostic push") \
            _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"") \
            _Pragma("clang diagnostic ignored \"-Wswitch-enum\"") \
            _Pragma("clang diagnostic ignored \"-Wfloat-equal\"") \
            _Pragma("clang diagnostic ignored \"-Wmissing-noreturn\"")

    #define DISABLE_PUGI_WARNINGS_END \
            _Pragma("clang diagnostic pop")

    #define DISABLE_DEPRECATED_START \
            _Pragma("clang diagnostic push") \
            _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")

    #define DISABLE_DEPRECATED_END \
            _Pragma("clang diagnostic pop")

    #define ATTRIBUTE_PRINTF(FORMAT_SPECIFIER_ARG, FIRST_FORMATTED_ARG)

    #define _FUNC_      __PRETTY_FUNCTION__
    #define _WHERE_     __FILE__, __func__, __LINE__

    #ifdef _WIN32
        #define stack_alloc _alloca
    #else
        #define stack_alloc alloca
    #endif

#elif defined(_MSC_VER)

    #ifndef __MSC__
        #define __MSC__
    #endif

    #if _MSC_FULL_VER < 190024210
        #pragma message("Microsoft compiler version: " TOSTRING(_MSC_FULL_VER))
        #error Visual C++ 2015 Update 3 compiler (190024210) or later must be used.
    #endif

    // These resolve warnings in various headers. These warnings should be enabled for most local code.
    #define DISABLE_3RD_PARTY_WARNINGS_START \
            __pragma(warning(push)) \
            __pragma(warning(disable:4265)) /* class has virtual functions, but destructor is not virtual */ \
            __pragma(warning(disable:4548)) /* expression before comma has no effect; expected expression with side-effect */

    #define DISABLE_3RD_PARTY_WARNINGS_END \
            __pragma(warning(pop))

    #define DISABLE_CONVERSION_WARNINGS_START \
            __pragma(warning(push)) \
            __pragma(warning(disable:4244)) /* 'conversion' conversion from 'type1' to 'type2', possible loss of data */

    #define DISABLE_CONVERSION_WARNINGS_END \
            __pragma(warning(pop))

    #define DISABLE_QT_WARNINGS_START \
            __pragma(warning(push,3))

    #define DISABLE_QT_WARNINGS_END \
            __pragma(warning(pop))

    #define DISABLE_UNREACHABLE_CODE_START \
            __pragma(warning(push)) \
            __pragma(warning(disable:4702)) /* unreachable code */

    #define DISABLE_UNREACHABLE_CODE_END \
            __pragma(warning(pop))

    #ifdef _DEBUG
        #undef DISABLE_EXPRESSION_BEFORE_COMMA_WARNING_START
        #define DISABLE_EXPRESSION_BEFORE_COMMA_WARNING_START \
                __pragma(warning(push)) \
                __pragma(warning(disable:4548))

        #undef DISABLE_EXPRESSION_BEFORE_COMMA_WARNING_END
        #define DISABLE_EXPRESSION_BEFORE_COMMA_WARNING_END \
                __pragma(warning(pop))
    #endif

    #define SUPPRESS_SECURITY_WARNING \
            __pragma(warning(suppress:4996)) /* This function or variable may be unsafe. */

    #define SUPPRESS_EXPRESSION_BEFORE_COMMA_WARNING \
            __pragma(warning(suppress:4548)) /* expression before comma has no effect; expected expression with side-effect */

    #define DISABLE_WARNING_FORMAT_SECURITY_START
    #define DISABLE_WARNING_FORMAT_SECURITY_END

    #ifdef NDEBUG
        #define OPTIMIZE_OFF   __pragma(optimize("", off))
        #define OPTIMIZE_ON    __pragma(optimize("", on))
    #endif

    #define DISABLE_WARNING_ZERO_AS_NULL_POINTER_START
    #define DISABLE_WARNING_ZERO_AS_NULL_POINTER_END
    #define DISABLE_WARNING_MISSING_FIELD_INITIALIZERS

    #define DISABLE_WARNING_SUGGEST_ATTRIBUTE_FORMAT_START
    #define DISABLE_WARNING_SUGGEST_ATTRIBUTE_FORMAT_END

    #define DISABLE_PUGI_WARNINGS_START
    #define DISABLE_PUGI_WARNINGS_END

    #define DISABLE_DEPRECATED_START \
            __pragma(warning(push)) \
            __pragma(warning(disable:4996)) /* The compiler encountered a deprecated declaration. */

    #define DISABLE_DEPRECATED_END \
            __pragma(warning(pop))

    #define ATTRIBUTE_PRINTF(FORMAT_SPECIFIER_ARG, FIRST_FORMATTED_ARG)

    #define _FUNC_       __FUNCSIG__
    #define _WHERE_      __FILE__, __FUNCTION__, __LINE__
    #define _COND_WHERE(_pFile_, _pFunction_, _line_) (_pFile_ ? _pFile_ : __FILE__), (_pFunction_ ? _pFunction_ : __FUNCTION__), (_line_ ? _line_ : __LINE__)
    #define stack_alloc  _alloca

#elif defined(__GNUC__)

    #if ((__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) < 60201)
        #pragma message("GCC compiler version: " TOSTRING(__GNUC__) "." TOSTRING(__GNUC_MINOR__) "." TOSTRING(__GNUC_PATCHLEVEL__))
        #error GCC 6.2.1 or later must be used.
    #endif

    // These resolve warnings in various headers. These warnings should be enabled for most local code.
    #define DISABLE_3RD_PARTY_WARNINGS_START
    #define DISABLE_3RD_PARTY_WARNINGS_END

    #define DISABLE_CONVERSION_WARNINGS_START
    #define DISABLE_CONVERSION_WARNINGS_END

    #define DISABLE_QT_WARNINGS_START
    #define DISABLE_QT_WARNINGS_END

    #define DISABLE_UNREACHABLE_CODE_START
    #define DISABLE_UNREACHABLE_CODE_END

    #define SUPPRESS_SECURITY_WARNING
    #define SUPPRESS_EXPRESSION_BEFORE_COMMA_WARNING

    #define DISABLE_WARNING_FORMAT_SECURITY_START
    #define DISABLE_WARNING_FORMAT_SECURITY_END

    #ifdef NDEBUG
        #define OPTIMIZE_OFF \
            _Pragma("GCC push_options") \
            _Pragma("GCC optimize(\"-O0\")")

        #define OPTIMIZE_ON  \
            _Pragma("GCC pop_options")
    #endif

    #define DISABLE_WARNING_ZERO_AS_NULL_POINTER_START \
            _Pragma("GCC diagnostic push")     \
            _Pragma("GCC diagnostic ignored \"-Wzero-as-null-pointer-constant\"")

    #define DISABLE_WARNING_ZERO_AS_NULL_POINTER_END \
            _Pragma("GCC diagnostic pop")

    #define DISABLE_WARNING_MISSING_FIELD_INITIALIZERS \
            _Pragma("GCC diagnostic ignored \"-Wmissing-field-initializers\"")

    #define DISABLE_WARNING_SUGGEST_ATTRIBUTE_FORMAT_START \
            _Pragma("GCC diagnostic push")         \
            _Pragma("GCC diagnostic ignored \"-Wsuggest-attribute=format\"")

    #define DISABLE_WARNING_SUGGEST_ATTRIBUTE_FORMAT_END \
            _Pragma("GCC diagnostic pop")

    #define DISABLE_PUGI_WARNINGS_START
    #define DISABLE_PUGI_WARNINGS_END

    #define DISABLE_DEPRECATED_START \
            _Pragma("GCC diagnostic push") \
            _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")

    #define DISABLE_DEPRECATED_END \
            _Pragma("GCC diagnostic pop")

    #define ATTRIBUTE_PRINTF(FORMAT_SPECIFIER_ARG, FIRST_FORMATTED_ARG) \
            __attribute__((__format__(__printf__, FORMAT_SPECIFIER_ARG, FIRST_FORMATTED_ARG)))

    #define _FUNC_      __PRETTY_FUNCTION__
    #define _WHERE_     __FILE__, __func__, __LINE__
    #define stack_alloc alloca

#endif

// clang-format on

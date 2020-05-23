// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <ostream>

namespace numeric_string
{
    constexpr int abs_val(int x)
    {
        return x < 0 ? -x : x;
    }

    constexpr int num_digits(int x)
    {
        return x < 0 ? 1 + num_digits(-x) : x < 10 ? 1 : 1 + num_digits(x / 10);
    }

    template<char... args>
    struct metastring
    {
        const char data[sizeof... (args)] = { args... };
    };

    template<int size, int x, char... args>
    struct numeric_builder
    {
        typedef typename numeric_builder<size - 1, x / 10, '0' + abs_val(x) % 10, args...>::type type;
    };

    // special case for two digits; minus sign is handled here
    template<int x, char... args>
    struct numeric_builder<2, x, args...>
    {
        typedef metastring < x < 0 ? '-' : '0' + x / 10, '0' + abs_val(x) % 10, args...> type;
    };

    // special case for one digit (positive numbers only)
    template<int x, char... args>
    struct numeric_builder<1, x, args...> {
        typedef metastring<'0' + x, args...> type;
    };
}

template<int x>
class constexpr_numeric_string
{
private:
    typedef typename numeric_string::numeric_builder<numeric_string::num_digits(x), x, '\0'>::type type;

    type value{};

public:
    const char* c_str() const
    {
        return value.data;
    }

    operator const char*() const
    {
        return value.data;
    }
};

template<int x>
std::ostream& operator <<(std::ostream& ostr, const constexpr_numeric_string<x>& str)
{
    ostr << str.c_str();
    return ostr;
}

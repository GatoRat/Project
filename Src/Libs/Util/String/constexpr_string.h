// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstring>
#include <type_traits>

template<typename T>
class constexpr_string
{
    static_assert(std::is_same<T, char>::value || std::is_same<T, wchar_t>::value,
        "char or wchar_t required.");

private:
    const T* const _pStr;
    const std::size_t _size;

public:
    template<std::size_t N>
    constexpr explicit constexpr_string(const T(&a)[N])
        : _pStr(a)
        , _size(N-1)
    {
    }

    constexpr char operator[](std::size_t n) const
    {
        return n < _size ? _pStr[n] : throw std::out_of_range("constexpr_string operator[]");
    }

    constexpr operator const T*() const
    {
        return _pStr;
    }

    constexpr const T* c_str() const
    {
        return _pStr;
    }

    constexpr const T* data() const
    {
        return _pStr;
    }

    constexpr std::size_t size() const
    {
        return _size;
    }

    constexpr bool operator==(const T* pStr) const
    {
        return strncmp(_pStr, pStr, _size) == 0;
    }
};

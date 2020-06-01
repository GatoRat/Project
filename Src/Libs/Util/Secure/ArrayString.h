#pragma once

#include <algorithm>
#include <stdexcept>
#include <string_view>
#include "SecureEraseMemory.h"

// This deliberately doesn't inherit from std::array for now to keep it simple
// It's also deliberately not virtual

template <size_t bufferSize>
class ArrayString final
{
public:
    ArrayString()
    {
        erase();
    }

    explicit ArrayString(const char* pStr)
    {
        erase();
        cat(pStr);
    }

    ArrayString(const char* pStr, size_t strLen)
    {
        erase();
        cat(pStr, strLen);
    }

    ArrayString(const ArrayString&) = delete;
    void operator=(const ArrayString&) = delete;

    ~ArrayString()
    {
        SecureEraseMemory(data(), bufferSize);
    }

    const char* data() const noexcept
    {
        return _str;
    }

    char* data() noexcept
    {
        return _str;
    }

    size_t size() const noexcept
    {
        return _strLen;
    }

    char operator[](size_t n) const
    {
        return n < size() ? _str[n] : throw std::out_of_range("SecureBuffer operator[]");
    }

    char& operator[](size_t n) //-V659
    {
        if (n >= size())
        {
            throw std::out_of_range("SecureBuffer operator[]");
        }
        return _str[n];
    }

    operator std::string_view() const noexcept
    {
        return getView();
    }

    std::string_view getView() const noexcept
    {
        return std::string_view(data(), size());
    }

    void erase()
    {
        SecureEraseMemory(data(), bufferSize);
        _strLen = 0;
    }

    const char* cat(const char* pStr)
    {
        return cat(pStr, bufferSize);
    }

    const char* cat(const char* pStr, size_t strLen)
    {
        return insertAt(_strLen, pStr, strLen);
    }

    const char* catV(std::string_view str)
    {
        return cat(str.data(), str.size());
    }

    const char* catData(const void* pSrc, size_t srcLen)
    {
        return insertDataAt(_strLen, pSrc, srcLen);
    }

    const char* insertAt(size_t dstOffset, const char* pStr)
    {
        return insertAt(pStr, bufferSize);
    }

    const char* insertAt(size_t dstOffset, const char* pStr, size_t strLen)
    {
        if (pStr && (dstOffset == 0 || dstOffset <= _strLen))
        {
            for (size_t srcOffset = 0;
                 dstOffset < (bufferSize - 1) && srcOffset < strLen && pStr[srcOffset];)
            {
                _str[dstOffset++] = pStr[srcOffset++];
            }

            if (dstOffset > _strLen)
            {
                _strLen = dstOffset;
            }
        }
        return _str;
    }

    const char* insertAtV(size_t dstOffset, std::string_view str)
    {
        return insertAt(dstOffset, str.data(), str.size());
    }

    const char* insertDataAt(size_t dstOffset, const void* pSrc, size_t srcLen)
    {
        if (pSrc && (dstOffset == 0 || dstOffset <= _strLen))
        {
            for (size_t srcOffset = 0; dstOffset < (bufferSize - 1) && srcOffset < srcLen;)
            {
                _str[dstOffset++] = reinterpret_cast<const uint8_t*>(pSrc)[srcOffset++];
            }

            if (dstOffset > _strLen)
            {
                _strLen = dstOffset;
            }
        }
        return _str;
    }

public:
    char   _str[bufferSize];
    size_t _strLen = 0;
};

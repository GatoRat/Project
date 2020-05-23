// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <algorithm>
#include <stdexcept>
#include "SecureEraseMemory.h"

// This deliberately doesn't inherit from std::array for now to keep it simple
// It's also deliberately not virtual

template <size_t bufferSize, typename T = uint8_t>
class SecureBuffer final
{
    static_assert(bufferSize != 0, "a non-zero buffer size is required.");
    static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, char>::value,
        "type but must char or uint8_t.");

public:
    explicit SecureBuffer(uint8_t fill = 0)
    {
        erase(fill);
    }

    explicit SecureBuffer(const char* pSrc)
    {
        erase();
        copy(pSrc);
    }

    SecureBuffer(const void* pSrc, size_t srcLenInBytes)
    {
        erase();
        copy(pSrc, srcLenInBytes);
    }

    SecureBuffer(const SecureBuffer&) = delete;
    void operator=(const SecureBuffer&) = delete;

    ~SecureBuffer()
    {
        SecureEraseMemory(data(), size());
    }

    const T* data() const noexcept
    {
        return _data;
    }

    T* data() noexcept
    {
        return _data;
    }

    size_t size() const noexcept
    {
        return bufferSize;
    }

    T operator[](size_t n) const
    {
        return n < size() ? _data[n] : throw std::out_of_range("SecureBuffer operator[]");
    }

    T& operator[](size_t n) //-V659
    {
        if (n >= size())
        {
            throw std::out_of_range("SecureBuffer operator[]");
        }
        return _data[n];
    }

    void erase(T fill = 0)
    {
        SecureEraseMemory(data(), size());
        if (fill)
        {
            memset(data(), fill, size());
        }
    }

    void copy(const char* pSrc)
    {
        if (pSrc)
        {
            for (size_t offset = 0; offset < bufferSize && pSrc[offset]; ++offset)
            {
                _data[offset] = pSrc[offset];
            }
        }
    }

    void copy(const void* pSrc, size_t srcLenInBytes)
    {
        if (pSrc)
        {
            memcpy(data(), pSrc, std::min(srcLenInBytes, bufferSize));
        }
    }

    void copyAt(size_t offset, const void* pSrc, size_t srcLenInBytes)
    {
        if (pSrc && offset < bufferSize)
        {
            memcpy(&_data[offset], pSrc, std::min(srcLenInBytes, bufferSize - offset));
        }
    }

public:
    T _data[bufferSize];
};

#pragma once

#include <cstdint>
#include <memory>

inline
bool IsBufferEmpty(const void* pBuffer, size_t bufferLen)
{
    return reinterpret_cast<const uint8_t*>(pBuffer)[0] == 0 &&
           memcmp(pBuffer, &(reinterpret_cast<const uint8_t*>(pBuffer)[1]), bufferLen - 1) == 0;
}

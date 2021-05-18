// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include <functional>
#include <vector>

void RandSeed();
void RandCheckAndSeed();

// These do not use templated functions so as to keep the random engine hidden.

// Floating point results are NOT inclusive; val1 <= result < val2
float  RandBetween(float val1, float val2);
double RandBetween(double val1, double val2);

// Non-floating point results ARE inclusive; val1 <= result <= val2
int      RandBetween(int val1, int val2);
uint32_t RandBetweenUint32(uint32_t val1, uint32_t val2);

void RandBuffer(void* pBuffer, size_t randLen, uint8_t minVal = 0, uint8_t maxVal = UINT8_MAX);

void RandAppend(std::vector<uint8_t> data, size_t randLen, uint8_t minVal = 0, uint8_t maxVal = UINT8_MAX);

template<typename T>
T* FisherYatesShuffle(T* pData, uint32_t dataLen)
{
    if (pData && dataLen > 1)
    {
        for (uint32_t i = dataLen - 1; i > 0; i--)
        {
            uint32_t j = RandBetweenUint32(0, i);
            if (i != j)
            {
                std::swap(pData[i], pData[j]); //-V108
            }
        }
    }
    return pData;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type*
FisherYatesInsideOut(T* pData, uint32_t dataLen)
{
    if (pData && dataLen)
    {
        pData[0] = 0;
        for (uint32_t i = 1; i < dataLen; ++i)
        {
            uint32_t j = RandBetweenUint32(0, i);
            pData[i] = pData[j]; //-V108
            pData[j] = i; //-V108
        }
    }
    return pData;
}

namespace FisherYates
{
    typedef std::function<uint32_t(uint32_t val1, uint32_t val2)> RandBetweenCallback;

    uint32_t* InsideOut(uint32_t* pData, uint32_t dataLen, RandBetweenCallback randBetween);
}

template<typename T>
class RandMinMax
{
    static_assert(std::is_same<T, int>::value ||
                  std::is_same<T, float>::value  || std::is_same<T, double>::value,
                  "int, float or double required.");
public:
    constexpr RandMinMax(T min_ = 0.0f, T max_ = 0.0f) : min(min_), max(max_) {}

    T getRand() const
    {
        return RandBetween(min, max);
    }

public:
    T min;
    T max;
};
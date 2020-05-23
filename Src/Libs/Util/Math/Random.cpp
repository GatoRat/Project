// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Random.h"
#include <chrono>
#include <random>

using namespace std;

static mt19937 randEngine;
static bool seeded = false;

void RandSeed()
{
    randEngine.seed(static_cast<unsigned>(chrono::high_resolution_clock::now().time_since_epoch().count()));
    seeded = true;
}

static inline void CheckAndSeed()
{
    if (!seeded)
    {
        RandSeed();
    }
}

void RandCheckAndSeed()
{
    CheckAndSeed();
}

float RandBetween(float val1, float val2)
{
    CheckAndSeed();
    return uniform_real_distribution<float>(val1, val2)(randEngine);
}

double RandBetween(double val1, double val2)
{
    CheckAndSeed();
    return uniform_real_distribution<double>(val1, val2)(randEngine);
}

int RandBetween(int val1, int val2)
{
    CheckAndSeed();
    return uniform_int_distribution<int>(val1, val2)(randEngine);
}

uint32_t RandBetweenUint32(uint32_t val1, uint32_t val2)
{
    CheckAndSeed();
    return uniform_int_distribution<uint32_t>(val1, val2)(randEngine);
}

void RandBuffer(void* pBuffer_, size_t randLen, uint8_t minVal, uint8_t maxVal)
{
    CheckAndSeed();

    uint8_t* pBuffer = reinterpret_cast<uint8_t*>(pBuffer_);

    while (randLen--)
    {
        pBuffer[randLen] = static_cast<uint8_t>(uniform_int_distribution<uint16_t>(minVal, maxVal)(randEngine));
    }
}

void RandAppend(vector<uint8_t> data, size_t randLen, uint8_t minVal, uint8_t maxVal)
{
    const size_t offset = data.size();
    data.resize(offset + randLen);
    RandBuffer(&data[offset], randLen, minVal, maxVal);
}

/*
template<class InputIterator, class OutputIterator>
OutputIterator RandBufferT(InputIterator first, InputIterator last, OutputIterator dst)
{
    for (; first != last; ++dst, ++first)
    {
        *dst = *first;
    }
    return dst;
}
*/

namespace FisherYates
{
    uint32_t* InsideOut(uint32_t* pData, uint32_t dataLen, RandBetweenCallback randBetween)
    {
        if (pData && dataLen)
        {
            pData[0] = 0;
            for (uint32_t i = 1; i < dataLen; ++i)
            {
                const uint32_t j = randBetween(0, i);
                pData[i]   = pData[j]; //-V108
                pData[j]   = i; //-V108
            }
        }
        return pData;
    }
}
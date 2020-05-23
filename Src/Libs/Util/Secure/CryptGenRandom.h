// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <vector>

/**
    @param pBuffer A pointer to the buffer which will receive the random sequence.
    @param bufferLen The length, in bytes, of \p pBuffer. Due to restrictions of the
        underlying calls, if bufferLen is greater than INT_MAX, results may be
        undefined.
*/
bool CryptGenRandom(void* pBuffer, size_t bufferLen);
bool CryptGenRandom(std::vector<uint8_t>& buffer);
std::vector<uint8_t> CryptGenRandom(size_t bufferLen);

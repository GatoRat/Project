// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "AES_GCM.h"

namespace AES_GCM_OpenSSL
{
    int encrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData = nullptr, size_t additionalDataLen = 0);

    int decrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData = nullptr, size_t additionalDataLen = 0);
}

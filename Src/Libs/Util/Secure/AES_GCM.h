// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include "../Misc/Defines.Misc.h"

namespace AES_GCM
{
    constexpr size_t KeyLen     = 32; //-V112
    constexpr size_t IvLen      = 12;
    constexpr size_t TagLen     = 16;
    constexpr size_t MaxDataLen = INT32_MAX - ONE_KILOBYTE;
    constexpr size_t MaxAdditionalDataLen = ONE_KILOBYTE;

    /** Encrypts a buffer using AES GCM 256
        @param pKey The encryption key; must be KeyLen (32 bytes) long.
        @param pIV  The IV; must be IvLen (12 bytes) long.
        @param pSrc The source data. If nullptr, \p pDst will be used as the source.
        @param pDst The destination buffer. If \p pSrc is nullptr, this will also be
            the source data. This can be used to encrypt the data in place.
        @param dataLen The length of the source and destination buffers in bytes.
        @param pTag The tag which will be returned; this must be TagLen (16 bytes) long.
        @param pAdditionalData Optional additional data, which will not be encrypted but
            which will be included in the tag.
        @param additionalDataLen The length of the additional data, if used.
    */
    int encrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData = nullptr, size_t additionalDataLen = 0);

    /** Decrypts a buffer using AES GCM 256
        @param pKey The encryption key; must be KeyLen (32 bytes) long.
        @param pIV  The IV; must be IvLen (12 bytes) long.
        @param pSrc The source data. If nullptr, \p pDst will be used as the source.
        @param pDst The destination buffer. If \p pSrc is nullptr, this will also be
            the source data. This can be used to encrypt the data in place.
        @param dataLen The length of the source and destination buffers in bytes.
        @param pTag The tag which will be returned; this must be TagLen (16 bytes) long.
        @param pAdditionalData Optional additional data, which will not be encrypted but
            which will be included in the tag.
        @param additionalDataLen The length of the additional data, if used.
    */
    int decrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData = nullptr, size_t additionalDataLen = 0);
}

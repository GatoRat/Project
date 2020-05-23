// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "AES_GCM.h"

#ifdef USE_WIN32_BCRYPT
    #include "AES_GCM.Win32.h"
#else
    #include "AES_GCM.OpenSSL.h"
#endif

using namespace std;

namespace AES_GCM
{
    int encrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData, size_t additionalDataLen)
    {
#ifdef USE_WIN32_BCRYPT
        return AES_GCM_Win32::encrypt(pKey, pIV, pSrc, pDst, dataLen, pTag, pAdditionalData, additionalDataLen);
#else
        return AES_GCM_OpenSSL::encrypt(pKey, pIV, pSrc, pDst, dataLen, pTag, pAdditionalData, additionalDataLen);
#endif
    }

    int decrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData, size_t additionalDataLen)
    {
#ifdef USE_WIN32_BCRYPT
        return AES_GCM_Win32::decrypt(pKey, pIV, pSrc, pDst, dataLen, pTag, pAdditionalData, additionalDataLen);
#else
        return AES_GCM_OpenSSL::decrypt(pKey, pIV, pSrc, pDst, dataLen, pTag, pAdditionalData, additionalDataLen);
#endif
    }
}
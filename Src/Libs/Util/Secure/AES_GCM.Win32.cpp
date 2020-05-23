// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "AES_GCM.Win32.h"
#include <array>
#include "../System/_lean_windows_.h"
#include <Bcrypt.h>
#include "../Misc/Finally.h"
#include "CryptGenRandom.Win32.h"
#include "SecureEraseMemory.h"

#pragma comment(lib, "bcrypt.lib")

using namespace std;

namespace AES_GCM_Win32
{
    // https://docs.microsoft.com/en-us/windows/desktop/SecCNG/encrypting-data-with-cng

    int encrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData, size_t additionalDataLen)
    {
        int status = -1;
        if (pKey && pIV && pDst && dataLen < AES_GCM::MaxDataLen && pTag &&
            (!pAdditionalData || (additionalDataLen > 0 && additionalDataLen <= AES_GCM::MaxAdditionalDataLen)))
        {
            BCRYPT_ALG_HANDLE hAes = nullptr;
            BCRYPT_KEY_HANDLE hKey = nullptr;
            vector<uint8_t> key;

            Finally finally([&]()
            {
                BCryptDestroyKey(hKey);
                BCryptCloseAlgorithmProvider(hAes, 0);
                SecureEraseMemory(key.data(), key.size());
            });

            status = BCryptOpenAlgorithmProvider(&hAes, BCRYPT_AES_ALGORITHM, nullptr, 0);
            if (status >= 0)
            {
                ULONG outputLen = 0;
                ULONG keyObjectLen = 0;
                status = BCryptGetProperty(hAes, BCRYPT_OBJECT_LENGTH, (PBYTE)&keyObjectLen, sizeof(ULONG), &outputLen, 0); //-V206
                if (status >= 0)
                {
                    status = BCryptSetProperty(hAes, BCRYPT_CHAINING_MODE, (PBYTE)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0); //-V542
                    if (status >= 0)
                    {
                        key.resize(keyObjectLen); //-V106
                        status = BCryptGenerateSymmetricKey(hAes, &hKey, key.data(), keyObjectLen, (PBYTE)pKey, AES_GCM::KeyLen, 0);
                        if (status >= 0)
                        {
                            BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
                            BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
                            authInfo.pbNonce    = (PUCHAR) pIV;
                            authInfo.cbNonce    = (ULONG)  AES_GCM::IvLen;
                            authInfo.pbAuthData = (PUCHAR) pAdditionalData;
                            authInfo.cbAuthData = (ULONG)  additionalDataLen;
                            authInfo.pbTag      = (PUCHAR) pTag;
                            authInfo.cbTag      = (ULONG)  AES_GCM::TagLen;

                            status = BCryptEncrypt(hKey, (PUCHAR)(pSrc ? pSrc : pDst), (ULONG)dataLen, &authInfo, nullptr, 0,
                                                         (PUCHAR)pDst, (ULONG)dataLen, &outputLen, 0);
                            if (status >= 0)
                            {
                                status = static_cast<int>(outputLen);
                            }
                        }
                    }
                }
            }
        }

        return status;
    }

    int decrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData, size_t additionalDataLen)
    {
        int status = -1;

        if (pKey && pIV && pDst && dataLen > 0 && dataLen < AES_GCM::MaxDataLen && pTag &&
            (!pAdditionalData || (additionalDataLen > 0 && additionalDataLen <= AES_GCM::MaxAdditionalDataLen)))
        {
            BCRYPT_ALG_HANDLE hAes = nullptr;
            BCRYPT_KEY_HANDLE hKey = nullptr;
            vector<uint8_t> key;

            Finally finally([&]()
            {
                BCryptDestroyKey(hKey);
                BCryptCloseAlgorithmProvider(hAes, 0);
                SecureEraseMemory(key.data(), key.size());
            });

            status = BCryptOpenAlgorithmProvider(&hAes, BCRYPT_AES_ALGORITHM, nullptr, 0);
            if (status >= 0)
            {
                ULONG outputLen = 0;
                DWORD keyObjectLen = 0;
                status = BCryptGetProperty(hAes, BCRYPT_OBJECT_LENGTH, (PBYTE)&keyObjectLen, sizeof(DWORD), &outputLen, 0); //-V206
                if (status >= 0)
                {
                    DWORD blockLength = 0;
                    status = BCryptGetProperty(hAes, BCRYPT_BLOCK_LENGTH, (BYTE*)&blockLength, sizeof(blockLength), &outputLen, 0); //-V206
                    if (status >= 0)
                    {
                        status = BCryptSetProperty(hAes, BCRYPT_CHAINING_MODE, (PBYTE)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0); //-V542
                        if (status >= 0)
                        {
                            key.resize(keyObjectLen); //-V106
                            status = BCryptGenerateSymmetricKey(hAes, &hKey, key.data(), keyObjectLen, (PBYTE)pKey, AES_GCM::KeyLen, 0);
                            if (status >= 0)
                            {
                                BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
                                BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
                                authInfo.pbNonce    = (PUCHAR) pIV;
                                authInfo.cbNonce    = (ULONG)  AES_GCM::IvLen;
                                authInfo.pbAuthData = (PUCHAR) pAdditionalData;
                                authInfo.cbAuthData = (ULONG)  additionalDataLen;
                                authInfo.pbTag      = (PUCHAR) pTag;
                                authInfo.cbTag      = (ULONG)  AES_GCM::TagLen;

                                status = BCryptDecrypt(hKey, (PUCHAR)(pSrc ? pSrc : pDst), (ULONG)dataLen, &authInfo, nullptr, 0,
                                                             (PUCHAR)pDst, (ULONG)dataLen, &outputLen, 0);
                                if (status >= 0)
                                {
                                    status = (int)(outputLen);
                                }
                            }
                        }
                    }
                }
            }
        }
        return status;
    }
}
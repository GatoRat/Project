// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "AES_GCM.OpenSSL.h"
#include <OpenSSL/openssl/bio.h>
#include <OpenSSL/openssl/evp.h>
#include "../Misc/Finally.h"

#pragma comment(lib, "libcrypto.lib")

using namespace std;

namespace AES_GCM_OpenSSL
{
    int encrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData, size_t additionalDataLen)
    {
        int result = 0;

        if (pKey && pIV && pDst && dataLen > 0 && dataLen < AES_GCM::MaxDataLen && pTag &&
            (!pAdditionalData || (additionalDataLen > 0 && additionalDataLen <= AES_GCM::MaxAdditionalDataLen)))
        {
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (ctx)
            {
                Finally finally([&]()
                {
                    EVP_CIPHER_CTX_free(ctx);
                });

                result = -1;

                if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) == 1)
                {
                    // Set IV length if default 96 bits is not appropriate
                    //EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, IvLen, nullptr);

                    // Initialize key and IV
                    if (EVP_EncryptInit_ex(ctx, nullptr, nullptr, pKey, pIV) == 1)
                    {
                        int outlen = 0;

                        bool success = true;

                        if (pAdditionalData)
                        {
                            success = EVP_EncryptUpdate(ctx, nullptr, &outlen, reinterpret_cast<const uint8_t*>(pAdditionalData), static_cast<int>(additionalDataLen)) == 1;
                        }

                        if (success)
                        {
                            if (EVP_EncryptUpdate(ctx, pDst, &outlen, pSrc ? pSrc : pDst, static_cast<int>(dataLen)) == 1)
                            {
                                // Finalize: No output for GCM
                                int discardedOutLen = 0;
                                if (EVP_EncryptFinal_ex(ctx, nullptr, &discardedOutLen) == 1)
                                {
                                    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_GET_TAG, AES_GCM::TagLen, pTag) == 1)
                                    {
                                        result = outlen;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    int decrypt(
        const uint8_t* pKey, const uint8_t* pIV,
        const uint8_t* pSrc, uint8_t* pDst, size_t dataLen, uint8_t* pTag,
        const void* pAdditionalData, size_t additionalDataLen)
    {
        int result = 0;

        if (pKey && pIV && pDst && dataLen > 0 && dataLen < AES_GCM::MaxDataLen && pTag &&
            (!pAdditionalData || (additionalDataLen > 0 && additionalDataLen <= AES_GCM::MaxAdditionalDataLen)))
        {
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (ctx)
            {
                Finally finally([&]()
                {
                    EVP_CIPHER_CTX_free(ctx);
                });

                result = -1;

                if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) == 1)
                {
                    if (EVP_DecryptInit_ex(ctx, nullptr, nullptr, pKey, pIV) == 1)
                    {
                        int outlen = 0;

                        bool success = true;

                        if (pAdditionalData)
                        {
                            success = EVP_DecryptUpdate(ctx, nullptr, &outlen, reinterpret_cast<const uint8_t*>(pAdditionalData), static_cast<int>(additionalDataLen)) == 1;
                        }

                        if (success)
                        {
                            if (EVP_DecryptUpdate(ctx, pDst, &outlen, pSrc ? pSrc : pDst, static_cast<int>(dataLen)) == 1)
                            {
                                if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AES_GCM::TagLen, pTag) == 1)
                                {
                                    // Finalize: No output for GCM
                                    int discardedOutLen = 0;
                                    if (EVP_DecryptFinal_ex(ctx, nullptr, &discardedOutLen) == 1)
                                    {
                                        result = outlen;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return result;
    }
}
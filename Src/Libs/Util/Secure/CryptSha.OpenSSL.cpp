// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CryptSha.OpenSSL.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include "../Misc/Finally.h"

#pragma comment(lib, "libcrypto.lib")

using namespace std;

vector<uint8_t> CryptSha_OpenSSL(const void* pData, size_t dataLen, bool use512)
{
    vector<uint8_t> hash;
    hash.resize(use512 ? 64 : 32); //-V112

    if (!CryptSha_OpenSSL(pData, dataLen, hash.data(), hash.size()))
    {
        hash.clear();
    }

    return hash;
}

bool CryptSha_OpenSSL(const void* pData, size_t dataLen, uint8_t* pHash, size_t hashLen)
{
    bool success = false;

    if (hashLen == 32 || hashLen == 64) //-V112
    {
        auto ctx = EVP_MD_CTX_new();
        if (ctx)
        {
            Finally finally([&]()
            {
                EVP_MD_CTX_free(ctx);
            });

            if (EVP_DigestInit_ex(ctx, hashLen == 64 ? EVP_sha512() : EVP_sha256(), NULL))
            {
                if (EVP_DigestUpdate(ctx, pData, dataLen))
                {
                    unsigned tmpHashLen = 0;
                    if (EVP_DigestFinal_ex(ctx, pHash, &tmpHashLen))
                    {
                        success = true;
                    }
                }
            }
        }
    }

    return success;
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CryptHashPassword.OpenSSL.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include "../Error/Xception.h"
#include "../Math/Random.h"
#include "CryptGenRandom.OpenSSL.h"
#include "CryptHashPassword.h"

#pragma comment(lib, "libcrypto.lib")

using namespace std;

bool CryptHashPassword_OpenSSL(string_view password, const void* pSalt, int32_t saltLen, void* pHashBuffer, int32_t hashBufferLen, int32_t iterations)
{
    bool success = false;

    if (!password.empty() && iterations > 0 && pHashBuffer && hashBufferLen > 0)
    {
        success = PKCS5_PBKDF2_HMAC(password.data(), static_cast<int>(password.size()),
                                    reinterpret_cast<const unsigned char*>(pSalt), saltLen, iterations,
                                    EVP_sha512(), hashBufferLen, reinterpret_cast<unsigned char*>(pHashBuffer)) > 0;
    }

    return success;
}

void CryptHashPassword2::set(string_view password, int32_t iterations)
{
    if (!CryptGenRandom_OpenSSL(salt, sizeof(salt)))
    {
        if (!CryptGenRandom_OpenSSL(salt, sizeof(salt)))
        {
            RandBuffer(salt, sizeof(salt));
        }
    }

    if (!CryptHashPassword_OpenSSL(password, salt, SaltLen, hash, HashLen, iterations))
    {
        char errStr[256];
        auto code = ERR_get_error();
        ERR_error_string_n(code, errStr, sizeof(errStr));
        throw Xception(0, errStr);
    }
}
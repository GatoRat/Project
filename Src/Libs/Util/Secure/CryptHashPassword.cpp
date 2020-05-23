// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CryptHashPassword.h"

#ifdef USE_WIN32_BCRYPT
#else
    #include "CryptHashPassword.OpenSSL.h"
#endif
#include "SecureEraseMemory.h"

using namespace std;

bool CryptHashPassword(string_view password, const void* pSalt, int32_t saltLen, void* pHashBuffer, int32_t hashBufferLen, int32_t iterations)
{
#ifdef USE_WIN32_BCRYPT
#else
    return CryptHashPassword_OpenSSL(password, pSalt, saltLen, pHashBuffer, hashBufferLen, iterations);
#endif
}

bool CryptHashPasswordCheck(string_view password, const void* pSalt, int32_t saltLen, const void* pHash, int32_t hashLen, int32_t iterations)
{
    bool matched = false;

    if (pSalt && static_cast<size_t>(saltLen) == CryptHashPassword2::SaltLen &&
        pHash && static_cast<size_t>(hashLen) == CryptHashPassword2::HashLen)
    {
        uint8_t hash[CryptHashPassword2::HashLen];

        if (CryptHashPassword(password, pSalt, saltLen, hash, hashLen, iterations))
        {
            matched = memcmp(pHash, hash, CryptHashPassword2::HashLen) == 0;
        }
    }

    return matched;
}

CryptHashPassword2::CryptHashPassword2(string_view password, int32_t iterations)
{
    set(password, iterations);
}

CryptHashPassword2::~CryptHashPassword2()
{
    SecureEraseMemory(salt, sizeof(salt));
    SecureEraseMemory(hash, sizeof(hash));
}

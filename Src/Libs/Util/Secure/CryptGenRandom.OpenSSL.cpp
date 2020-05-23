// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CryptGenRandom.OpenSSL.h"
#include <OpenSSL/rand.h>
#include "../Misc/Finally.h"

#pragma comment(lib, "libcrypto.lib")

bool CryptGenRandom_OpenSSL(void* pBuffer, size_t bufferLen)
{
    bool success = false;

    if (pBuffer && bufferLen)
    {
        success = RAND_bytes(reinterpret_cast<unsigned char*>(pBuffer), static_cast<int>(bufferLen)) == 1;
    }

    return success;
}

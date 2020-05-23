// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CryptGenRandom.Win32.h"
#include "../Misc/Finally.h"
#include "../System/_lean_windows_.h"
#include <Bcrypt.h>

#pragma comment(lib, "bcrypt.lib")

bool CryptGenRandom_Win32(void* pBuffer, size_t bufferLen)
{
    bool success = false;

    if (pBuffer && bufferLen)
    {
        BCRYPT_ALG_HANDLE hRng = nullptr;

        int status = BCryptOpenAlgorithmProvider(&hRng, BCRYPT_RNG_ALGORITHM, nullptr, 0);
        if (status >= 0)
        {
            Finally finally([&]()
            {
                BCryptCloseAlgorithmProvider(hRng, 0);
            });

            status = BCryptGenRandom(hRng, reinterpret_cast<PBYTE>(pBuffer), static_cast<ULONG>(bufferLen), 0);
            success = status >= 0;
        }
    }

    return success;
}

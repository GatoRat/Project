// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CryptHashPassword.Win32.h"
#include "../Misc/Finally.h"
#include "../System/_lean_windows_.h"
#include <Bcrypt.h>

using namespace std;

#if (NTDDI_VERSION >= NTDDI_WIN8)

bool CryptHashPassword_Win32(void* pBuffer, size_t bufferLen)
{
    bool success = false;
    
    if (pBuffer && bufferLen)
    {
/*
        BCRYPT_ALG_HANDLE hAlg = nullptr;

        int status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_PBKDF2_ALGORITHM, nullptr, 0);
        if (status >= 0)
        {
            Finally finally([&]()
            {
                BCryptCloseAlgorithmProvider(hAlg, 0);
            });

            internal int cbBuffer;
            internal int BufferType;
            internal IntPtr pvBuffer;       // PVOID
            
            static  
BCryptBuffer PBKDF2ParamBuffer[] = 
{ 
     { 
        sizeof(Salt), 
        KDF_SALT, 
        (PBYTE)Salt, 
    }, 
    { 
        sizeof(IterationCount), 
        KDF_ITERATION_COUNT, 
        (PBYTE)&IterationCount, 
    }, 
    { 
        sizeof(BCRYPT_SHA256_ALGORITHM), 
        KDF_HASH_ALGORITHM, 
        BCRYPT_SHA256_ALGORITHM, 
    } 
}; 

        struct BCryptBuffer[]
        {
            {
            internal int cbBuffer;
            internal int BufferType;
            internal IntPtr pvBuffer;       // PVOID
            }
        
            status = BCryptKeyDerivation(hAlg, KDF_GENERIC_PARAMETER
  BCryptBufferDesc  *pParameterList,
  PUCHAR            pbDerivedKey,
  ULONG             cbDerivedKey,
  ULONG             *pcbResult,
  0
);
            status = BCryptGenRandom(hRng, pBuffer, static_cast<ULONG>(bufferLen), 0);
            success = status >= 0;
        }
*/
    }

    return success;
}

#else

bool CryptHashPassword_Win32(void* /*pBuffer*/, size_t /*bufferLen*/)
{
    return false;
}

#endif
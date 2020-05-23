// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CryptSha.h"

#ifdef USE_WIN32_BCRYPT
    #include "CryptSha.Win32.h"
#else
    #include "CryptSha.OpenSSL.h"
#endif

using namespace std;

vector<uint8_t> CryptSha(const void* pData, size_t dataLen, bool use512)
{
#ifdef USE_WIN32_BCRYPT
    return CryptSha_Win32(pData, dataLen, use512);
#else
    return CryptSha_OpenSSL(pData, dataLen, use512);
#endif
}

bool CryptSha(const void* pData, size_t dataLen, uint8_t* pHash, size_t hashLen)
{
#ifdef USE_WIN32_BCRYPT
#else
    return CryptSha_OpenSSL(pData, dataLen, pHash, hashLen);
#endif
}

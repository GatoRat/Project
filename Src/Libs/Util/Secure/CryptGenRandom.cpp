// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CryptGenRandom.h"

#ifdef USE_WIN32_BCRYPT
    #include "CryptGenRandom.Win32.h"
#else
    #include "CryptGenRandom.OpenSSL.h"
#endif

using namespace std;

bool CryptGenRandom(void* pBuffer, size_t bufferLen)
{
#ifdef USE_WIN32_BCRYPT
    return CryptGenRandom_Win32(pBuffer, bufferLen);
#else
    return CryptGenRandom_OpenSSL(pBuffer, bufferLen);
#endif
}

bool CryptGenRandom(vector<uint8_t>& buffer)
{
    return CryptGenRandom(buffer.data(), buffer.size());
}

vector<uint8_t> CryptGenRandom(size_t bufferLen)
{
    vector<uint8_t> data;
    if (bufferLen)
    {
        data.resize(bufferLen);
        if (!CryptGenRandom(data))
        {
            data.clear();
        }
    }
    return data;
}

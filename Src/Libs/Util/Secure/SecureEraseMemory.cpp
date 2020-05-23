// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SecureEraseMemory.h"
#include "../Detect/DetectCompiler.h"
#if defined(_WIN32) && defined(__MSC__)
    #include "../System/_lean_windows_.h"
#endif

void SecureEraseMemory(void* pDst, size_t len) noexcept
{
    if (pDst && len)
    {
#if defined(_WIN32) && defined(__MSC__)
        RtlSecureZeroMemory(pDst, len);
#else
        volatile char* p = reinterpret_cast<volatile char*>(pDst);
        while (len--)
        {
            *p++ = 0;
        }
#endif
    }
}

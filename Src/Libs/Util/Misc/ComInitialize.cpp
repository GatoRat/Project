// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ComInitialize.h"
#ifdef _WIN32
    #include <objbase.h>
#endif

ComInitialize::ComInitialize()
{
#ifdef _WIN32
    CoInitialize(nullptr);
#endif
}

ComInitialize::~ComInitialize()
{
#ifdef _WIN32
    CoUninitialize();
#endif
}

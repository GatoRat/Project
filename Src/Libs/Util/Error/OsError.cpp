// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "OsError.h"
#include "../System/_lean_windows_.h"
#include "ErrString.h"

using namespace std;

OsError::OsError()
    : OsError(getCode())
{
}

OsError::OsError(uint32_t errCode)
    : code(errCode)
{
    setStr(code);
}

OsError::OsError(uint32_t errCode, const string& errStr)
    : code(errCode)
    , str(errStr)
{
}

const string& OsError::setStr(uint32_t errCode)
{
    auto errStr = GetErrStrOS(errCode); //-V106
    str = errStr.value_or(GetDefaultErrStr(errCode)); //-V106
    return str;
}

uint32_t OsError::getCode()
{
    return static_cast<uint32_t>(GetLastError());
}

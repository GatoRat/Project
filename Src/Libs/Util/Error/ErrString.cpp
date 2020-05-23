// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ErrString.h"
#include "../Detect/DetectCompiler.h"
#include "../Detect/DetectOperatingSystem.h"
#ifdef _WIN32
    #include <map>
    #include "../System/_lean_windows_.h"
    #include "Util/String/Convert.h"
#endif

using namespace std;

string GetDefaultErrStr(intptr_t code)
{
    return code ? /* "Error " +*/ to_string(code) : string();
}

optional<string> GetErrStr(intptr_t code)
{
    return GetDefaultErrStr(code);
}

optional<string> GetErrStrErrno(intptr_t code)
{
    // maxErrno will vary by compiler and platform
#ifdef __MSC__
    constexpr int maxErrno = 42;
#endif

    optional<string> str;

    if (code >= 0 && code <= maxErrno)
    {
        char buffer[256];
        if (strerror_s(buffer, static_cast<int>(code)) == 0)
        {
            str = buffer;
        }
    }

    return str;
}

#ifdef _WIN32

static map<intptr_t, const char*> shortErrorStrings =
{
    { ERROR_SUCCESS       , "Success"      },
    { ERROR_FILE_NOT_FOUND, "Missing file" },
    { ERROR_PATH_NOT_FOUND, "Invalid path" },
    { ERROR_TIMEOUT       , "Timed out"    }
};

optional<string> GetErrStrOS(intptr_t code)
{
    optional<string> errStr;

    const auto& it = shortErrorStrings.find(code);
    if (it != shortErrorStrings.end())
    {
        errStr = it->second;
    }
    else
    {
        string tmpStr;
        tmpStr.resize(512);

        const DWORD len = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS,
                                         nullptr, static_cast<DWORD>(code), MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
                                         &tmpStr[0], static_cast<DWORD>(tmpStr.size()), nullptr);
        if (len)
        {
            tmpStr.resize(len); //-V106
            errStr = tmpStr;
        }
    }

    return errStr;
}

optional<wstring> GetErrStrOSW(intptr_t code, const wchar_t* pDefault)
{
    optional<wstring> errStr;

    const auto& it = shortErrorStrings.find(code);
    if (it != shortErrorStrings.end())
    {
        errStr = Convert::ToWide(it->second);
    }
    else
    {
        wstring tmpStr;
        tmpStr.resize(512);

        const DWORD len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS,
                                         nullptr, static_cast<DWORD>(code), MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
                                         &tmpStr[0], static_cast<DWORD>(tmpStr.size()), nullptr);
        if (len)
        {
            tmpStr.resize(len); //-V106
            errStr = tmpStr;
        }
        else if (pDefault && *pDefault)
        {
            errStr = pDefault;
        }
    }

    return errStr;
}

#else

optional<string> GetErrStrOS(intptr_t code)
{
    return GetErrStrErrno(code);
}

#endif

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "IsValidFilenameChar.h"

using namespace std;

static bool IsAscii(char ch)
{
    return ch >= 32 && ch < 127; //-V112
}

static bool IsAscii(wchar_t ch)
{
    return ch >= 32 && ch < 127;
}

static const char* pInvalidCharsWin32 = "\"*/:<>?\\|";

bool IsValidFilenameCharWin32(char ch, bool asciiOnly)
{
    return (asciiOnly ? IsAscii(ch) : ch < 0 || ch >= 32) && strchr(pInvalidCharsWin32, ch) == nullptr; //-V112
}

#ifdef _WIN32

#include <cstring>

static const wchar_t* pInvalidCharsW = L"\"*/:<>?\\|";

bool IsValidFilenameChar(char ch, bool asciiOnly)
{
    return IsValidFilenameCharWin32(ch, asciiOnly);
}

bool IsValidFilenameChar(wchar_t ch, bool asciiOnly)
{
    return (asciiOnly ? IsAscii(ch) : /*ch < 0 ||*/ ch >= 32) && wcschr(pInvalidCharsW, ch) == nullptr;
}

#elif defined(__linux__)

bool IsValidFilenameChar(char ch, bool asciiOnly)
{
    return (asciiOnly ? IsAscii(ch) : ch != 0) && ch != '/';
}

bool IsValidFilenameChar(wchar_t ch, bool asciiOnly)
{
    return (asciiOnly ? IsAscii(ch) : ch != 0) && ch != L'/';
}

#endif // _WIN32

bool IsValidFilenameStr(string_view str, bool asciiOnly)
{
    bool isValid = true;

    for (auto ch : str)
    {
        if (!IsValidFilenameChar(ch, asciiOnly))
        {
            isValid = false;
            break;
        }
    }

    return isValid;
}

bool IsValidFilenameStr(wstring_view str, bool asciiOnly)
{
    bool isValid = true;

    for (auto ch : str)
    {
        if (!IsValidFilenameChar(ch, asciiOnly))
        {
            isValid = false;
            break;
        }
    }

    return isValid;
}

bool IsValidFilenameStrAllPlatforms(string_view str, bool asciiOnly)
{
    bool isValid = true;

    for (auto ch : str)
    {
        if (!IsValidFilenameCharWin32(ch, asciiOnly))
        {
            isValid = false;
            break;
        }
    }

    return isValid;
}

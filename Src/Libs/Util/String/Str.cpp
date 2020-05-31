// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
    The author disclaims copyright to this source code.

    Permission to use, copy, modify, and/or distribute this software for any purpose
    with or without fee is hereby granted, provided that the above copyright notice
    and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
    REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
    FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT,
    OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
    DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
    ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "Str.h"
#include <algorithm>
#include <cctype>
#include <cwctype>
#include <climits>
#include <cstring>
#include "ASCII.h"
#include "ASCIIW.h"

using namespace std;

/*
    Note: isspace is a travesty. It returns an int and under MSVC, it asserts if the value is negative.
*/

namespace Str
{
    static int CompareUnsafe(const char* pStr1, const char* pStr2, bool ignoreCase, size_t len);
    static int CompareUnsafe(const wchar_t* pStr1, const wchar_t* pStr2, bool ignoreCase, size_t len);

    bool IsNullOrWhiteSpace(const char* pStr)
    {
        bool success = true;
        if (!IsNullOrEmpty(pStr))
        {
            while (*pStr && success)
            {
                success = ASCII::IsWhitespace(*pStr++);
            }
        }
        return success;
    }

    bool IsEmptyOrWhiteSpace(const string& str)
    {
        bool success = true;
        if (!str.empty())
        {
            success = all_of(str.begin(), str.end(), [](char ch) -> bool { return ASCII::IsWhitespace(ch); });
        }
        return success;
    }

    bool IsEmptyOrWhiteSpace(const string_view& str)
    {
        bool success = true;
        if (!str.empty())
        {
            success = all_of(str.begin(), str.end(), [](char ch) -> bool { return ASCII::IsWhitespace(ch); });
        }
        return success;
    }

    bool IsNullOrWhiteSpace(const wchar_t* pStr)
    {
        bool success = true;
        if (!IsNullOrEmpty(pStr))
        {
            while (*pStr && success)
            {
                success = ASCIIW::IsWhitespace(*pStr++);
            }
        }
        return success;
    }

    bool IsEmptyOrWhiteSpace(const wstring& str)
    {
        bool success = true;
        if (!str.empty())
        {
            success = all_of(str.begin(), str.end(), [](wchar_t ch) -> bool
            {
                return ASCIIW::IsWhitespace(ch);
            });
        }
        return success;
    }

    bool IsEmptyOrWhiteSpace(const wstring_view& str)
    {
        bool success = true;
        if (!str.empty())
        {
            success = all_of(str.begin(), str.end(), [](wchar_t ch) -> bool
            {
                return ASCIIW::IsWhitespace(ch);
            });
        }
        return success;
    }

    size_t GetLenMax(const char* pStr, size_t maxLen)
    {
        size_t strLen = 0;

        if (pStr)
        {
            while (strLen < maxLen && pStr[strLen])
            {
                ++strLen;
            }
        }

        return strLen;
    }

    //size_t GetLenMax(const char8_t* pStr, size_t maxLen)
    //{
    //    size_t strLen = 0;

    //    if (pStr)
    //    {
    //        while (strLen < maxLen && pStr[strLen])
    //        {
    //            ++strLen;
    //        }
    //    }

    //    return strLen;
    //}

    size_t GetLenMax(const wchar_t* pStr, size_t maxLen)
    {
        size_t strLen = 0;

        if (pStr)
        {
            while (strLen < maxLen && pStr[strLen])
            {
                ++strLen;
            }
        }

        return strLen;
    }

    ///////////////////////////

    const char* pWhitespace = " \t\r\n\v\f";

    string_view TrimLeft(const char* pStr)
    {
        if (pStr)
        {
            while (ASCII::IsWhitespace(*pStr))
            {
                pStr++;
            }
        }
        else
        {
            static char emptyStr = 0;
            pStr = &emptyStr;
        }

        return string_view(pStr);
    }

    string_view TrimRight(const char* pStr) //-V2506
    {
        if (pStr)
        {
            size_t len = strlen(pStr);

            while (len > 0 && ASCII::IsWhitespace(pStr[len - 1]))
            {
                len--;
            }

            return string_view(pStr, len);
        }
        else
        {
            static char emptyStr = 0;
            return string_view(&emptyStr, 0);
        }
    }

    string_view Trim(const char* pStr) //-V2506
    {
        if (pStr)
        {
            while (ASCII::IsWhitespace(*pStr))
            {
                pStr++;
            }

            size_t len = strlen(pStr);

            if (len)
            {
                while (ASCII::IsWhitespace(pStr[len - 1]))
                {
                    --len;
                }
            }

            return string_view(pStr, len);
        }
        else
        {
            static char emptyStr = 0;
            return string_view(&emptyStr, 0);
        }
    }

    string_view TrimLeftView(string_view str)
    {
        const size_t prefixLen = distance(str.begin(), find_if_not(str.begin(), str.end(), [](char ch)->bool
        {
            return ASCII::IsWhitespace(ch);
        }));
        str.remove_prefix(prefixLen);
        return str;
    }

    string_view TrimRightView(string_view str)
    {
        const size_t suffixLen = distance(find_if_not(str.rbegin(), str.rend(), [](char ch)->bool
        {
            return ASCII::IsWhitespace(ch);
        }).base(), str.end());
        str.remove_suffix(suffixLen);
        return str;
    }

    string_view TrimLeftView(string_view str, const char* pCharsToTrim)
    {
        if (!pCharsToTrim)
        {
            str = TrimLeftView(str);
        }
        else if (*pCharsToTrim)
        {
            const size_t prefixLen = distance(str.begin(), find_if_not(str.begin(), str.end(), [&](char ch)->bool
            {
                return strchr(pCharsToTrim, ch) != nullptr;
            }));
            str.remove_prefix(prefixLen);
        }
        return str;
    }

    string_view TrimRightView(string_view str, const char* pCharsToTrim)
    {
        if (!pCharsToTrim)
        {
            str = TrimRightView(str);
        }
        else if (*pCharsToTrim)
        {
            const size_t suffixLen = distance(find_if_not(str.rbegin(), str.rend(), [&](int ch)->bool
            {
                return strchr(pCharsToTrim, ch) != nullptr;
            }).base(), str.end());
            str.remove_suffix(suffixLen);
        }

        return str;
    }

    string& TrimLeft(string& str, const char* pCharsToTrim)
    {
        if (!pCharsToTrim)
        {
            str.erase(str.begin(), find_if_not(str.begin(), str.end(), [](char ch)->bool
            {
                return ASCII::IsWhitespace(ch);
            }));
        }
        else if (*pCharsToTrim)
        {
            str.erase(str.begin(), find_if_not(str.begin(), str.end(), [&](int ch)->bool
            {
                return strchr(pCharsToTrim, ch) != nullptr;
            }));
        }
        return str;
    }

    string& TrimRight(string& str, const char* pCharsToTrim)
    {
        if (!pCharsToTrim)
        {
            str.erase(find_if_not(str.rbegin(), str.rend(), [](char ch)->bool
            {
                return ASCII::IsWhitespace(ch);
            }).base(), str.end());
        }
        else if (*pCharsToTrim)
        {
            str.erase(find_if_not(str.rbegin(), str.rend(), [&](int ch)->bool
            {
                return strchr(pCharsToTrim, ch) != nullptr;
            }).base(), str.end());
        }
        return str;
    }

    ///////////////////////////

    bool StartsWith(string_view strToCheck, string_view strToCompare, bool ignoreCase)
    {
        bool success = false;
        if (!strToCompare.empty() && strToCompare.size() <= strToCheck.size())
        {
            success = CompareUnsafe(strToCheck.data(), strToCompare.data(), ignoreCase, strToCompare.size()) == 0;
        }
        return success;
    }

    bool StartsWith(wstring_view strToCheck, wstring_view strToCompare, bool ignoreCase)
    {
        bool success = false;
        if (!strToCompare.empty() && strToCompare.size() <= strToCheck.size())
        {
            success = CompareUnsafe(strToCheck.data(), strToCompare.data(), ignoreCase, strToCompare.size()) == 0;
        }
        return success;
    }

    bool EndsWith(string_view str, string_view strToCompare, bool ignoreCase)
    {
        bool success = false;
        if (!str.empty() && !strToCompare.empty())
        {
            if (str.size() >= strToCompare.size())
            {
                const char* pStrToCheck = str.data() + (str.size() - strToCompare.size());
                success = CompareUnsafe(pStrToCheck, strToCompare.data(), ignoreCase, strToCompare.size()) == 0;
            }
        }
        return success;
    }

    bool EndsWith(wstring_view str, wstring_view strToCompare, bool ignoreCase)
    {
        bool success = false;
        if (!str.empty() && !strToCompare.empty())
        {
            if (str.size() >= strToCompare.size())
            {
                const wchar_t* pStrToCheck = str.data() + (str.size() - strToCompare.size());
                success = CompareUnsafe(pStrToCheck, strToCompare.data(), ignoreCase, strToCompare.size()) == 0;
            }
        }
        return success;
    }

    ///////////////////////////

    int GetEolLen(const char* pStr) //-V2506
    {
        if (pStr)
        {
            if (pStr[0] == ASCII::LF)
            {
                return 1;
            }

            if (pStr[0] == ASCII::CR)
            {
                return pStr[1] == ASCII::LF ? 2 : 1;
            }
        }
        return 0;
    }

    LineColumn::LineColumn(const char* pStr, size_t offset)
        : line  { 0 }
        , column{ 0 }
    {
        if (pStr && *pStr)
        {
            for (size_t curOffset = 0; curOffset < offset;)
            {
                if (pStr[curOffset] == ASCII::LF)
                {
                    if (curOffset++ == offset) //-V547
                    {
                        break;
                    }

                    column = 0;
                    ++line;
                }
                else if (pStr[curOffset] == ASCII::CR)
                {
                    if (pStr[curOffset + 1] == ASCII::LF)
                    {
                        ++curOffset;
                    }

                    if (curOffset++ == offset)
                    {
                        break;
                    }

                    column = 0;
                    ++line;
                }
                else if (pStr[curOffset])
                {
                    ++curOffset;
                    ++column;
                }
                else
                {
                    break;
                }
            }

            ++line;
            ++column;
        }
    }

    LineColumn::LineColumn(string_view str, size_t offset)
        : line  { 0 }
        , column{ 0 }
    {
        if (!str.empty() && offset < str.length())
        {
            for (size_t curOffset = 0; curOffset < offset;)
            {
                if (str[curOffset] == ASCII::LF)
                {
                    if (curOffset++ == offset) //-V547
                    {
                        break;
                    }

                    column = 0;
                    ++line;
                }
                else if (str[curOffset] == ASCII::CR)
                {
                    if (str[curOffset + 1] == ASCII::LF)
                    {
                        ++curOffset;
                    }

                    if (curOffset++ == offset)
                    {
                        break;
                    }

                    column = 0;
                    ++line;
                }
                else if (str[curOffset])
                {
                    ++curOffset;
                    ++column;
                }
                else
                {
                    break;
                }
            }

            ++line;
            ++column;
        }
    }

    ///////////////////////////////////////////////////////////////////////

    string& RemoveAllNonPrintableChars(string& str)
    {
        if (!str.empty())
        {
            string result;
            result.reserve(str.size());
            for (char ch : str)
            {
                if (ASCII::IsPrintable(ch))
                {
                    result += ch;
                }
            }

            if (result.size() < str.size())
            {
                str = result;
            }
        }
        return str;
    }

    string& ReplaceAllNonPrintableChars(string& str, char ch)
    {
        if (!str.empty())
        {
            for (size_t i = 0; i < str.size(); ++i)
            {
                if (!ASCII::IsPrintable(ch))
                {
                    str[i] = ch;
                }
            }
        }
        return str;
    }

    string& RemoveAll(string& str, string_view find, size_t* pCount)
    {
        if (pCount)
        {
            *pCount = 0;
        }

        if (!str.empty() && !find.empty())
        {
            for (string::size_type i = 0; (i = str.find(find, i)) != string::npos;)
            {
                str.erase(i, find.size());
                if (pCount)
                {
                    ++(*pCount);
                }
            }
        }
        return str;
    }

    string& ReplaceAll(string& str, string_view find, string_view replace, size_t* pCount)
    {
        if (pCount)
        {
            *pCount = 0;
        }

        if (replace.empty())
        {
            str = RemoveAll(str, find, pCount);
        }
        else if (!str.empty() && !find.empty())
        {
            for (string::size_type i = 0; (i = str.find(find, i)) != string::npos; i += replace.size())
            {
                str.replace(i, find.size(), replace);
                if (pCount)
                {
                    ++(*pCount);
                }
            }
        }
        return str;
    }

    size_t Copy(char* pDst, const char* pSrc, size_t maxLen, bool fill, bool* pErr)
    {
        if (pErr)
        {
            *pErr = false;
        }

        if (pDst && maxLen > 0)
        {
            if (pSrc)
            {
                const size_t copyLen = maxLen - 1;

                for (maxLen = 0; maxLen < copyLen && pSrc[maxLen]; maxLen++)
                {
                    pDst[maxLen] = pSrc[maxLen];
                }

                if (pErr && maxLen == copyLen && pSrc[maxLen])
                {
                    *pErr = true;
                }

                if (fill)
                {
                    memset(&pDst[maxLen], 0, copyLen - maxLen + 1);
                }
                else
                {
                    pDst[maxLen] = 0;
                }
            }
            else
            {
                if (fill)
                {
                    memset(pDst, 0, maxLen);
                }

                maxLen = 0;
            }
        }
        else
        {
            if (pErr)
            {
                *pErr = maxLen > 0;
            }
            maxLen = 0;
        }

        return maxLen;
    }

    string Right(const string& str, size_t maxChars)
    {
        return str.size() < maxChars ? str : str.substr(str.size() - maxChars);
    }

    string_view RightView(const string_view& str, size_t maxChars)
    {
        return str.size() < maxChars ? str : string_view(str.data() + str.size() - maxChars);
    }

    ///////////////////////////

    string Concat(const char* pStr1, const char* pStr2)
    {
        string result(pStr1 ? pStr1 : "");
        if (pStr2)
        {
            result += pStr2;
        }
        return result;
    }

    size_t Concat(char* pDst, const char* pSrc, size_t maxLen)
    {
        if (pDst && maxLen > 0)
        {
            const size_t orgLen = strlen(pDst);
            if (maxLen > orgLen)
            {
                maxLen = orgLen + Copy(&pDst[orgLen], pSrc, maxLen - orgLen);
            }
            else
            {
                pDst[--maxLen] = 0;
            }
        }
        else
        {
            maxLen = 0;
        }

        return maxLen;
    }

    ///////////////////////////

    string ToUpper(const char* pStr, bool trimLeft, bool trimRight)
    {
        string str;

        if (!IsNullOrEmpty(pStr))
        {
            if (trimLeft)
            {
                while (ASCII::IsWhitespace(*pStr))
                {
                    pStr++;
                }
            }

            size_t len = strlen(pStr);

            if (trimRight)
            {
                while (len > 0 && ASCII::IsWhitespace(pStr[len - 1]))
                {
                    len--;
                }
            }

            str.reserve(len);
            for (size_t i = 0; i < len; i++)
            {
                str.push_back(ASCII::ToUpper(pStr[i]));
            }
        }

        return str;
    }

    string ToUpper(const string& str)
    {
        string result(str);
        ToUpperInPlace(result);
        return result;
    }

    string& ToUpperInPlace(string& str)
    {
        transform(str.begin(), str.end(), str.begin(), [&](char ch) -> char { return ASCII::ToUpper(ch); });
        return str;
    }

    string ToLower(const char* pStr, bool trimLeft, bool trimRight)
    {
        string str;

        if (!IsNullOrEmpty(pStr))
        {
            if (trimLeft)
            {
                while (ASCII::IsWhitespace(*pStr))
                {
                    pStr++;
                }
            }

            size_t len = strlen(pStr);

            if (trimRight)
            {
                while (len > 0 && ASCII::IsWhitespace(pStr[len - 1]))
                {
                    len--;
                }
            }

            str.reserve(len);
            for (size_t i = 0; i < len; i++)
            {
                str.push_back(ASCII::ToLower(pStr[i]));
            }
        }

        return str;
    }

    string ToLower(const string& str)
    {
        string result(str);
        ToLowerInPlace(result);
        return result;
    }

    string& ToLowerInPlace(string& str)
    {
        transform(str.begin(), str.end(), str.begin(), [&](char ch) -> char { return ASCII::ToLower(ch); });
        return str;
    }

    bool FindNoCase(string_view stringToFind, string_view stringToSearch)
    {
        auto it = search(stringToSearch.begin(), stringToSearch.end(),
                         stringToFind.begin(), stringToFind.end(),
                         [](char ch1, char ch2)
        {
            return toupper(ch1) == toupper(ch2);
        });

        return it != stringToSearch.end();
    }

    bool FindNoCase(wstring_view stringToFind, wstring_view stringToSearch)
    {
        auto it = search(stringToSearch.begin(), stringToSearch.end(),
                         stringToFind.begin(), stringToFind.end(),
                         [](wchar_t ch1, wchar_t ch2)
        {
            return towupper(ch1) == towupper(ch2);
        });

        return it != stringToSearch.end();
    }

    ///////////////////////////

    static int CompareUnsafe(const char* pStr1, const char* pStr2, bool ignoreCase, size_t len)
    {
        int result = 0;

        if (ignoreCase)
        {
            if (len)
            {
#ifdef _WIN32
                result = _strnicmp(pStr1, pStr2, len);
#else
                result = strncasecmp(pStr1, pStr2, len);
#endif
            }
            else
            {
#ifdef _WIN32
                result = _stricmp(pStr1, pStr2);
#else
                result = strcasecmp(pStr1, pStr2);
#endif
            }
        }
        else
        {
            if (len)
            {
                result = strncmp(pStr1, pStr2, len);
            }
            else
            {
                result = strcmp(pStr1, pStr2);
            }
        }

        return result;
    }

    int Compare(const char* pStr1, const char* pStr2, bool ignoreCase, size_t len)
    {
        int result = 0;

        if (pStr1)
        {
            if (pStr2)
            {
                result = CompareUnsafe(pStr1, pStr2, ignoreCase, len);
            }
            else
            {
                result = INT_MAX;
            }
        }
        else if (pStr2)
        {
            result = INT_MIN;
        }

        return result;
    }

    bool AreEqual(const char* pStr1, const char* pStr2, bool failOnNull, bool ignoreCase)
    {
        bool success = false;

        if (pStr1)
        {
            if (pStr2)
            {
                if (pStr1 == pStr2)
                {
                    success = true;
                }
                else
                {
                    success = CompareUnsafe(pStr1, pStr2, ignoreCase, 0) == 0;
                }
            }
            else
            {
                success = false;
            }
        }
        else if (!failOnNull)
        {
            success = pStr1 == pStr2;
        }

        return success;
    }

    int CompareV(string_view str1, string_view str2, bool ignoreCase, size_t len)
    {
        return CompareV2(str1, str2, ignoreCase, len);
    }

    int CompareV2(const string_view& str1, const string_view& str2, bool ignoreCase, size_t len)
    {
        int result = 0;

        if (len == 0)
        {
            if (str1.size() > str2.size())
            {
                result = CompareUnsafe(str1.data(), str2.data(), ignoreCase, str2.size());
                if (result == 0)
                {
                    result = INT_MAX;
                }
            }
            else
            {
                result = CompareUnsafe(str1.data(), str2.data(), ignoreCase, str1.size());
                if (result == 0 && str1.size() < str2.size())
                {
                    result = INT_MIN;
                }
            }
        }
        else if (str1.size() > str2.size())
        {
            result = CompareUnsafe(str1.data(), str2.data(), ignoreCase, len < str2.size() ? len : str2.size());
            if (result == 0 && len > str2.size())
            {
                result = INT_MAX;
            }
        }
        else
        {
            result = CompareUnsafe(str1.data(), str2.data(), ignoreCase, len < str1.size() ? len : str1.size());
            if (result == 0 && str1.size() < str2.size() && len > str1.size())
            {
                result = INT_MIN;
            }
        }

        return result;
    }

    ///////////////////////////

    static int CompareUnsafe(const wchar_t* pStr1, const wchar_t* pStr2, bool ignoreCase, size_t len)
    {
        int result = 0;

        if (ignoreCase)
        {
            if (len)
            {
#ifdef _WIN32
                result = _wcsnicmp(pStr1, pStr2, len);
#else
                result = wcsncasecmp(pStr1, pStr2, len);
#endif
            }
            else
            {
#ifdef _WIN32
                result = _wcsicmp(pStr1, pStr2);
#else
                result = wcscasecmp(pStr1, pStr2);
#endif
            }
        }
        else
        {
            if (len)
            {
                result = wcsncmp(pStr1, pStr2, len);
            }
            else
            {
                result = wcscmp(pStr1, pStr2);
            }
        }

        return result;
    }

    int Compare(const wchar_t* pStr1, const wchar_t* pStr2, bool ignoreCase, size_t len)
    {
        int result = 0;

        if (pStr1)
        {
            if (pStr2)
            {
                result = CompareUnsafe(pStr1, pStr2, ignoreCase, len);
            }
            else
            {
                result = INT_MAX;
            }
        }
        else if (pStr2)
        {
            result = INT_MIN;
        }

        return result;
    }

    int CompareV(wstring_view str1, wstring_view str2, bool ignoreCase, size_t len)
    {
        int result = 0;

        if (len == 0)
        {
            if (str1.size() > str2.size())
            {
                result = CompareUnsafe(str1.data(), str2.data(), ignoreCase, str2.size());
                if (result == 0)
                {
                    result = INT_MAX;
                }
            }
            else
            {
                result = CompareUnsafe(str1.data(), str2.data(), ignoreCase, str1.size());
                if (result == 0 && str1.size() < str2.size())
                {
                    result = INT_MIN;
                }
            }
        }
        else if (str1.size() > str2.size())
        {
            result = CompareUnsafe(str1.data(), str2.data(), ignoreCase, len < str2.size() ? len : str2.size());
            if (result == 0 && len > str2.size())
            {
                result = INT_MAX;
            }
        }
        else
        {
            result = CompareUnsafe(str1.data(), str2.data(), ignoreCase, len < str1.size() ? len : str1.size());
            if (result == 0 && str1.size() < str2.size() && len > str1.size())
            {
                result = INT_MIN;
            }
        }

        return result;
    }

    string NormalizeString(const char* pStr)
    {
        string result;

        if (pStr)
        {
            while (ASCII::IsWhitespace(*pStr) != 0)
            {
                pStr++;
            }

            size_t len = strlen(pStr);

            if (len)
            {
                while (ASCII::IsWhitespace(pStr[len - 1]) != 0)
                {
                    len--;
                }

                if (len)
                {
                    result.reserve(len);
                    result += pStr[0];

                    for (size_t i = 1; i < len;)
                    {
                        const char ch = pStr[i++];
                        if (ASCII::IsWhitespace(ch))
                        {
                            result += ASCII::Space;
                            while (i < len && ASCII::IsWhitespace(pStr[i]))
                            {
                                ++i;
                            }

                            if (i < len)
                            {
                                result += pStr[i++];
                            }
                        }
                        else
                        {
                            result += ch;
                        }
                    }
                }
            }
        }

        return result;
    }

    string NormalizeString(string_view str)
    {
        const size_t prefixLen = distance(str.begin(), find_if_not(str.begin(), str.end(), [](char ch) -> bool { return ASCII::IsWhitespace(ch); }));

        str.remove_prefix(prefixLen);

        string result;

        if (!str.empty())
        {
            result.reserve(str.size());

            bool inWhitespace = false;

            for (char ch : str)
            {
                if (ASCII::IsWhitespace(ch))
                {
                    if (!inWhitespace)
                    {
                        inWhitespace = true;
                        result += ASCII::Space;
                    }
                }
                else
                {
                    inWhitespace = false;
                    result += ch;
                }
            }

            if (inWhitespace)
            {
                result.pop_back();
            }
        }

        return result;
    }

    string ToQuotedString(const char* pStr)
    {
        string str;

        if (pStr)
        {
            str += ASCII::DoubleQuote;
            str += pStr;
            str += ASCII::DoubleQuote;
        }
        else
        {
            str = "<null>";
        }

        return str;
    }

    string ToQuotedString2(string_view str)
    {
        string quotedStr;
        quotedStr.reserve(str.size() + 2);

        quotedStr += ASCII::DoubleQuote;
        quotedStr += str;
        quotedStr += ASCII::DoubleQuote;

        return quotedStr;
    }

    string Format(string_view formatString,
        const char* pStr0, const char* pStr1, const char* pStr2,
        const char* pStr3, const char* pStr4, const char* pStr5)
    {
        string formattedStr;
        FormatAppend(formattedStr, formatString, pStr0, pStr1, pStr2, pStr3, pStr4, pStr5);
        return formattedStr;
    }

    string& FormatAppend(string& strToAppend, string_view formatString,
        const char* pStr0, const char* pStr1, const char* pStr2,
        const char* pStr3, const char* pStr4, const char* pStr5)
    {
        if (!formatString.empty())
        {
            string_view strings[]
            {
                pStr0 ? pStr0 : "",
                pStr1 ? pStr1 : "",
                pStr2 ? pStr2 : "",
                pStr3 ? pStr3 : "",
                pStr4 ? pStr4 : "",
                pStr5 ? pStr5 : ""
            };

            size_t reserveLen = strToAppend.size() + formatString.size();
            for (auto& str : strings)
            {
                reserveLen += str.size();
            }

            strToAppend.reserve(reserveLen);

            for (size_t i = 0; i < formatString.size();)
            {
                if (formatString[i] == '{' &&
                    i + 2 < formatString.size() &&
                    formatString[i + 2] == '}' &&
                    formatString[i + 1] >= '0' &&
                    formatString[i + 1] <= '5')
                {
                    size_t index = static_cast<size_t>(formatString[i + 1] - '0');
                    strToAppend += strings[index];
                    i += 3;
                }
                else
                {
                    strToAppend += formatString[i++];
                }
            }
        }

        return strToAppend;
    }

    string Format3(string_view formatString, string_view string0, string_view string1, string_view string2)
    {
        string formattedStr;

        if (!formatString.empty())
        {
            formattedStr.reserve(formatString.size() + string0.size() + string1.size() + string2.size());

            for (size_t i = 0; i < formatString.size();)
            {
                if (formatString[i] == '{' &&
                    i + 2 < formatString.size() &&
                    formatString[i + 2] == '}' &&
                    formatString[i + 1] >= '0' &&
                    formatString[i + 1] <= '2')
                {
                    switch (formatString[i + 1])
                    {
                        case '0':
                            formattedStr += string0;
                            break;
                        case '1':
                            formattedStr += string1;
                            break;
                        case '2':
                            formattedStr += string2;
                            break;
                    }
                    i += 3;
                }
                else
                {
                    formattedStr += formatString[i++];
                }
            }
        }

        return formattedStr;
    }

    string FormatV(string_view formatString, const vector<string_view>& strings, size_t reserveExtra)
    {
        string formattedStr;

        if (!formatString.empty())
        {
            reserveExtra += formatString.size();

            for (auto& string : strings)
            {
                reserveExtra += string.size();
            }

			formattedStr.reserve(reserveExtra);

            for (size_t i = 0; i < formatString.size();)
            {
                if (formatString[i] == '{' &&
                    i + 2 < formatString.size() &&
                    formatString[i + 2] == '}' &&
                    formatString[i + 1] >= '0' &&
                    formatString[i + 1] <= '9')
                {
                    auto stringIndex = ASCII::DigitToIndex(formatString[i + 1]);
                    if (stringIndex < strings.size())
                    {
                        formattedStr += strings[stringIndex];
                    }
                    else
                    {
                        formattedStr.append(&formatString[i], 3);
                    }
                    i += 3;
                }
                else
                {
                    formattedStr += formatString[i++];
                }
            }
        }

        return formattedStr;
    }

} // namespace Str

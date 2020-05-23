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

#pragma once

#include <string>
#include <string_view>
#include <vector>

/// <summary>
/// String functions.
/// </summary>
namespace Str
{
    /// <summary>
    /// Indicates whether the passed string is null.
    /// </summary>
    /// <param name="pStr">The string to check.</param>
    /// <returns><b>true</b> if pStr is null, else <b>false</b>.</returns>
    inline bool IsNull(const void* pStr)
    {
        return !pStr;
    }

    /// <summary>
    /// Indicates whether the passed string is null or empty.
    /// </summary>
    /// <param name="pStr">The string to check.</param>
    /// <returns><b>true</b> if pStr is null or empty, else <b>false</b>.</returns>
    inline bool IsNullOrEmpty(const char* pStr)
    {
        return !pStr || !*pStr;
    }

    /// <summary>
    /// Indicates whether the passed string is null, empty or contains only whitespace.
    /// </summary>
    /// <param name="pStr">The string to check.</param>
    /// <returns><b>true</b> if pStr is null, empty or contains only whitespace,
    ///     else <b>false</b>.</returns>
    bool IsNullOrWhiteSpace(const char* pStr);

    /// <summary>
    /// Indicates whether the passed string is null, empty or contains only whitespace.
    /// </summary>
    /// <param name="pStr">The string to check.</param>
    /// <returns><b>true</b> if pStr is null, empty or contains only whitespace,
    ///     else <b>false</b>.</returns>
    inline bool IsEmptyOrWhiteSpace(const char* pStr)
    {
        return IsNullOrWhiteSpace(pStr);
    }

    /// <summary>
    /// Indicates whether the passed string is null or empty.
    /// </summary>
    /// <param name="pStr">The string to check.</param>
    /// <returns><b>true</b> if pStr is null or empty, else <b>false</b>.</returns>
    inline bool IsNullOrEmpty(const wchar_t* pStr)
    {
        return !pStr || !*pStr;
    }

    /// <summary>
    /// Indicates whether the passed string is null, empty or contains only whitespace.
    /// </summary>
    /// <param name="pStr">The string to check.</param>
    /// <returns><b>true</b> if pStr is null, empty or contains only whitespace,
    ///     else <b>false</b>.</returns>
    bool IsNullOrWhiteSpace(const wchar_t* pStr);

    /// <summary>
    /// Indicates whether the passed string is empty or contains only whitespace.
    /// </summary>
    /// <param name="str">The string to check.</param>
    /// <returns><b>true</b> if str is empty or contains only whitespace,
    ///     else <b>false</b>.</returns>
    bool IsEmptyOrWhiteSpace(const std::string& str);

    /// <summary>
    /// Indicates whether the passed string is empty or contains only whitespace.
    /// </summary>
    /// <param name="str">The string to check.</param>
    /// <returns><b>true</b> if str is empty or contains only whitespace,
    ///     else <b>false</b>.</returns>
    bool IsEmptyOrWhiteSpace(const std::string_view& str);

    /// <summary>
    /// Indicates whether the passed string is empty or contains only whitespace.
    /// </summary>
    /// <param name="str">The string to check.</param>
    /// <returns><b>true</b> if str is empty or contains only whitespace,
    ///     else <b>false</b>.</returns>
    bool IsEmptyOrWhiteSpace(const std::wstring& str);

    /// <summary>
    /// Indicates whether the passed string is empty or contains only whitespace.
    /// </summary>
    /// <param name="str">The string to check.</param>
    /// <returns><b>true</b> if str is empty or contains only whitespace,
    ///     else <b>false</b>.</returns>
    bool IsEmptyOrWhiteSpace(const std::wstring_view& str);

    /// <summary>
    /// Safetly gets the length of a string.
    /// </summary>
    /// <param name="pStr">The string to use.</param>
    /// <returns>The length of pStr. If pStr is nullptr, zero (0) is returned.</returns>
    inline size_t GetLen(const char* pStr)
    {
        return pStr ? strlen(pStr) : 0;
    }

    size_t GetLenMax(const char* pStr, size_t maxLen);
//    size_t GetLenMax(const char8_t* pStr, size_t maxLen);
    size_t GetLenMax(const wchar_t* pStr, size_t maxLen);

    inline char Front(std::string_view str, char defaultChar = 0)
    {
        return str.empty() ? defaultChar : str.front();
    }

    inline std::string& ReserveAtLeast(std::string& str, size_t len)
    {
        if (str.capacity() < len)
        {
            str.reserve(str.capacity() + len);
        }
        return str;
    }

    inline std::string& ReserveAdditional(std::string& str, size_t len)
    {
        return ReserveAtLeast(str, str.size() + len);
    }

    extern const char* pWhitespace;

    /// <summary>
    /// Trims whitespace from the start of a string.
    /// </summary>
    /// <param name="pStr">The string to trim.</param>
    /// <returns>The trimmed string.</returns>
    std::string_view TrimLeft(const char* pStr);

    /// <summary>
    /// Trims whitespace from the end of a string.
    /// </summary>
    /// <param name="pStr">The string to trim.</param>
    /// <returns>The trimmed string.</returns>
    std::string_view TrimRight(const char* pStr);

    /// <summary>
    /// Trims whitespace from the start and end of a string.
    /// </summary>
    /// <param name="pStr">The string to trim.</param>
    /// <returns>The trimmed string.</returns>
    std::string_view Trim(const char* pStr);

    /// <summary>
    /// Trims whitespace from the start of a string.
    /// </summary>
    /// <param name="pStr">The string to trim.</param>
    /// <returns>The trimmed string.</returns>
    std::string_view TrimLeftView(std::string_view str);

    /// <summary>
    /// Trims whitespace from the end of a string.
    /// </summary>
    /// <param name="pStr">The string to trim.</param>
    /// <returns>The trimmed string.</returns>
    std::string_view TrimRightView(std::string_view str);

    /// <summary>
    /// Trims whitespace from the start and end of a string.
    /// </summary>
    /// <param name="pStr">The string to trim.</param>
    /// <returns>The trimmed string.</returns>
    inline std::string_view TrimView(std::string_view str)
    {
        return TrimLeftView(TrimRightView(str));
    }

    /// <summary>
    /// Trims whitespace from the start of a string.
    /// </summary>
    /// <param name="pStr">The string to trim.</param>
    /// <param name="pCharsToTrim">A string containing the characters to trim.
    ///     If nullptr, ASCII::IsWhitespace() is used. If empty, nothing happens.</param>
    /// <returns>The trimmed string.</returns>
    std::string_view TrimLeftView(std::string_view str, const char* pCharsToTrim);

    /// <summary>
    /// Trims whitespace from the end of a string.
    /// </summary>
    /// <param name="pStr">The string to trim.</param>
    /// <param name="pCharsToTrim">A string containing the characters to trim.
    ///     If nullptr, ASCII::IsWhitespace() is used. If empty, nothing happens.</param>
    /// <returns>The trimmed string.</returns>
    std::string_view TrimRightView(std::string_view str, const char* pCharsToTrim);

    /// <summary>
    /// Trims whitespace from the start and end of a string.
    /// </summary>
    /// <param name="pStr">The string to trim.</param>
    /// <param name="pCharsToTrim">A string containing the characters to trim.
    ///     If nullptr, ASCII::IsWhitespace() is used. If empty, nothing happens.</param>
    /// <returns>The trimmed string.</returns>
    inline std::string_view TrimView(std::string_view str, const char* pCharsToTrim)
    {
        return TrimLeftView(TrimRightView(str, pCharsToTrim), pCharsToTrim);
    }

    /// <summary>
    /// Trims characters from the start of a string.
    /// </summary>
    /// <param name="str">The string to trim.</param>
    /// <param name="pCharsToTrim">A string containing the characters to trim.
    ///     If nullptr, ASCII::IsWhitespace() is used. If empty, nothing happens.</param>
    /// <returns>The trimmed string.</returns>
    std::string& TrimLeft(std::string& str, const char* pCharsToTrim = nullptr);

    /// <summary>
    /// Trims characters from the end of a string.
    /// </summary>
    /// <param name="str">The string to trim.</param>
    /// <param name="pCharsToTrim">A string containing the characters to trim.
    ///     If nullptr, ASCII::IsWhitespace() is used. If empty, nothing happens.</param>
    /// <returns>The trimmed string.</returns>
    std::string& TrimRight(std::string& str, const char* pCharsToTrim = nullptr);

    /// <summary>
    /// Trims characters from the start and end of a string.
    /// </summary>
    /// <param name="str">The string to trim.</param>
    /// <param name="pCharsToTrim">A string containing the characters to trim.
    ///     If nullptr, ASCII::IsWhitespace() is used. If empty, nothing happens.</param>
    /// <returns>The trimmed string.</returns>
    inline std::string& Trim(std::string& str, const char* pCharsToTrim = nullptr)
    {
        return TrimLeft(TrimRight(str, pCharsToTrim), pCharsToTrim);
    }

    /// <summary>
    /// Determines whether a string starts with another string.
    /// </summary>
    /// <param name="strToCheck">The string to be checked.</param>
    /// <param name="strToCompare">The string to compare.</param>
    /// <param name="ignoreCase">If <b>true</b>, the comparison will be case insensitive.</param>
    /// <returns><b>true</b> if strToCheck isn't empty, strToCompare isn't empty and
    ///     strToCheck starts with strToCompare, else <b>false</b>.</returns>
    bool StartsWith(std::string_view strToCheck, std::string_view strToCompare, bool ignoreCase = true);

    /// <summary>
    /// Determines whether a wide string starts with another string.
    /// </summary>
    /// <param name="strToCheck">The wide string to be checked.</param>
    /// <param name="strToCompare">The wide string to compare.</param>
    /// <param name="ignoreCase">If <b>true</b>, the comparison will be case insensitive.</param>
    /// <returns><b>true</b> if strToCheck isn't empty, strToCompare isn't empty and
    ///     strToCheck starts with strToCompare, else <b>false</b>.</returns>
    bool StartsWith(std::wstring_view strToCheck, std::wstring_view strToCompare, bool ignoreCase = true);

    /// <summary>
    /// Determines whether a string ends with another string.
    /// </summary>
    /// <param name="strToCheck">The string to be checked.</param>
    /// <param name="strToCompare">The string to compare.</param>
    /// <param name="ignoreCase">If <b>true</b>, the comparison will be case insensitive.</param>
    /// <returns><b>true</b> if strToCheck isn't empty, strToCompare isn't empty and
    ///     strToCheck starts with strToCompare, else <b>false</b>.</returns>
    bool EndsWith(std::string_view strToCheck, std::string_view strToCompare, bool ignoreCase = true);

    /// <summary>
    /// Determines whether a wide string ends with another string.
    /// </summary>
    /// <param name="strToCheck">The string to be checked.</param>
    /// <param name="strToCompare">The string to compare.</param>
    /// <param name="ignoreCase">If <b>true</b>, the comparison will be case insensitive.</param>
    /// <returns><b>true</b> if strToCheck isn't empty, strToCompare isn't empty and
    ///     strToCheck starts with strToCompare, else <b>false</b>.</returns>
    bool EndsWith(std::wstring_view strToCheck, std::wstring_view strToCompare, bool ignoreCase = true);

    /// <summary>
    /// Returns the length of the end-of-line (EOL) characters pointed to by pStr.
    /// </summary>
    /// <param name="pStr">The pointer to the EOL characters.</param>
    /// <returns>The EOL length.</returns>
    /// <remarks>
    /// "\n" will ALWAYS return 1. (i.e. "\n\r" will return one.)
    /// "\r\n" will return 2.
    /// "\r" by itself will return 1.
    /// ("\r\n\n\r" will return 2. Upon incrementing the pointer, it will point to "\n",
    /// which will return 1. Increment the pointer again and it will return 1.)
    /// </remarks>
    int GetEolLen(const char* pStr);

    struct LineColumn
    {
        size_t line;
        size_t column;

        /// <summary>
        /// Calculated the line and column of the offset in pStr.
        /// </summary>
        /// <param name="pStr">The pointer to the string.</param>
        /// <param name="offset">The offset to use for the calculation.</param>
        LineColumn(const char* pStr, size_t offset);
        LineColumn(std::string_view str, size_t offset);
    };

    /// <summary>
    /// Removes all non-printable characters in str.
    /// </summary>
    /// <param name="str">The string to search.</param>
    /// <returns>A reference to str.</returns>
    std::string& RemoveAllNonPrintableChars(std::string& str);

    /// <summary>
    /// Replaces all non-printable characters in str.
    /// </summary>
    /// <param name="str">The string to search.</param>
    /// <returns>A reference to str.</returns>
    /// <remarks>
    /// This may destroy the integrity of UTF-8 strings.
    /// </remarks>
    std::string& ReplaceAllNonPrintableChars(std::string& str, char ch = '.');

    /// <summary>
    /// Removes all instances of find in str.
    /// </summary>
    /// <param name="str">The string to search.</param>
    /// <param name="find">The string to find.</param>
    /// <param name="pCount">Optional pointer to receive number of removes.</param>
    /// <returns>A reference to str.</returns>
    std::string& RemoveAll(std::string& str, std::string_view find, size_t* pCount = nullptr);

    /// <summary>
    /// Replaces all instances of find with replace in str.
    /// </summary>
    /// <param name="str">The string to search.</param>
    /// <param name="find">The string to find.</param>
    /// <param name="replace">The string to replace. If this is zero length,
    ///     RemoveAll2 will be called.</param>
    /// <param name="pCount">Optional pointer to receive number of replaces.</param>
    /// <returns>A reference to str.</returns>
    std::string& ReplaceAll(std::string& str, std::string_view find, std::string_view replace, size_t* pCount = nullptr);

    /// <summary>
    /// Safely copies a string into a buffer.
    /// </summary>
    /// <param name="pDst">Pointer to the destination buffer.</param>
    /// <param name="pSrc">Pointer to the source string.</param>
    /// <param name="maxLen">Maximum number of characters to copy, including the
    ///     terminating null.</param>
    /// <param name="fill">If <b>true</b> and len exceeds length of pSrc, buffer will be
    ///     filled with zeros up to len.</param>
    /// <param name="pErr">Optional pointer to a bool that will be set to <b>true</b> if
    ///     the buffer would overrun.</param>
    /// <returns>Length of destination string.</returns>
    /// <remarks>
    /// If pDst is nullptr, function will return 0. If pSrc is nullptr and maxLen is greater
    /// than zero, *pDst will ONLY be set if fill is true. If maxLen is zero, nothing
    /// will happen.
    /// </remarks>
    size_t Copy(char* pDst, const char* pSrc, size_t maxLen, bool fill = false, bool* pErr = nullptr);

    template <size_t _size>
    size_t Copy(char (&dst)[_size], const char* pSrc, bool fill = false, bool* pErr = nullptr)
    {
        return Copy(dst, pSrc, _size, fill, pErr);
    }

    /// <summary>
    /// Returns a string containing a specified number of characters from the
    /// right side of a string.
    /// </summary>
    /// <param name="str">The string.</param>
    /// <param name="maxChars">The maximum length of the returned string. If this
    ///     is greater than the length of <paramref="str"/>, the entire string will
    ///     be returned.</param>
    /// <returns>A string containing a specified number of characters from the
    ///     right side of a string.</returns>
    std::string Right(const std::string& str, size_t maxChars);

    /// <summary>
    /// Returns a string_view referencing a specified number of characters from the
    /// right side of a string_view.
    /// </summary>
    /// <param name="str">The string_view.</param>
    /// <param name="maxChars">The maximum length of the returned string. If this
    ///     is greater than the length of <paramref="str"/>, the entire string will
    ///     be returned.</param>
    /// <returns>A string_view referencing a specified number of characters from the
    ///     right side of a string_view.</returns>
    std::string_view RightView(const std::string_view& str, size_t maxChars);

    /// <summary>
    /// Safely concatenates two strings.
    /// </summary>
    /// <param name="pStr1">The first string. May be nullptr.</param>
    /// <param name="pStr2">The second string. May be nullptr.</param>
    /// <returns>A string made of pStr2 concatenated to pStr1. If both are nullptr,
    ///     an empty string is returned.</returns>
    std::string Concat(const char* pStr1, const char* pStr2);

    /// <summary>
    /// Safely concatenates two strings.
    /// </summary>
    /// <param name="pDst">Pointer to the destination string buffer.</param>
    /// <param name="pSrc">Pointer to the source string.</param>
    /// <param name="maxLen">Length of destination buffer.</param>
    /// <returns>Length of destination string.</returns>
    /// <remarks>
    /// If pDst is nullptr, function will return 0. If pSrc is nullptr or len is equal
    /// equal to zero, nothing will happen.
    /// </remarks>
    size_t Concat(char* pDst, const char* pSrc, size_t maxLen);

    template <size_t _size>
    size_t ConcatBuffer(char (&dst)[_size], const char* pSrc)
    {
        return Concat(dst, pSrc, _size);
    }

    /// <summary>
    /// Safely converts a string to uppercase.
    /// </summary>
    /// <param name="pStr">Pointer to the string to convert.</param>
    /// <param name="trimLeft">If \b true, skips whitespace at start of <paramref="pStr"/>.</param>
    /// <param name="trimRight">If \b true, ignores whitespace at end of <paramref="pStr"/>.</param>
    /// <returns>The string converted to uppercase.</returns>
    /// <remarks>
    /// If pStr is nullptr, function will return an empty string.
    /// </remarks>
    std::string ToUpper(const char* pStr, bool trimLeft = false, bool trimRight = false);

    /// <summary>
    /// Converts a string to uppercase.
    /// </summary>
    /// <param name="str">Reference to the string to convert.</param>
    /// <returns>The new string in uppercase.</returns>
    std::string ToUpper(const std::string& str);

    /// <summary>
    /// Converts a string to uppercase without creating a new string.
    /// </summary>
    /// <param name="str">Reference to the string to convert.</param>
    /// <returns>A reference to <paramref="str"/>.</returns>
    std::string& ToUpperInPlace(std::string& str);

    /// <summary>
    /// Safely converts a string to lowercase.
    /// </summary>
    /// <param name="pStr">Pointer to the string to convert.</param>
    /// <param name="trimLeft">If \b true, skips whitespace at start of <paramref="pStr"/>.</param>
    /// <param name="trimRight">If \b true, ignores whitespace at end of <paramref="pStr"/>.</param>
    /// <returns>The string converted to lowercase.</returns>
    /// <remarks>
    /// If pStr is nullptr, function will return an empty string.
    /// </remarks>
    std::string ToLower(const char* pStr, bool trimLeft = false, bool trimRight = false);

    /// <summary>
    /// Converts a string to lowercase.
    /// </summary>
    /// <param name="str">Reference to the string to convert.</param>
    /// <returns>The new string in lowercase.</returns>
    std::string ToLower(const std::string& str);

    /// <summary>
    /// Converts a string to lowercase without creating a new string.
    /// </summary>
    /// <param name="str">Reference to the string to convert.</param>
    /// <returns>A reference to <paramref="str"/>.</returns>
    std::string& ToLowerInPlace(std::string& str);

    /// <summary>
    /// Checks if one string contains another, ignoring case.
    /// </summary>
    /// <param name="stringToFind">The string to find.</param>
    /// <param name="stringToSearch">The string to search.</param>
    /// <returns><b>true</b> if <paramref="stringToFind"/> is within
    ///     <paramref="stringToSearch"/>, else <b>false</b>.</returns>
    bool FindNoCase(std::string_view stringToFind, std::string_view stringToSearch);

    /// <summary>
    /// Checks if one string contains another, ignoring case.
    /// </summary>
    /// <param name="stringToFind">The string to find.</param>
    /// <param name="stringToSearch">The string to search.</param>
    /// <returns><b>true</b> if <paramref="stringToFind"/> is within
    ///     <paramref="stringToSearch"/>, else <b>false</b>.</returns>
    bool FindNoCase(std::wstring_view stringToFind, std::wstring_view stringToSearch);

    /// <summary>
    /// Safely compares two strings.
    /// </summary>
    /// <param name="pStr1">Pointer to the first string. May be nullptr.</param>
    /// <param name="pStr2">Pointer to the second string. May be nullptr.</param>
    /// <param name="ignoreCase">If <b>true</b>, case will be ignored.</param>
    /// <param name="len">If non-zero, maximum number of characters to compare.</param>
    /// <returns>An value indicating the relationship between pStr1 and pStr2.
    ///
    /// Value   Indicates
    /// ======  ============================
    /// < 0     pStr1 is less than pStr2
    ///   0     pStr1 is equal to pStr2
    /// > 0     pStr1 is greater than pStr2
    ///
    /// If pStr1 or pStr2 are null, the following are returned:
    ///
    /// pStr1       pStr2       return
    /// =========== =========== ===========
    /// nullptr     nullptr     0
    /// nullptr     valid       INT_MIN
    /// valid       nullptr     INT_MAX
    /// </returns>
    int Compare(const char* pStr1, const char* pStr2, bool ignoreCase = false, size_t len = 0);

    // The following is an example of a Doxygen comment block for the above function.
    /** Safely compares two strings.
        @param str1 Pointer to the first string. May be nullptr.
        @param str2 Pointer to the second string. May be nullptr.
        @param ignoreCase If \b true, case will be ignored.
        @param len If non-zero, maximum number of characters to compare.
        @returns A value indicating the relationship between pStr1 and pStr2.
        @returns
            Value  | Indicates
            ------ | ------------------------------
            < 0    | \a pStr1 is less than \a pStr2
              0    | \a pStr1 is equal to \a pStr2
            > 0    | \a pStr1 is greater than \a pStr2
        @returns
            If \a pStr1 or \a pStr2 are nullptr, the following are returned:
        @returns
            pStr1      | pStr2      | returns
            ---------- | ---------- | ----------
            nullptr    | nullptr    | 0
            nullptr    | valid      | INT_MIN
            valid      | nullptr    | INT_MAX
     */

    /// <summary>
    /// Safely compares two strings for equality.
    /// </summary>
    /// <param name="pStr1">Pointer to the first string. May be nullptr.</param>
    /// <param name="pStr2">Pointer to the second string. May be nullptr.</param>
    /// <param name="failOnNull">If <b>true</b>, if either string is nullptr,
    ///     <b>false</b> will be returned.</param>
    /// <param name="ignoreCase">If <b>true</b>, case will be ignored.</param>
    /// <returns><b>true</b> if the string are equal, else <b>false</b>.
    /// </returns>
    bool AreEqual(const char* pStr1, const char* pStr2, bool failOnNull = false, bool ignoreCase = false);

    /// <summary>
    /// Compares two string views.
    /// </summary>
    /// <param name="str1">First string_view.</param>
    /// <param name="str2">Second string_view.</param>
    /// <param name="ignoreCase">If <b>true</b>, case will be ignored.</param>
    /// <param name="len">If non-zero, maximum number of characters to compare.</param>
    /// <returns>An value indicating the relationship between str1 and str2.
    /// <para>
    /// Value   Indicates
    /// ======  ============================
    /// < 0     str1 is less than str2
    ///   0     str1 is equal to str2
    /// > 0     str1 is greater than str2
    /// </para>
    /// </returns>
    int CompareV(std::string_view str1, std::string_view str2, bool ignoreCase = true, size_t len = 0);

    /// <summary>
    /// Compares two string view references.
    /// </summary>
    /// <param name="str1">Reference to first string_view.</param>
    /// <param name="str2">Reference to second string_view.</param>
    /// <param name="ignoreCase">If <b>true</b>, case will be ignored.</param>
    /// <param name="len">If non-zero, maximum number of characters to compare.</param>
    /// <returns>An value indicating the relationship between str1 and str2.
    /// <para>
    /// Value   Indicates
    /// ======  ============================
    /// < 0     str1 is less than str2
    ///   0     str1 is equal to str2
    /// > 0     str1 is greater than str2
    /// </para>
    /// </returns>
    int CompareV2(const std::string_view& str1, const std::string_view& str2, bool ignoreCase = true, size_t len = 0);

    /// <summary>
    /// Safely compares two strings.
    /// </summary>
    /// <param name="pStr1">Pointer to the first string. May be nullptr.</param>
    /// <param name="pStr2">Pointer to the second string. May be nullptr.</param>
    /// <param name="ignoreCase">If <b>true</b>, case will be ignored.</param>
    /// <param name="len">If non-zero, maximum number of characters to compare.</param>
    /// <returns>An value indicating the relationship between pStr1 and pStr2.
    /// <para>
    /// Value   Indicates
    /// ======  ============================
    /// < 0     pStr1 is less than pStr2
    ///   0     pStr1 is equal to pStr2
    /// > 0     pStr1 is greater than pStr2
    /// </para><para>
    /// If pStr1 or pStr2 are null, the following are returned:
    /// </para><para>
    /// pStr1       pStr2       return
    /// =========== =========== ===========
    /// nullptr     nullptr     0
    /// nullptr     valid       INT_MIN
    /// valid       nullptr     INT_MAX
    /// <para>
    /// </returns>
    int Compare(const wchar_t* pStr1, const wchar_t* pStr2, bool ignoreCase = false, size_t len = 0);

    /// <summary>
    /// Compares two wide string views.
    /// </summary>
    /// <param name="str1">First string_view.</param>
    /// <param name="str2">Second string_view.</param>
    /// <param name="ignoreCase">If <b>true</b>, case will be ignored.</param>
    /// <param name="len">If non-zero, maximum number of characters to compare.</param>
    /// <returns>An value indicating the relationship between str1 and str2.
    /// <para>
    /// Value   Indicates
    /// ======  ============================
    /// < 0     str1 is less than str2
    ///   0     str1 is equal to str2
    /// > 0     str1 is greater than str2
    /// </para>
    /// </returns>
    int CompareV(std::wstring_view str1, std::wstring_view str2, bool ignoreCase = true, size_t len = 0);

    /// <summary>
    /// Trims a string, converts whitespace to spaces and removes excess whitespace.
    /// </summary>
    /// <param name="pStr">The string to normalize.</param>
    /// <returns>The normalized string.</returns>
    std::string NormalizeString(const char* pStr);

    /// <summary>
    /// Trims a string, converts whitespace to spaces and removes excess whitespace.
    /// </summary>
    /// <param name="str">The string to normalize.</param>
    /// <returns>The normalized string.</returns>
    std::string NormalizeString(std::string_view str);

    /// <summary>
    /// Converts a raw string to a quoted loggable string.
    /// </summary>
    /// <param name="pStr">The string to quote. This can be null or empty.</param>
    /// <returns>A quoted string. If pStr is null, "&lt;null&gt;" is returned.</returns>
    std::string ToQuotedString(const char* pStr);

    /// <summary>
    /// Converts a string_view to a quoted loggable string.
    /// </summary>
    /// <param name="str">The string to quote. This can be empty.</param>
    std::string ToQuotedString2(std::string_view str);

    /// <summary>
    /// Formats a string, replacing placeholders with associated strings.
    /// </summary>
    /// <param name="formatString">The string to format. Placeholders are in the format
    ///     {x} where x is a digit 0-5. If the associated string is a nullptr, it will
    ///     be treated as an empty string.</param>
    /// <param name="pStr0">String arument 0. This can be empty or nullptr.</param>
    /// <param name="pStr1">String arument 1. This can be empty or nullptr.</param>
    /// <param name="pStr2">String arument 2. This can be empty or nullptr.</param>
    /// <param name="pStr3">String arument 3. This can be empty or nullptr.</param>
    /// <param name="pStr4">String arument 4. This can be empty or nullptr.</param>
    /// <param name="pStr5">String arument 5. This can be empty or nullptr.</param>
    /// <returns>The formatted string.</param>
    std::string Format(std::string_view formatString,
        const char* pStr0          , const char* pStr1 = nullptr, const char* pStr2 = nullptr,
        const char* pStr3 = nullptr, const char* pStr4 = nullptr, const char* pStr5 = nullptr);

    /// <summary>
    /// Formats a string, replacing placeholders with associated strings while appending
    /// the results to the passed string.
    /// </summary>
    /// <param name="strToAppend">A reference to the string to append.</param>
    /// <param name="formatString">The string to format. Placeholders are in the format
    ///     {x} where x is a digit 0-5. If the associated string is a nullptr, it will
    ///     be treated as an empty string.</param>
    /// <param name="pStr0">String arument 0. This can be empty or nullptr.</param>
    /// <param name="pStr1">String arument 1. This can be empty or nullptr.</param>
    /// <param name="pStr2">String arument 2. This can be empty or nullptr.</param>
    /// <param name="pStr3">String arument 3. This can be empty or nullptr.</param>
    /// <param name="pStr4">String arument 4. This can be empty or nullptr.</param>
    /// <param name="pStr5">String arument 5. This can be empty or nullptr.</param>
    /// <returns>A reference to dstString.</param>
    std::string& FormatAppend(std::string& strToAppend, std::string_view formatString,
        const char* pStr0          , const char* pStr1 = nullptr, const char* pStr2 = nullptr,
        const char* pStr3 = nullptr, const char* pStr4 = nullptr, const char* pStr5 = nullptr);

    /// <summary>
    /// Formats a string, replacing placeholders with associated strings.
    /// </summary>
    /// <param name="formatString">The string to format. Placeholders are in the format
    ///     {x} where x is a digit 0-5.</param>
    /// <param name="string0">String arument 0. This can be empty.</param>
    /// <param name="string1">String arument 1. This can be empty.</param>
    /// <param name="string2">String arument 2. This can be empty.</param>
    /// <returns>The formatted string.</param>
    std::string Format3(std::string_view formatString,
        std::string_view string0, std::string_view string1 = std::string_view(), std::string_view string2 = std::string_view());

    /// <summary>
    /// Formats a string, replacing placeholders with associated strings.
    /// </summary>
    /// <param name="formatString">The string to format. Placeholders are in the format
    ///     {x} where x is a digit 0-9.</param>
    /// <param name="strings">A vector of the string arguments.</param>
    /// <param name="reserveExtra">Number of extra characters to reserve in the return string.</param>
    /// <returns>The formatted string.</param>
    std::string FormatV(std::string_view formatString, const std::vector<std::string_view>& strings, size_t reserveExtra = 0);



} // namespace Str

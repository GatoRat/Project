// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <climits>
#include <cstring>
#include <string>
#include "Str.h"

// Predicates and Hashes which can be used by std::map and std::set classes.
// Note: std::map and std::set use less than
//       std::unordered_map and std::unordered_set use a hash and equal

/** A binary predicate to compare two strings.
 */
struct str_equal_to
{
    /** The predicate operator for const char*.
        @param pStr1 The first string to compare.
        @param pStr2 The second string to compare.
        @returns \b true if strings are equal, else \b false.
        @remarks This is case sensitive.
    */
    bool operator()(const char* pStr1, const char* pStr2) const
    {
        return strcmp(pStr1, pStr2) == 0;
    }
};

/** A binary predicate to compare two strings, ignoring case.
*/
struct str_equal_to_nocase
{
    /** The predicate operator for const char*.
        @param pStr1 The first string to compare.
        @param pStr2 The second string to compare.
        @returns \b true if strings are equal, else \b false.
        @remarks This is case insensitive.
    */
    bool operator()(const char* pStr1, const char* pStr2) const
    {
        return _stricmp(pStr1, pStr2) == 0;
    }

    /** The predicate operator for const std::string&.
        @param str1 The first string to compare.
        @param str2 The second string to compare.
        @returns \b true if strings are equal, else \b false.
        @remarks This is case insensitive.
    */
    bool operator()(const std::string& str1, const std::string& str2) const
    {
        return _stricmp(str1.c_str(), str2.c_str()) == 0;
    }
};

/** A binary predicate to compare two strings for "less than".
 */
struct str_less_than
{
    /** The predicate operator.
        @param pStr1 The first string to compare.
        @param pStr2 The second string to compare.
        @returns \b true if \a pStr1 is less than \a pStr2, else \b false.
        @remarks This is case sensitive.
    */
    bool operator()(const char* pStr1, const char* pStr2) const
    {
        return strcmp(pStr1, pStr2) < 0;
    }
};

/** A binary predicate to compare two strings for "less than".
*/
struct wstr_less_than
{
    /** The predicate operator.
        @param pStr1 The first string to compare.
        @param pStr2 The second string to compare.
        @returns \b true if \a pStr1 is less than \a pStr2, else \b false.
        @remarks This is case sensitive.
    */
    bool operator()(const wchar_t* pStr1, const wchar_t* pStr2) const
    {
        return wcscmp(pStr1, pStr2) < 0;
    }
};

/** A binary predicate to compare two strings for "less than", ignoring case.
*/
struct str_less_than_nocase
{
    /** The predicate operator for const char*.
        @param pStr1 The first string to compare.
        @param pStr2 The second string to compare.
        @returns \b true if \a pStr1 is less than \a pStr2, else \b false.
        @remarks This is case insensitive.
    */
    bool operator() (const char* pStr1, const char* pStr2) const
    {
        return _stricmp(pStr1, pStr2) < 0;
    }

    /** The predicate operator for const std::string&.
        @param str1 The first string to compare.
        @param str2 The second string to compare.
        @returns \b true if \a str1 is less than \a str2, else \b false.
        @remarks This is case insensitive.
    */
    bool operator() (const std::string& str1, const std::string& str2) const
    {
        return _stricmp(str1.c_str(), str2.c_str()) < 0;
    }

#if __has_include(<qstring.h>)
    /** The predicate operator for const QString&.
        @param str1 The first string to compare.
        @param str2 The second string to compare.
        @returns \b true if \a str1 is less than \a str2, else \b false.
        @remarks This is case insensitive.
    */
    bool operator() (const QString& str1, const QString& str2) const
    {
        return str1.compare(str2, Qt::CaseInsensitive) < 0;
    }
#endif
};

/** A binary predicate to compare two strings for "less than", ignoring case.
*/
struct str_less_than_nocase_view
{
    /** The predicate operator for const std::string&.
        @param str1 The first string to compare.
        @param str2 The second string to compare.
        @returns \b true if \a str1 is less than \a str2, else \b false.
        @remarks This is case insensitive.
    */
    bool operator() (const std::string_view& str1, const std::string_view& str2) const
    {
        return Str::CompareV2(str1, str2, true) < 0;
    }
};

/** A binary predicate to compare two strings for "less than", ignoring case.
*/
struct wstr_less_than_nocase
{
    /** The predicate operator for const wchar_t*.
        @param pStr1 The first string to compare.
        @param pStr2 The second string to compare.
        @returns \b true if \a pStr1 is less than \a pStr2, else \b false.
        @remarks This is case insensitive.
    */
    bool operator() (const wchar_t* pStr1, const wchar_t* pStr2) const
    {
#ifdef _WIN32
        return _wcsicmp(pStr1, pStr2) < 0;
#else
        return wcscasecmp(pStr1, pStr2) < 0;
#endif
    }

    /** The predicate operator for const std::wstring&.
        @param str1 The first string to compare.
        @param str2 The second string to compare.
        @returns \b true if \a str1 is less than \a str2, else \b false.
        @remarks This is case insensitive.
    */
    bool operator() (const std::wstring& str1, const std::wstring& str2) const
    {
#ifdef _WIN32
        return _wcsicmp(str1.c_str(), str2.c_str()) < 0;
#else
        return wcscasecmp(str1.c_str(), str2.c_str()) < 0;
#endif
    }
};

/** A simple unary hash for a const char*.
    This is a fast, commonly used, public domain hash.
 */
struct str_hash_sdbm
{
    /** The unary hash operator.
        @param pStr The string to hash.
        @returns The hash of \a pStr.
    */
    int operator()(const char* pStr) const
    {
        int hash = 0;

        while (*pStr)
        {
            hash = *pStr++ + (hash << 6) + (hash << 16) - hash;
        }

        return hash & INT_MAX;
    }
};

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string_view>

/** Indicates whether a character is a valid filename character
 */
bool IsValidFilenameChar(char ch, bool asciiOnly = false);

/** Indicates whether a character is a valid filename character
*/
bool IsValidFilenameChar(wchar_t ch, bool asciiOnly = false);

/** Indicates whether a string is valid for a filename
*/
bool IsValidFilenameStr(std::string_view str, bool asciiOnly = false);

/** Indicates whether a string is valid for a filename
*/
bool IsValidFilenameStr(std::wstring_view str, bool asciiOnly = false);

/** Indicates whether a string is valid for a filename on all platforms
*/
bool IsValidFilenameStrAllPlatforms(std::string_view str, bool asciiOnly = false);

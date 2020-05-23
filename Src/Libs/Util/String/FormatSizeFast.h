// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include <string>

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @param isNeg \b True if \a val is negative, else \b false.
   @returns The resulting string.
 */
std::string FormatSizeFast(uint64_t val, bool isNeg = false);

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @returns The resulting string.
 */
std::string FormatSizeFast(int64_t val);

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @param isNeg \b True if \a val is negative, else \b false.
   @returns The resulting string.
*/
inline
std::string FormatSizeFast(uint32_t val, bool isNeg = false)
{
    return FormatSizeFast(static_cast<uint64_t>(val), isNeg);
}

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @param isNeg \b True if \a val is negative, else \b false.
   @returns The resulting string.
*/
inline
std::string FormatSizeFast(uint16_t val, bool isNeg = false)
{
    return FormatSizeFast(static_cast<uint64_t>(val), isNeg);
}

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @returns The resulting string.
*/
std::string FormatSizeFast(int32_t val);

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @param isNeg \b True if \a val is negative, else \b false.
   @returns The resulting string.
*/
inline
std::string FormatSizeFast(int32_t val)
{
    return FormatSizeFast(static_cast<int64_t>(val));
}

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @param isNeg \b True if \a val is negative, else \b false.
   @returns The resulting string.
*/
inline
std::string FormatSizeFast(int16_t val)
{
    return FormatSizeFast(static_cast<int64_t>(val));
}

/** Formats a memory size with no localization.
   @param val The value to format.
   @param decimalPlaces The number of decimal places to use. Any value greater
            than 2, will be treated as 2.
   @param isNeg \b True if \a val is negative, else \b false.
   @returns The resulting string.
 */
std::string FormatMemorySizeFast(uint64_t val, unsigned decimalPlaces = 2, bool isNeg = false);

/** Formats a memory size with no localization.
   @param val The value to format.
   @param decimalPlaces The number of decimal places to use. Any value greater
            than 2, will be treated as 2.
   @param isNeg \b True if \a val is negative, else \b false.
   @returns The resulting string.
 */
inline
std::string FormatMemorySizeFast(uint32_t val, unsigned decimalPlaces = 2, bool isNeg = false)
{
    return FormatMemorySizeFast(static_cast<uint64_t>(val), decimalPlaces, isNeg);
}

/** Formats a memory size with no localization.
   @param val The value to format.
   @param decimalPlaces The number of decimal places to use. Any value greater
            than 2, will be treated as 2.
   @returns The resulting string.
 */
std::string FormatMemorySizeFast(int64_t val, unsigned decimalPlaces = 2);

/** Formats a memory size with no localization.
   @param val The value to format.
   @param decimalPlaces The number of decimal places to use. Any value greater
            than 2, will be treated as 2.
   @returns The resulting string.
 */
inline
std::string FormatMemorySizeFast(int32_t val, unsigned decimalPlaces = 2)
{
    return FormatMemorySizeFast(static_cast<int64_t>(val), decimalPlaces);
}

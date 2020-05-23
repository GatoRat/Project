// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <locale>
#include <string>

/// <summary>
/// Values that represent format number styles.
/// </summary>
enum class FormatSizeStyle : uint32_t
{
    /// <summary>Use no styles.</summary>
    None             = 0b00000000,
    /// <summary>Use group separators.</summary>
    Separators       = 0b00000001,
    /// <summary>Use one decimal for the number.</summary>
    OneDecimal       = 0b00000010,
    /// <summary>Use two decimals for the number.</summary>
    TwoDecimals      = 0b00000100,
    /// <summary>If val is zero, don't use a decimal symbol.</summary>
    NoDecimalIfZero  = 0b00001000,
    /// <summary>If symbols are used, do not put a space between the number and the symbol.</summary>
    Abbrev_NoSpace   = 0b00010000,
    /// <summary>Use the traditional two letter symbols.</summary>
    Abbrev_TwoLetter = 0b00100000,
    /// <summary>Use the IEEE 1541 symbols.</summary>
    /// <remarks>IEEE 1541 defines the prefixes Ki, Mi, Gi and Ti.</remarks>
    Abbrev_IEEE_1541 = 0b01000000,
    /// <summary>Numbers will be calculated as per the SI standard.</summary>
    /// <remarks>
    /// The SI standard specifies that 1K is equal to 1000, while IEEE 1541
    /// specifies that 1Ki is equal to 1024. Computers have traditionally used 1K
    /// for 1024. This setting enforces the SI and IEEE standards.
    /// </remarks>
    StrictCalculation = 0b10000000,
};

/// <summary>Bitwise 'or' operator.</summary>
/// <param name="a">A bit-field to process.</param>
/// <param name="b">One or more bits to OR into the bit-field.</param>
/// <returns>The result of the operation.</returns>
constexpr FormatSizeStyle operator|(FormatSizeStyle a, FormatSizeStyle b)
{
    return static_cast<FormatSizeStyle>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

/// <summary>Bitwise 'and' operator.</summary>
/// <param name="a">A bit-field to process.</param>
/// <param name="b">A mask of bits to apply to the bit-field.</param>
/// <returns>The result of the operation.</returns>
constexpr uint32_t operator&(FormatSizeStyle a, FormatSizeStyle b)
{
    return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
}

/// <summary>
/// Format a size.
/// </summary>
/// <param name="val">    The value.</param>
/// <param name="style">  The style to use for formatting.</param>
/// <param name="pLocale">If null, the global C++ locale will be used, else the
///  locale to use.</param>
/// <returns>The formatted number.</returns>
/// <remarks>
/// This function will format the number using the locale rules. For example, in
/// India, groupings are two digits long except for the last three digits
/// (i.e. 10,10,100 vs 1,010,100.)
/// </remarks>
std::string FormatSize(uint64_t val,
                       FormatSizeStyle style = FormatSizeStyle::Separators | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::TwoDecimals,
                       const std::locale* pLocale = nullptr);

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @param isNeg \b True if \p val is negative, else \b false.
   @returns The resulting string.
*/
inline
std::string FormatSize(uint32_t val,
                       FormatSizeStyle style = FormatSizeStyle::Separators | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::TwoDecimals,
                       const std::locale* pLocale = nullptr)
{
    return FormatSize(static_cast<uint64_t>(val), style, pLocale);
}

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @param isNeg \b True if \p val is negative, else \b false.
   @returns The resulting string.
*/
inline
std::string FormatSize(uint16_t val,
                       FormatSizeStyle style = FormatSizeStyle::Separators | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::TwoDecimals,
                       const std::locale* pLocale = nullptr)
{
    return FormatSize(static_cast<uint64_t>(val), style, pLocale);
}

/// <summary>
/// Format a size.
/// </summary>
/// <param name="val">    The value.</param>
/// <param name="style">  The style to use for formatting.</param>
/// <param name="pLocale">If null, the global C++ locale will be used, else the
///  locale to use.</param>
/// <returns>The formatted number.</returns>
/// <remarks>
/// This function will format the number using the locale rules. For example, in
/// India, groupings are two digits long except for the last three digits
/// (i.e. 10,10,100 vs 1,010,100.)
/// </remarks>
std::string FormatSize(int64_t val,
                       FormatSizeStyle style = FormatSizeStyle::Separators | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::TwoDecimals,
                       const std::locale* pLocale = nullptr);

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @param isNeg \b True if \p val is negative, else \b false.
   @returns The resulting string.
*/
inline
std::string FormatSize(int32_t val,
                       FormatSizeStyle style = FormatSizeStyle::Separators | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::TwoDecimals,
                       const std::locale* pLocale = nullptr)
{
    return FormatSize(static_cast<int64_t>(val), style, pLocale);
}

/** Formats a value to a comma delimited string with no localization.
   @param val The value to format.
   @param isNeg \b True if \p val is negative, else \b false.
   @returns The resulting string.
*/
inline
std::string FormatSize(int16_t val,
                       FormatSizeStyle style = FormatSizeStyle::Separators | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::TwoDecimals,
                       const std::locale* pLocale = nullptr)
{
    return FormatSize(static_cast<int64_t>(val), style, pLocale);
}

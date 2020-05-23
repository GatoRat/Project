// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include <locale>

namespace Money
{
    /** Calculates the value of the string passed to hundredths of a cent.
        @param pStr The string value. The string must start with a dollar ($) sign, or the
            locale equivalent, or a digit (0-9). Leading whitespace will be ignored. If
            there is a currency symbol, whitespace immediately after it will be ignored.
        @param val The value to hundredths of a cent.
        @param pEnd An optional pointer to the character which ended the parsing.
        @param pLocale An optional pointer to the locale to use for currency symbols.
        @returns \b true, if conversion was successful and parsing ended at the terminating
            zero, else \b false. If the string is expected to end with non-money characters,
            pEnd should be passed and checked upon a \b false return.
        @remarks If \a pStr is "1.23" (in US locale) \a val will be set to 12300.
    */
    bool ToVal(const char* pStr, uint64_t& val, const char** pEnd = nullptr, const std::locale* pLocale = nullptr);

    /** Calculates the value of the string passed to hundredths of a cent.
        @param pStr The string value. The string must start with a currency symbol or a
            digit (0-9). Leading whitespace will be ignored. If there is a currency
            symbol, whitespace immediately after it will be ignored. If pCurSymbol is
            not NULL and does not point to zero (0) and a non digit is found first, it
            must match this value.
        @param val The value to hundredths of a cent.
        @param decimalPoint The expected character of a decimal point.
        @param thousandsSep The expected character of a thousands separator.
        @param pCurSymbol If the string doesn't start with a digit, that character will be
            considered a currency symbol. If this is not NULL and points to a non-zero (0)
            character, the currency symbol must match this character. If it points to zero (0),
            and a non-digit is found, it will set to that character.
        @param pEnd An optional pointer to the character which ended the parsing.
        @returns \b true, if conversion was successful and parsing ended at the terminating
            zero, else \b false. If the string is expected to end with non-money characters,
            pEnd should be passed and checked upon a \b false return.
        @remarks If \a pStr is "X 1.23" \a val will be set to 12300.
    */
    bool ToVal2(const char* pStr, int64_t& val, char decimalPoint = '.', char thousandsSep = ',', char* pCurSymbol = nullptr, const char** pEnd = nullptr);

    namespace Optima
    {
        extern char currencySymbol[6];
        extern char thousandsSymbol;
        extern char appendCurrency;

        const char* parseCurrency(const char* pStr);
        void        parseMoneyFormat(const char* pMoneyFormat);
        const char* formatDollars(__int64 money, char* pBuffer, size_t bufferLen);
    }
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Money.h"
#include <string_view>
#include "../Misc/Defines.Misc.h"
#include "../String/ASCII.h"

using namespace std;

namespace Money
{
    bool ToVal(const char* pStr, uint64_t& val, const char** pEnd, const locale* pLocale)
    {
        bool err = true;
        val = 0;

        if (pStr)
        {
            while (ASCII::IsWhitespace(*pStr))
            {
                ++pStr;
            }

            err = false;

            // Note: loc has to stay valid for the life of the moneypunct facet!
            //       (Hence this odd construct to handle a temporary locale.)
            locale loc(pLocale && has_facet<moneypunct<char>>(*pLocale) ? pLocale->name() :
#ifdef __linux__
                "en_US.utf8");
#else
                "en-US");
#endif
            auto& punct = use_facet<moneypunct<char>>(loc);

            if (*pStr)
            {
                if (string_view(pStr, 1) == punct.curr_symbol())
                {
                    ++pStr;

                    while (ASCII::IsWhitespace(*pStr))
                    {
                        ++pStr;
                    }
                }
                else if (!ASCII::IsDigit(*pStr))
                {
                    err = true;
                }
            }

            if (!err && *pStr)
            {
                const uint32_t lastDigit = UINT64_MAX % BASE_TEN;
                const uint64_t maxValue  = UINT64_MAX / BASE_TEN;

                int decimalPlaces = -1;

                while (*pStr && decimalPlaces < 4) //-V112
                {
                    if (*pStr == punct.decimal_point())
                    {
                        if (decimalPlaces >= 0)
                        {
                            err = true;
                            break;
                        }

                        decimalPlaces = 0;
                    }
                    else if (*pStr != punct.thousands_sep())
                    {
                        if (val <= maxValue && ASCII::IsDigit(*pStr))
                        {
                            if (decimalPlaces >= 0)
                            {
                                ++decimalPlaces;
                            }

                            const uint32_t chVal = static_cast<uint32_t>(ASCII::DigitToVal(*pStr));
                            if (val == maxValue && chVal > lastDigit)
                            {
                                err = true;
                                break;
                            }

                            val *= BASE_TEN;
                            val += chVal;
                        }
                        else
                        {
                            err = true;
                            break;
                        }
                    }

                    ++pStr;
                }

                if (decimalPlaces < 0)
                {
                    decimalPlaces = 0;
                }

                while (decimalPlaces++ < 4) //-V112
                {
                    val *= 10;
                }
            }
            else
            {
                err = true;
            }
        }

        if (pEnd)
        {
            *pEnd = pStr;
        }

        return !err;
    }

    constexpr bool IsMoney(char ch)
    {
        return ch == '$' ||
            ASCII::IsUpper(ch) ||
            ASCII::IsLower(ch) || 
            ch < 0;
    }

    bool ToVal2(const char* pStr, int64_t& val, char decimalPoint, char thousandsSep, char* pCurSymbol, const char** pEnd)
    {
        bool err = true;
        val = 0;

        if (pStr)
        {
            while (ASCII::IsWhitespace(*pStr))
            {
                ++pStr;
            }

            err = false;

            if (*pStr && !ASCII::IsDigit(*pStr))
            {
                if (!IsMoney(*pStr))
                {
                    err = true;
                }
                else
                {
                    if (pCurSymbol)
                    {
                        if (*pCurSymbol)
                        {
                            err = *pCurSymbol != *pStr;
                        }
                        else
                        {
                            *pCurSymbol = *pStr;
                        }
                    }

                    if (!err)
                    {
                        ++pStr;
                    }

                    while (ASCII::IsWhitespace(*pStr))
                    {
                        ++pStr;
                    }
                }
            }

            if (!err && *pStr)
            {
                const uint32_t lastDigit = UINT64_MAX % BASE_TEN;
                const uint64_t maxValue  = UINT64_MAX / BASE_TEN;

                bool hasDecimal = false;
                int decimalPlaces = -1;

                while (*pStr && decimalPlaces < 4) //-V112
                {
                    if (*pStr == decimalPoint)
                    {
                        if (decimalPlaces >= 0)
                        {
                            err = true;
                            break;
                        }

                        hasDecimal = true;
                        decimalPlaces = 0;
                    }
                    else if (*pStr == thousandsSep)
                    {
                        if (hasDecimal)
                        {
                            err = true;
                            break;
                        }
                    }
                    else
                    {
                        if (val <= maxValue && ASCII::IsDigit(*pStr))
                        {
                            if (decimalPlaces >= 0)
                            {
                                ++decimalPlaces;
                            }

                            const uint32_t chVal = static_cast<uint32_t>(ASCII::DigitToVal(*pStr));
                            if (val == maxValue && chVal > lastDigit)
                            {
                                err = true;
                                break;
                            }

                            val *= BASE_TEN;
                            val += chVal;
                        }
                        else
                        {
                            err = true;
                            break;
                        }
                    }

                    ++pStr;
                }

                if (decimalPlaces < 0)
                {
                    decimalPlaces = 0;
                }

                while (decimalPlaces++ < 4) //-V112
                {
                    val *= 10;
                }
            }
            else
            {
                err = true;
            }
        }

        if (pEnd)
        {
            *pEnd = pStr;
        }

        return !err;
    }

    namespace Optima
    {
#define MAX_CURRENCY_SYMBOL_LEN 5
        char currencySymbol[MAX_CURRENCY_SYMBOL_LEN + 1] = "";
        int  currencySymbolLen = 0;
        char thousandsSymbol   = 0;
        char appendCurrency    = 0;

        static void setCurrencySymbolDefault()
        {
            currencySymbol[0] = '$';
            currencySymbol[1] = 0;
            currencySymbolLen = 1;
        }

        const char* parseCurrency(const char* pStr)
        {
            setCurrencySymbolDefault();

            if (pStr && *pStr == '\'')
            {
                ++pStr;

                currencySymbolLen = 0;
                while (*pStr && *pStr != '\'' && currencySymbolLen < MAX_CURRENCY_SYMBOL_LEN)
                {
                    currencySymbol[currencySymbolLen++] = *pStr++;
                }

                if (currencySymbolLen)
                {
                    currencySymbol[currencySymbolLen] = 0;
                }
                else
                {
                    setCurrencySymbolDefault();
                }

                while (*pStr && *pStr != '\'')
                {
                    ++pStr;
                }

                if (*pStr == '\'')
                {
                    ++pStr;
                }
            }

            return pStr;
        }

        void parseMoneyFormat(const char* pMoneyFormat)
        {
            setCurrencySymbolDefault();
            thousandsSymbol = ',';
            appendCurrency  = 0;

            if (pMoneyFormat)
            {
                if (*pMoneyFormat == '\'')
                {
                    pMoneyFormat = parseCurrency(pMoneyFormat);
                }

                if (*pMoneyFormat == ',' || *pMoneyFormat == '.') //-V522
                {
                    thousandsSymbol = *pMoneyFormat++;
                }

                if (*pMoneyFormat == ',' || *pMoneyFormat == '.')
                {
                    ++pMoneyFormat;
                    //decimalSymbol = *pMoneyFormat++;
                }

                if (*pMoneyFormat == '\'')
                {
                    appendCurrency = 1;
                    pMoneyFormat = parseCurrency(pMoneyFormat);
                }
            }
        }

        static char* CopyCurrency(char* pBuffer)
        {
            for (int i = currencySymbolLen; i > 0;)
            {
                *--pBuffer = currencySymbol[--i];
            }
            return pBuffer;
        }

        // bufferLen should be at least 27 characters long.
        const char* formatDollars(__int64 money, char* pFinalBuffer, size_t finalBufferLen)
        {
            int isNeg = 0;

            if (money < 0)
            {
                isNeg = 1;
                money = -money;
            }

            money /= 10000;

            char buffer[30];
            char* pBuffer = &buffer[sizeof(buffer) - 1];
            *--pBuffer = 0;

            if (appendCurrency)
            {
                pBuffer = CopyCurrency(pBuffer);
            }

            if (money == 0)
            {
                *--pBuffer = '0';
            }
            else
            {
                int digitGroupCount = 0;

                do
                {
                    *--pBuffer = static_cast<char>((money % 10) + '0');
                    money /= 10;

                    if (++digitGroupCount == 3 && money)
                    {
                        digitGroupCount = 0;
                        *--pBuffer = thousandsSymbol;
                    }

                } while (money > 0);

                if (isNeg && appendCurrency)
                {
                    *--pBuffer = '-';
                }
            }

            if (!appendCurrency)
            {
                pBuffer = CopyCurrency(pBuffer);

                if (isNeg)
                {
                    *--pBuffer = '-';
                }
            }

            strncpy_s(pFinalBuffer, finalBufferLen, pBuffer, _TRUNCATE);

            return pFinalBuffer;
        }
    }

} // namespace Money

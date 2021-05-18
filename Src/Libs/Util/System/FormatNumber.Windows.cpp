// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FormatNumber.h"
#include "_lean_windows_.h"
//#include <cstdio>
//#include <cstdlib>

using namespace std;

static UINT GetLocaleNumInfo(DWORD infoType)
{
    char buffer[100];
    GetLocaleInfoA(LOCALE_USER_DEFAULT, infoType, buffer, static_cast<int>(sizeof(buffer)));
    return static_cast<UINT>(atoi(buffer));
}

struct LocaleData
{
    NUMBERFMTW   numberFormat;
    CURRENCYFMTW currencyFormat;
    wchar_t      decimalSep[4];
    wchar_t      thousandSep[4];
    wchar_t      currencySymbol[16];

    LocaleData()
    {
        memset(this, 0, sizeof(*this));
    }

    void set()
    {
        GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, decimalSep, static_cast<int>(sizeof(decimalSep) / sizeof(wchar_t)));
        numberFormat.lpDecimalSep =
            currencyFormat.lpDecimalSep = decimalSep;

        GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, thousandSep, static_cast<int>(sizeof(thousandSep) / sizeof(wchar_t)));
        numberFormat.lpThousandSep =
            currencyFormat.lpThousandSep = thousandSep;

        numberFormat.NumDigits   = GetLocaleNumInfo(LOCALE_IDIGITS);
        currencyFormat.NumDigits = GetLocaleNumInfo(LOCALE_ICURRDIGITS);

        numberFormat.Grouping =
            currencyFormat.Grouping = GetLocaleNumInfo(LOCALE_SGROUPING);

        numberFormat.LeadingZero =
            currencyFormat.LeadingZero = GetLocaleNumInfo(LOCALE_ILZERO);

        numberFormat.NegativeOrder   = GetLocaleNumInfo(LOCALE_INEGNUMBER);
        currencyFormat.NegativeOrder = GetLocaleNumInfo(LOCALE_INEGCURR);
        currencyFormat.PositiveOrder = GetLocaleNumInfo(LOCALE_ICURRENCY);

        wchar_t emptyStr[1] = L"";
        currencyFormat.lpCurrencySymbol = emptyStr;
        GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, currencySymbol, static_cast<int>(sizeof(currencySymbol) / sizeof(wchar_t)));
    }
};

FormatNumber::FormatNumber()
{
    LocaleData localeData;
    _localeData.resize(sizeof(localeData));
    auto pLocaleData = reinterpret_cast<LocaleData*>(_localeData.data());
    pLocaleData->set();
}

wchar_t* FormatNumber::moneyCents(__int64 amountInCents, wchar_t* pBuffer, size_t bufferLen, bool useCurrencySymbol) const
{
    wchar_t tmpStr[32];
    _snwprintf_s(tmpStr, _TRUNCATE, L"%I64d.%02I64d", amountInCents / 100, amountInCents % 100);

    return money(tmpStr, pBuffer, bufferLen, useCurrencySymbol, false);
}

wstring FormatNumber::moneyCents(__int64 amountInCents, bool useCurrencySymbol) const
{
    wchar_t tmpStr[100];
    return moneyCents(amountInCents, tmpStr, useCurrencySymbol);
}

wchar_t* FormatNumber::moneyDollars(__int64 amountInDollars, wchar_t* pBuffer, size_t bufferLen, bool useCurrencySymbol) const
{
    wchar_t tmpStr[32];
    _snwprintf_s(tmpStr, _TRUNCATE, L"%I64d", amountInDollars);

    return money(tmpStr, pBuffer, bufferLen, useCurrencySymbol, true);
}

wstring FormatNumber::moneyDollars(__int64 amountInDollars, bool useCurrencySymbol) const
{
    wchar_t tmpStr[100];
    return moneyDollars(amountInDollars, tmpStr, useCurrencySymbol);
}

wchar_t* FormatNumber::money(const wchar_t* pValue, wchar_t* pBuffer, size_t bufferLen, bool useCurrencySymbol, bool stripCents) const
{
    if (pBuffer && bufferLen && pValue)
    {
        *pBuffer = 0;

        auto pLocaleData = reinterpret_cast<const LocaleData*>(_localeData.data());

        CURRENCYFMTW format = pLocaleData->currencyFormat;
        
        if (useCurrencySymbol)
        {
            // Casting away const is benign since GetCurrencyFormatW takes a const pointer 
            format.lpCurrencySymbol = const_cast<wchar_t*>(pLocaleData->currencySymbol);
        }

        if (stripCents)
        {
            format.NumDigits = 0;
        }

        GetCurrencyFormatW(LOCALE_USER_DEFAULT, 0, pValue, &format, pBuffer, static_cast<int>(bufferLen));
    }

    return pBuffer;
}

wstring FormatNumber::money(const wchar_t* pValue, bool useCurrencySymbol, bool stripCents) const
{
    wchar_t tmpStr[100];
    return money(pValue, tmpStr, useCurrencySymbol, stripCents);
}

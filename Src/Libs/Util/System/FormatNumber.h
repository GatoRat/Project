#pragma once

#include <string>
#include <vector>

class FormatNumber
{
public:
    FormatNumber();
    FormatNumber(const FormatNumber&) = delete;
    void operator=(const FormatNumber&) = delete;

    wchar_t* moneyCents(__int64 amountInCents, wchar_t* pBuffer, size_t bufferLen, bool useCurrencySymbol) const;
    std::wstring moneyCents(__int64 amountInDollars, bool useCurrencySymbol = true) const;

    template <size_t _size>
    wchar_t* moneyCents(__int64 amountInDollars, wchar_t(&dst)[_size], bool useCurrencySymbol = true) const
    {
        return moneyCents(amountInDollars, dst, _size, useCurrencySymbol);
    }

    wchar_t* moneyDollars(__int64 amountInDollars, wchar_t* pBuffer, size_t bufferLen, bool useCurrencySymbol = true) const;
    std::wstring moneyDollars(__int64 amountInDollars, bool useCurrencySymbol = true) const;

    template <size_t _size>
    wchar_t* moneyDollars(__int64 amountInDollars, wchar_t(&dst)[_size], bool useCurrencySymbol = true) const
    {
        return moneyDollars(amountInDollars, dst, _size, useCurrencySymbol);
    }

    wchar_t* money(const wchar_t* pValue, wchar_t* pBuffer, size_t bufferLen, bool useCurrencySymbol = true, bool stripCents = false) const;
    std::wstring money(const wchar_t* pValue, bool useCurrencySymbol = true, bool stripCents = false) const;

    template <size_t _size>
    wchar_t* money(const wchar_t* pValue, wchar_t(&dst)[_size], bool useCurrencySymbol = true, bool stripCents = false) const
    {
        return money(pValue, dst, _size, useCurrencySymbol, stripCents);
    }

private:
    std::vector<uint8_t> _localeData;
};

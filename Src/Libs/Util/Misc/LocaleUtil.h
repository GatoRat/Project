// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <locale>

namespace Locale
{
    extern const char* pDefault;

    struct NumFacet
    {
        std::locale loc;
        const std::numpunct<char>& facet;

        // This will throw a base_cast exception if the global locale does not have the numpunct facet.
        NumFacet();
        explicit NumFacet(const std::locale* pLocale);
        NumFacet(const NumFacet&) = delete;
        void operator=(const NumFacet&) = delete;
        ~NumFacet() = default;
    };

    struct MoneyFacet
    {
        std::locale loc;
        const std::moneypunct<char>& facet;

        // This will throw a base_cast exception if the global locale does not have the moneypunct facet.
        MoneyFacet();
        explicit MoneyFacet(const std::locale* pLocale);
        MoneyFacet(const MoneyFacet&) = delete;
        void operator=(const MoneyFacet&) = delete;
        ~MoneyFacet() = default;
    };
}; // namespace Locale

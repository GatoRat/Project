// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "LocaleUtil.h"

using namespace std;

namespace Locale
{
#ifdef _WIN32
    const char* pDefault = "en-US";
#else
    const char* pDefault = "en_US.utf8";
#endif

    NumFacet::NumFacet()
        : facet(use_facet<numpunct<char>>(loc))
    {
    }

    NumFacet::NumFacet(locale* pLocale)
        : loc(pLocale && has_facet<numpunct<char>>(*pLocale) ? pLocale->name() : Locale::pDefault)
        , facet(use_facet<numpunct<char>>(loc))
    {
    }

    MoneyFacet::MoneyFacet()
        : facet(use_facet<moneypunct<char>>(loc))
    {
    }

    MoneyFacet::MoneyFacet(locale* pLocale)
        : loc(pLocale && has_facet<moneypunct<char>>(*pLocale) ? pLocale->name() : Locale::pDefault)
        , facet(use_facet<moneypunct<char>>(loc))
    {
    }
}; // namespace Locale

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/FormatSize.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("string/FormatNumber::Size(uint64_t, en-US)")
{
    struct
    {
        uint64_t val;
        FormatSizeStyle style;
        const char* pResult;
    }
    tests[] =
    {
        { 0, FormatSizeStyle::None, "0" },
        { 0, FormatSizeStyle::OneDecimal, "0.0" },
        { 0, FormatSizeStyle::TwoDecimals, "0.00" },
        { 0, FormatSizeStyle::None | FormatSizeStyle::NoDecimalIfZero, "0" },
        { 0, FormatSizeStyle::OneDecimal | FormatSizeStyle::NoDecimalIfZero, "0" },
        { 0, FormatSizeStyle::TwoDecimals | FormatSizeStyle::NoDecimalIfZero, "0" },

        { 0, FormatSizeStyle::None | FormatSizeStyle::Abbrev_TwoLetter, "0 bytes" },
        { 0, FormatSizeStyle::OneDecimal | FormatSizeStyle::Abbrev_TwoLetter, "0.0 bytes" },
        { 0, FormatSizeStyle::TwoDecimals | FormatSizeStyle::Abbrev_TwoLetter, "0.00 bytes" },
        { 0, FormatSizeStyle::None | FormatSizeStyle::NoDecimalIfZero | FormatSizeStyle::Abbrev_TwoLetter, "0 bytes" },
        { 0, FormatSizeStyle::OneDecimal | FormatSizeStyle::NoDecimalIfZero | FormatSizeStyle::Abbrev_TwoLetter, "0 bytes" },
        { 0, FormatSizeStyle::TwoDecimals | FormatSizeStyle::NoDecimalIfZero | FormatSizeStyle::Abbrev_TwoLetter, "0 bytes" },

        { 0, FormatSizeStyle::None | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::Abbrev_NoSpace, "0bytes" },
        { 0, FormatSizeStyle::OneDecimal | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::Abbrev_NoSpace, "0.0bytes" },
        { 0, FormatSizeStyle::TwoDecimals | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::Abbrev_NoSpace, "0.00bytes" },
        { 0, FormatSizeStyle::None | FormatSizeStyle::NoDecimalIfZero | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::Abbrev_NoSpace, "0bytes" },
        { 0, FormatSizeStyle::OneDecimal | FormatSizeStyle::NoDecimalIfZero | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::Abbrev_NoSpace, "0bytes" },
        { 0, FormatSizeStyle::TwoDecimals | FormatSizeStyle::NoDecimalIfZero | FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::Abbrev_NoSpace, "0bytes" },
    };

#ifdef __linux__
    locale loc("en_US.utf8");
#else
    locale loc("en-US");
#endif

    for (auto test : tests)
    {
        string result = FormatSize(test.val, test.style, &loc);
        REQUIRE(!result.empty()); //-V521
        REQUIRE(result == test.pResult); //-V521
    }
}

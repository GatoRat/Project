// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FormatSize.h"
#include <iomanip>
#include <locale>
#include <sstream>
#include "ASCII.h"
#include "Convert.h"
#include "Str.h"

using namespace std;

static const char* pSingleByte    = "byte";
static const char* pMultipleBytes = "bytes";
static const char* pSymbols[2][6]
{
    { "",  "KB" , "MB" , "GB" , "TB" , nullptr },
    { "",  "KiB", "MiB", "GiB", "TiB", nullptr }
};

constexpr uint64_t Kilo = 1000;
constexpr uint64_t Kibi = 1024;

static string FormatSize(uint64_t val, FormatSizeStyle style, bool isNeg, const locale* pLocale)
{
    string retStr;

    int decimals;
    double multiplier;

    if (style & FormatSizeStyle::OneDecimal)
    {
        decimals = 1;
        multiplier = 100.0;
    }
    else if (style & FormatSizeStyle::TwoDecimals)
    {
        decimals = 2;
        multiplier = 1000.0;
    }
    else
    {
        decimals = 0;
        multiplier = 1.0;
    }

    const char* pBytesStr = val == 1 ? pSingleByte : pMultipleBytes;
    int symbol = 0;

    ostringstream ss;
    ss.imbue(pLocale ? *pLocale : locale());

    if (val == 0)
    {
        if (decimals && (style & FormatSizeStyle::NoDecimalIfZero) == 0)
        {
            ss << fixed << setprecision(decimals) << static_cast<double>(val);
            retStr = ss.str();
        }
        else
        {
            retStr = "0";
        }
    }
    else
    {
        uint64_t multiple;

        if (style & FormatSizeStyle::StrictCalculation)
        {
            multiple = (style & FormatSizeStyle::Abbrev_IEEE_1541) ? Kibi : Kilo;
        }
        else
        {
            multiple = Kibi;
        }

        unsigned remainder = 0;

        do
        {
            if (val < multiple)
            {
                break;
            }

            symbol++;

            remainder = static_cast<unsigned>(val % multiple);
            val /= multiple;

        } while (pSymbols[0][symbol + 1]);

        if (decimals)
        {
            remainder = static_cast<unsigned>((remainder / static_cast<double>(multiple)) * multiplier);
            const unsigned tmpRemainder = remainder % 10;
            remainder /= 10;

            if (tmpRemainder > 5)
            {
                if (++remainder == 10)
                {
                    remainder = 0;

                    if (isNeg)
                    {
                        val--;
                    }
                    else
                    {
                        val++;
                    }
                }
            }

            ss << fixed << setprecision(decimals) <<
                  static_cast<double>(isNeg ? -static_cast<int64_t>(val) : static_cast<int64_t>(val)) + (remainder / (multiplier / 10.0));
        }
        else
        {
            remainder = static_cast<unsigned>((remainder / static_cast<double>(multiple)) * 10);
            remainder %= 10;

            if (remainder > 5)
            {
                if (isNeg)
                {
                    val--;
                }
                else
                {
                    val++;
                }
            }

            ss << (isNeg ? -static_cast<int64_t>(val) : static_cast<uint64_t>(val));
        }
        retStr = ss.str();
    }

    if (style & (FormatSizeStyle::Abbrev_TwoLetter | FormatSizeStyle::Abbrev_IEEE_1541))
    {
        if (!(style & FormatSizeStyle::Abbrev_NoSpace))
        {
            retStr += ASCII::Space;
        }

        const char* pAbbrev = pSymbols[(style & FormatSizeStyle::Abbrev_IEEE_1541) ? 1 : 0][symbol];
        retStr += *pAbbrev ? pAbbrev : pBytesStr;
    }

    return retStr;
}

string FormatSize(uint64_t val, FormatSizeStyle style, const locale* pLocale)
{
    return FormatSize(val, style, false, pLocale);
}

string FormatSize(int64_t val, FormatSizeStyle style, const locale* pLocale)
{
    return val < 0 ? FormatSize(static_cast<uint64_t>(-val), style, true , pLocale) :
                     FormatSize(static_cast<uint64_t>( val), style, false, pLocale);
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FormatSizeFast.h"

using namespace std;

string FormatSizeFast(uint64_t val, bool isNeg)
{
    constexpr size_t MinBufferLen64 = 27; // -9,223,372,036,854,775,807
    char buffer[MinBufferLen64];

    char* pBuffer = &buffer[sizeof(buffer)];
    *--pBuffer = 0;

    if (val == 0)
    {
        *--pBuffer = '0';
    }
    else
    {
        constexpr int DigitsPerGroup = 3;
        int digitGroupCount = 0;

        do
        {
            *--pBuffer = static_cast<char>((val % 10) + '0');
            val /= 10;

            if (++digitGroupCount == DigitsPerGroup && val)
            {
                digitGroupCount = 0;
                *--pBuffer = ',';
            }

        } while (val > 0);

        if (isNeg)
        {
            *--pBuffer = '-';
        }
    }

    return string(pBuffer);
}

string FormatSizeFast(int64_t val)
{
    return val < 0 ?
        FormatSizeFast(static_cast<uint64_t>(-val), true) :
        FormatSizeFast(static_cast<uint64_t>(val), false);
}

string FormatMemorySizeFast(uint64_t val, unsigned decimalPlaces, bool isNeg)
{
    string formattedString;

    static const uint64_t Kilobyte{ 1024 };
    static const char* pSymbols[6]{ nullptr, " KB" , " MB" , " GB" , " TB" , nullptr };

    const char* pBytesStr = val == 1 ? " byte" : " bytes";

    int symbol = 0;
    uint64_t remainder = 0;

    //do
    //{
    //    if (val < Kilobyte)
    //    {
    //        break;
    //    }

    //    remainder = static_cast<uint64_t>(val % Kilobyte);
    //    val /= Kilobyte;

    //} while (pSymbols[++symbol]);

    while (val >= Kilobyte)
    {
        remainder = static_cast<uint64_t>(val % Kilobyte);
        val /= Kilobyte;

        if (!pSymbols[symbol + 1])
        {
            break;
        }

        ++symbol;
    }

    if (decimalPlaces)
    {
        double multiplier;
        uint64_t roundRemainder;

        if (decimalPlaces == 1)
        {
            multiplier = 100.0;
            roundRemainder = 10;
        }
        else
        {
            decimalPlaces = 2;
            multiplier = 1000.0;
            roundRemainder = 100;
        }

        remainder = static_cast<uint64_t>((remainder / static_cast<double>(Kilobyte)) * multiplier);
        const uint64_t tmpRemainder = remainder % 10;
        remainder /= 10;

        if (tmpRemainder > 5)
        {
            if (++remainder == roundRemainder)
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

        formattedString = FormatSizeFast(val, isNeg);

        if (symbol)
        {
            formattedString += '.';

            if (decimalPlaces == 1)
            {
                formattedString += static_cast<char>(remainder + '0');
            }
            else
            {
                formattedString += static_cast<char>((remainder / 10) + '0');
                formattedString += static_cast<char>((remainder % 10) + '0');
            }
        }
    }
    else
    {
        remainder = static_cast<uint64_t>((remainder / static_cast<double>(Kilobyte)) * 10);
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

        formattedString = FormatSizeFast(val, isNeg);
    }

    formattedString += symbol ? pSymbols[symbol] : pBytesStr;

    return formattedString;
}

string FormatMemorySizeFast(int64_t val, unsigned decimalPlaces)
{
    return val < 0 ?
        FormatMemorySizeFast(static_cast<uint64_t>(-val), decimalPlaces, true) :
        FormatMemorySizeFast(static_cast<uint64_t>(val), decimalPlaces, false);
}

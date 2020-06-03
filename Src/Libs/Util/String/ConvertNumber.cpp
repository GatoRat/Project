// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../Misc/Defines.Misc.h"
#include <algorithm>
#include "ASCII.h"
#include "Convert.h"
#include "GetNumDigits.h"
#include "Str.h"

using namespace std;

namespace Convert
{
    static const int DigitsPerGroup = 3;

    char* ToStr(uint32_t val, char* pBuffer, char groupSeparatorChar, bool isNeg, char** pEnd)
    {
        pBuffer += groupSeparatorChar ? (isNeg ? MaxStrLen32_Commas_Signed - 1 : MaxStrLen32_Commas - 1) :
                                        (isNeg ? MaxStrLen32_Signed - 1        : MaxStrLen32 - 1       );
        *pBuffer = 0;

        if (pEnd)
        {
            *pEnd = pBuffer;
        }

        if (val == 0)
        {
            *--pBuffer = ASCII::Zero;
        }
        else
        {
            int digitGroupCount = 0;

            do
            {
                *--pBuffer = static_cast<char>((val % BASE_TEN) + ASCII::Zero);
                val /= BASE_TEN;

                if (groupSeparatorChar && ++digitGroupCount == DigitsPerGroup && val)
                {
                    digitGroupCount = 0;
                    *--pBuffer = groupSeparatorChar;
                }

            } while (val);

            if (isNeg)
            {
                *--pBuffer = ASCII::Minus;
            }
        }

        return pBuffer;
    }

    char* ToStr(int32_t val, char* pBuffer, char groupSeparatorChar)
    {
        return val < 0 ?
            ToStr(static_cast<uint32_t>(-val), pBuffer, groupSeparatorChar, true) :
            ToStr(static_cast<uint32_t>(val), pBuffer, groupSeparatorChar, false);
    }

    char* ToStrAligned(uint32_t val, char* pBuffer, char groupSeparatorChar, bool isNeg, char** pEnd)
    {
        if (val == 0)
        {
            pBuffer[0] = ASCII::Zero;
            pBuffer[1] = 0;
        }
        else
        {
            const char* pResult = ToStr(val, pBuffer, groupSeparatorChar, isNeg, nullptr);
            if (pResult != pBuffer)
            {
                size_t i = 0;
                do
                {
                    pBuffer[i] = pResult[i];

                } while (pBuffer[i++]);

                if (pEnd)
                {
                    *pEnd = &pBuffer[i];
                }
            }
        }

        return pBuffer;
    }

    char* ToStrAligned(int32_t val, char* pBuffer, char groupSeparatorChar)
    {
        return val < 0 ?
            ToStrAligned(static_cast<uint32_t>(-val), pBuffer, groupSeparatorChar, true) :
            ToStrAligned(static_cast<uint32_t>(val), pBuffer, groupSeparatorChar, false);
    }

    /////////////////////////////////////////////////////////////////////////
    
    char* ToStr64(uint64_t val, char* pBuffer, char groupSeparatorChar, bool isNeg, char** pEnd)
    {
        pBuffer += groupSeparatorChar ? (isNeg ? MaxStrLen64_Commas_Signed - 1 : MaxStrLen64_Commas - 1) :
                                        (isNeg ? MaxStrLen64_Signed - 1        : MaxStrLen64 - 1       );
        *pBuffer = 0;

        if (pEnd)
        {
            *pEnd = pBuffer;
        }

        if (val == 0)
        {
            *--pBuffer = ASCII::Zero;
        }
        else
        {
            if (isNeg && val > 9223372036854775807)
            {
                val = 9223372036854775807;
            }

            int digitGroupCount = 0;

            do
            {
                *--pBuffer = static_cast<char>((val % BASE_TEN) + ASCII::Zero);
                val /= BASE_TEN;

                if (groupSeparatorChar && ++digitGroupCount == DigitsPerGroup && val)
                {
                    digitGroupCount = 0;
                    *--pBuffer = groupSeparatorChar;
                }

            } while (val > 0);

            if (isNeg)
            {
                *--pBuffer = ASCII::Minus;
            }
        }

        return pBuffer;
    }

    char* ToStr64Padded(uint64_t val, char* pBuffer, int minWidth, char padChar, char groupSeparatorChar, bool isNeg, char** pPassedEnd)
    {
        char* pEnd;
        char* pStr = ToStr64(val, pBuffer, groupSeparatorChar, isNeg, &pEnd);

        if (padChar && pStr > pBuffer)
        {
            ptrdiff_t numPadChars = min(static_cast<ptrdiff_t>(minWidth) - (pEnd - pStr), pStr - pBuffer);
            if (numPadChars > 0)
            {
                pStr -= numPadChars;

                if (isNeg && val)
                {
                    memset(pStr + 1, padChar, numPadChars);
                    *pStr = ASCII::Minus;
                }
                else
                {
                    memset(pStr, padChar, numPadChars);
                }
            }
        }

        if (pPassedEnd)
        {
            *pPassedEnd = pEnd;
        }

        return pStr;
    }

    char* ToStr64(int64_t val, char* pBuffer, char groupSeparatorChar)
    {
        return val < 0 ?
            ToStr64(static_cast<uint64_t>(-val), pBuffer, groupSeparatorChar, true, nullptr) :
            ToStr64(static_cast<uint64_t>(val), pBuffer, groupSeparatorChar, false, nullptr);
    }

    char* ToStrAligned64(uint64_t val, char* pBuffer, char groupSeparatorChar, bool isNeg, char** pEnd)
    {
        if (val == 0)
        {
            pBuffer[0] = ASCII::Zero;
            pBuffer[1] = 0;
        }
        else
        {
            const char* pResult = ToStr64(val, pBuffer, groupSeparatorChar, isNeg, nullptr);
            if (pResult != pBuffer)
            {
                size_t i = 0;
                do
                {
                    pBuffer[i] = pResult[i];

                } while (pBuffer[i++]);

                if (pEnd)
                {
                    *pEnd = &pBuffer[i];
                }
            }
        }

        return pBuffer;
    }

    char* ToStrAligned64(int64_t val, char* pBuffer, char groupSeparatorChar)
    {
        return val < 0 ?
            ToStrAligned64(static_cast<uint64_t>(-val), pBuffer, groupSeparatorChar, true) :
            ToStrAligned64(static_cast<uint64_t>(val), pBuffer, groupSeparatorChar, false);
    }

    string& ToStr64(string& str, uint64_t val, size_t minWidth, char fillChar, char groupSeparatorChar)
    {
        char tmpStr[MinBufferLen];

        char* pEnd;
        const char* pStr = ToStr64(val, tmpStr, groupSeparatorChar, false, &pEnd);

        const size_t width = pEnd - pStr;
        if (width < minWidth)
        {
            str.reserve(minWidth);
            str.append(minWidth - width, fillChar);
        }

        str.append(pStr, width);

        return str;
    }

    char* ToStrFixedFast(char* pStr, uint64_t val, size_t width)
    {
        if (pStr && width)
        {
            size_t tmpWidth = width;
            while (val && tmpWidth)
            {
                pStr[--tmpWidth] = static_cast<char>((val % BASE_TEN) + ASCII::Zero);
                val /= BASE_TEN;
            }

            memset(pStr, ASCII::Zero, tmpWidth);
            pStr += width;
        }
        return pStr;
    }

    string& AppendFixedFast(string& str, uint64_t val, size_t width)
    {
        if (width)
        {
            str.append(width, ASCII::Zero); //-V112
            char* pDst = str.data() + (str.size() - width);

            while (val && width)
            {
                pDst[--width] = static_cast<char>((val % BASE_TEN) + ASCII::Zero);
                val /= BASE_TEN;
            }
        }
        return str;
    }

} // namespace Convert

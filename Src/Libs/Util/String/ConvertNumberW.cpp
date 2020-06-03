// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../Misc/Defines.Misc.h"
#include <algorithm>
#include "ASCIIW.h"
#include "ConvertW.h"
#include "GetNumDigits.h"
#include "Str.h"

using namespace std;

namespace ConvertW
{
    static const int DigitsPerGroup = 3;

    wchar_t* ToStr32U(uint32_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar, bool isNeg, wchar_t** pEnd)
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
            *--pBuffer = ASCIIW::Zero;
        }
        else
        {
            int digitGroupCount = 0;

            do
            {
                *--pBuffer = static_cast<wchar_t>((val % BASE_TEN) + ASCIIW::Zero);
                val /= BASE_TEN;

                if (groupSeparatorChar && ++digitGroupCount == DigitsPerGroup && val)
                {
                    digitGroupCount = 0;
                    *--pBuffer = groupSeparatorChar;
                }

            } while (val);

            if (isNeg)
            {
                *--pBuffer = ASCIIW::Minus;
            }
        }

        return pBuffer;
    }

    wchar_t* ToStr32(int32_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar)
    {
        return val < 0 ?
            ToStr32U(static_cast<uint32_t>(-val), pBuffer, groupSeparatorChar, true) :
            ToStr32U(static_cast<uint32_t>(val) , pBuffer, groupSeparatorChar, false);
    }

    wchar_t* ToStrAligned32U(uint32_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar, bool isNeg, wchar_t** pEnd)
    {
        if (val == 0)
        {
            pBuffer[0] = ASCIIW::Zero;
            pBuffer[1] = 0;
        }
        else
        {
            const wchar_t* pResult = ToStr32U(val, pBuffer, groupSeparatorChar, isNeg, nullptr);
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

    wchar_t* ToStrAligned32(int32_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar)
    {
        return val < 0 ?
            ToStrAligned32U(static_cast<uint32_t>(-val), pBuffer, groupSeparatorChar, true) :
            ToStrAligned32U(static_cast<uint32_t>(val) , pBuffer, groupSeparatorChar, false);
    }

    /////////////////////////////////////////////////////////////////////////
    
    wchar_t* ToStr64U(uint64_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar, bool isNeg, wchar_t** pEnd)
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
            *--pBuffer = ASCIIW::Zero;
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
                *--pBuffer = static_cast<wchar_t>((val % BASE_TEN) + ASCIIW::Zero);
                val /= BASE_TEN;

                if (groupSeparatorChar && ++digitGroupCount == DigitsPerGroup && val)
                {
                    digitGroupCount = 0;
                    *--pBuffer = groupSeparatorChar;
                }

            } while (val > 0);

            if (isNeg)
            {
                *--pBuffer = ASCIIW::Minus;
            }
        }

        return pBuffer;
    }

    wchar_t* ToStrPadded64U(uint64_t val, wchar_t* pBuffer, int minWidth, wchar_t padChar, wchar_t groupSeparatorChar, bool isNeg, wchar_t** pPassedEnd)
    {
        wchar_t* pEnd;
        wchar_t* pStr = ToStr64U(val, pBuffer, groupSeparatorChar, isNeg, &pEnd);

        if (padChar && pStr > pBuffer)
        {
            ptrdiff_t numPadChars = min(static_cast<ptrdiff_t>(minWidth) - (pEnd - pStr), pStr - pBuffer);
            if (numPadChars > 0)
            {
                pStr -= numPadChars;

                if (isNeg && val)
                {
                    wmemset(pStr + 1, padChar, numPadChars);
                    *pStr = ASCIIW::Minus;
                }
                else
                {
                    wmemset(pStr, padChar, numPadChars);
                }
            }
        }

        if (pPassedEnd)
        {
            *pPassedEnd = pEnd;
        }

        return pStr;
    }

    wchar_t* ToStr64(int64_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar)
    {
        return val < 0 ?
            ToStr64U(static_cast<uint64_t>(-val), pBuffer, groupSeparatorChar, true, nullptr) :
            ToStr64U(static_cast<uint64_t>(val), pBuffer, groupSeparatorChar, false, nullptr);
    }

    wchar_t* ToStrAligned64U(uint64_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar, bool isNeg, wchar_t** pEnd)
    {
        if (val == 0)
        {
            pBuffer[0] = ASCIIW::Zero;
            pBuffer[1] = 0;
        }
        else
        {
            const wchar_t* pResult = ToStr64U(val, pBuffer, groupSeparatorChar, isNeg, nullptr);
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

    wchar_t* ToStrAligned64(int64_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar)
    {
        return val < 0 ?
            ToStrAligned64U(static_cast<uint64_t>(-val), pBuffer, groupSeparatorChar, true) :
            ToStrAligned64U(static_cast<uint64_t>(val), pBuffer, groupSeparatorChar, false);
    }

    wstring& ToStr64U(wstring& str, uint64_t val, size_t minWidth, wchar_t fillChar, wchar_t groupSeparatorChar)
    {
        wchar_t tmpStr[MinBufferLen];

        wchar_t* pEnd;
        const wchar_t* pStr = ToStr64U(val, tmpStr, groupSeparatorChar, false, &pEnd);

        const size_t width = pEnd - pStr;
        if (width < minWidth)
        {
            str.reserve(minWidth);
            str.append(minWidth - width, fillChar);
        }

        str.append(pStr, width);

        return str;
    }

    wchar_t* ToStrFixedFast(wchar_t* pStr, uint64_t val, size_t width)
    {
        if (pStr && width)
        {
            size_t tmpWidth = width;
            while (val && tmpWidth)
            {
                pStr[--tmpWidth] = static_cast<wchar_t>((val % BASE_TEN) + L'0');
                val /= BASE_TEN;
            }

            wmemset(pStr, ASCIIW::Zero, tmpWidth);
            pStr += width;
        }
        return pStr;
    }

    wstring& AppendFixedFast(wstring& str, uint64_t val, size_t width)
    {
        if (width)
        {
            str.append(width, ASCIIW::Zero); //-V112
            wchar_t* pDst = str.data() + (str.size() - width);

            while (val && width)
            {
                pDst[--width] = static_cast<wchar_t>((val % BASE_TEN) + ASCIIW::Zero);
                val /= BASE_TEN;
            }
        }
        return str;
    }

} // namespace Convert

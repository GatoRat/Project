// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ConvertW.h"
#include "../Misc/Defines.Misc.h"
#include "../System/_lean_windows_.h"
#include "ASCIIW.h"
#include "Str.h"

using namespace std;

namespace ConvertW
{
    wstring ToStr(const wchar_t* pStr, size_t strLen)
    {
        return pStr && *pStr && strLen ? wstring(pStr, strLen) : wstring();
    }

    wstring_view ToStrView(const wchar_t* pStr)
    {
        return pStr && *pStr ? wstring_view(pStr) : wstring_view();
    }

    wstring_view ToStrView(const wchar_t* pStr, size_t strLen)
    {
        return pStr && *pStr && strLen ? wstring_view(pStr, strLen) : wstring_view();
    }

    wstring_view ToStrViewMaxLen(const wchar_t* pStr, size_t maxLen)
    {
        return pStr && *pStr && maxLen ? wstring_view(pStr, Str::GetLenMax(pStr, maxLen)) : wstring_view();
    }


    uint64_t ToUint64(const wchar_t* pStr, ToUint64Err& err, const wchar_t** pEnd, uint64_t maxValue, bool allowCommas)
    {
        uint64_t val = 0;
        err = ToUint64_Success;

        if (pStr)
        {
            if (ASCIIW::IsDigit(*pStr))
            {
                const uint32_t lastDigit = maxValue % BASE_TEN;
                maxValue /= BASE_TEN;

                while (*pStr)
                {
                    if (val <= maxValue)
                    {
                        if (ASCIIW::IsDigit(*pStr))
                        {
                            const uint32_t chVal = static_cast<uint32_t>(ASCIIW::DigitToVal(*pStr));
                            if (val == maxValue && chVal > lastDigit)
                            {
                                err = ToUint64_Overflow;
                                break;
                            }

                            val *= BASE_TEN;
                            val += chVal;
                        }
                        else if (!(allowCommas && *pStr == ASCIIW::Comma))
                        {
                            err = ToUint64_EndWithChar;
                            break;
                        }
                    }
                    else
                    {
                        err = ToUint64_Overflow;
                        break;
                    }

                    ++pStr;
                }
            }
            else if (*pStr)
            {
                err = ToUint64_NotNumber;
            }
            else
            {
                err = ToUint64_Empty;
            }
        }
        else
        {
            err = ToUint64_Null;
        }

        if (pEnd)
        {
            *pEnd = pStr;
        }

        return val;
    }

    bool ToUint64(const wchar_t* pStr, uint64_t& val, const wchar_t** pEnd, bool allowCommas)
    {
        bool success = false;

        if (pStr)
        {
            while (ASCIIW::IsWhitespace(*pStr))
            {
                ++pStr;
            }

            ToUint64Err err;
            val = ToUint64(pStr, err, pEnd, UINT64_MAX, allowCommas);
            success = err >= ToUint64_Success;
        }
        else
        {
            val = 0;

            if (pEnd)
            {
                *pEnd = pStr;
            }
        }

        return success;
    }

    bool ToInt64(const wchar_t* pStr, int64_t& val, const wchar_t** pEnd, bool allowCommas)
    {
        bool success = false;

        if (pStr)
        {
            while (ASCIIW::IsWhitespace(*pStr))
            {
                ++pStr;
            }

            ToUint64Err err = ToUint64_Success;

            if (*pStr == ASCIIW::Minus)
            {
                val = -static_cast<int64_t>(ToUint64(++pStr, err, pEnd, static_cast<uint64_t>(INT64_MAX) + 1, allowCommas));
            }
            else
            {
                if (*pStr == '+')
                {
                    ++pStr;
                }

                val = static_cast<int64_t>(ToUint64(pStr, err, pEnd, INT64_MAX, allowCommas));
            }

            success = err >= ToUint64_Success;
        }
        else
        {
            val = 0;

            if (pEnd)
            {
                *pEnd = pStr;
            }
        }

        return success;
    }

    bool ToUint32(const wchar_t* pStr, uint32_t& val, const wchar_t** pEnd, bool allowCommas)
    {
        bool success = false;

        if (pStr)
        {
            while (ASCIIW::IsWhitespace(*pStr))
            {
                ++pStr;
            }

            ToUint64Err err;
            val = static_cast<uint32_t>(ToUint64(pStr, err, pEnd, UINT32_MAX, allowCommas));
            success = err >= ToUint64_Success;
        }
        else
        {
            val = 0;

            if (pEnd)
            {
                *pEnd = pStr;
            }
        }

        return success;
    }

    bool ToInt32(const wchar_t* pStr, int32_t& val, const wchar_t** pEnd, bool allowCommas)
    {
        bool success = false;

        if (pStr)
        {
            while (ASCIIW::IsWhitespace(*pStr))
            {
                ++pStr;
            }

            ToUint64Err err = ToUint64_Success;

            if (*pStr == ASCIIW::Minus)
            {
                val = -static_cast<int32_t>(ToUint64(++pStr, err, pEnd, static_cast<uint32_t>(INT32_MAX) + 1));
            }
            else
            {
                if (*pStr == '+')
                {
                    ++pStr;
                }

                val = static_cast<int32_t>(ToUint64(pStr, err, pEnd, INT32_MAX, allowCommas));
            }

            success = err >= ToUint64_Success;
        }
        else
        {
            val = 0;

            if (pEnd)
            {
                *pEnd = pStr;
            }
        }

        return success;
    }

    uint64_t ToUint64_FromView(wstring_view& str, ToUint64Err& err, bool skipLeadingWhitespace, uint64_t maxValue, bool allowCommas)
    {
        uint64_t val = 0;
        err = ToUint64_Success;

        if (skipLeadingWhitespace)
        {
            while (!str.empty() && ASCIIW::IsWhitespace(str.front()))
            {
                str.remove_prefix(1);
            }
        }

        if (!str.empty())
        {
            if (ASCIIW::IsDigit(str.front()))
            {
                const uint32_t lastDigit = maxValue % BASE_TEN;
                maxValue /= BASE_TEN;

                for (; !str.empty(); str.remove_prefix(1))
                {
                    if (val <= maxValue)
                    {
                        const wchar_t ch = str.front();
                        if (ASCIIW::IsDigit(ch))
                        {
                            const uint32_t chVal = static_cast<uint32_t>(ASCIIW::DigitToVal(ch));
                            if (val == maxValue && chVal > lastDigit)
                            {
                                err = ToUint64_Overflow;
                                break;
                            }

                            val *= BASE_TEN;
                            val += chVal;
                        }
                        else if (!(allowCommas && ch == ASCIIW::Comma))
                        {
                            err = ToUint64_EndWithChar;
                            break;
                        }
                    }
                    else
                    {
                        err = ToUint64_Overflow;
                        break;
                    }
                }
            }
            else if (str.front())
            {
                err = ToUint64_NotNumber;
            }
            else
            {
                err = ToUint64_Empty;
            }
        }
        else
        {
            err = ToUint64_Empty;
        }

        return val;
    }

    bool ToUint64_FromView(wstring_view str, uint64_t& val, bool skipLeadingWhitespace, uint64_t maxValue, bool allowCommas)
    {
        ToUint64Err err;
        val = ToUint64_FromView(str, err, skipLeadingWhitespace, maxValue, allowCommas);
        return err >= ToUint64_Success;
    }

    int Quick4(const wchar_t*& pStr)
    {
        int val = -1;

        // This must be done first so a failure will happen if wstring is terminated
        if (ASCIIW::IsDigit(pStr[0]) && ASCIIW::IsDigit(pStr[1]) &&
            ASCIIW::IsDigit(pStr[2]) && ASCIIW::IsDigit(pStr[3]))
        {
            val = ASCIIW::DigitToVal(pStr[0]) * 1000 + ASCIIW::DigitToVal(pStr[1]) * 100 +
                  ASCIIW::DigitToVal(pStr[2]) * 10 + ASCIIW::DigitToVal(pStr[3]);
            pStr += 4; //-V112
        }

        return val;
    }

    int Quick3(const wchar_t*& pStr)
    {
        int val = -1;

        // This must be done first so a failure will happen if wstring is terminated
        if (ASCIIW::IsDigit(pStr[0]) && ASCIIW::IsDigit(pStr[1]) &&
            ASCIIW::IsDigit(pStr[2]))
        {
            val = ASCIIW::DigitToVal(pStr[0]) * 100 + ASCIIW::DigitToVal(pStr[1]) * 10 +
                ASCIIW::DigitToVal(pStr[2]);
            pStr += 3; //-V112
        }

        return val;
    }

    int Quick2(const wchar_t*& pStr)
    {
        int val = -1;

        // This must be done first so a failure will happen if wstring is terminated
        if (ASCIIW::IsDigit(pStr[0]) && ASCIIW::IsDigit(pStr[1]))
        {
            val = ASCIIW::DigitToVal(pStr[0]) * 10 + ASCIIW::DigitToVal(pStr[1]);
            pStr += 2;
        }

        return val;
    }

    ///////////////////////////////////////////////////////////////////////

    wchar_t hexDigits[2][16] =
    {
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' },
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' }
    };

    wchar_t NibbleToHex(uint8_t byteVal, bool upperCase)
    {
        return hexDigits[upperCase ? 1 : 0][byteVal & LowNibble];
    }

    wchar_t HighNibbleToHex(uint8_t byteVal, bool upperCase)
    {
        return hexDigits[upperCase ? 1 : 0][byteVal >> NibbleLenInBits];
    }

    wstring& AppendHex(wstring& str, uint64_t val, bool useUpperCase, unsigned padLen)
    {
        if (padLen > MaxLen_Uint64HexStr)
        {
            padLen = MaxLen_Uint64HexStr;
        }

        if (!val)
        {
            if (padLen)
            {
                str.append(padLen, ASCIIW::Zero); //-V106
            }
            else
            {
                str += ASCIIW::Zero;
            }
        }
        else
        {
            const wchar_t* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
            wchar_t hexStr[MaxLen_Uint64HexStr];
            unsigned offset = MaxLen_Uint64HexStr;

            do
            {
                hexStr[--offset] = pHexDigits[val & LowestNibble];
                val >>= NibbleLenInBits;

            } while (val);

            const unsigned padOffset = MaxLen_Uint64HexStr - padLen;
            while (offset > padOffset)
            {
                hexStr[--offset] = ASCIIW::Zero;
            }

            str.append(&hexStr[offset], sizeof(hexStr) - offset); //-V104
        }

        return str;
    }

    wstring ToHex(uint64_t val, bool useUpperCase, unsigned padLen)
    {
        wstring str;
        str.reserve(MaxLen_Uint64HexStr);
        AppendHex(str, val, useUpperCase, padLen);
        return str;
    }

    wstring& BinToHex(const void* _pData, size_t dataLen, wstring& str, bool useUpperCase, wchar_t separator)
    {
        if (_pData && dataLen)
        {
            str.reserve(dataLen * (CharactersPerByte + (separator ? 1 : 0)));
            const wchar_t* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
            const uint8_t* pData = static_cast<const uint8_t*>(_pData);

            for (size_t i = 0; i < dataLen;)
            {
                str += pHexDigits[pData[i] >> NibbleLenInBits];
                str += pHexDigits[pData[i++] & LowNibble];

                if (separator && i < dataLen)
                {
                    str += separator;
                }
            }
        }
        return str;
    }

    wstring BinToHex(const void* _pData, size_t dataLen, bool useUpperCase, wchar_t separator)
    {
        wstring str;
        BinToHex(_pData, dataLen, str, useUpperCase, separator);
        return str;
    }

    size_t BinToHex(const void* _pData, size_t dataLen, wchar_t* pBuffer, size_t bufferLen, bool useUpperCase, wchar_t separator)
    {
        size_t dstIndex = 0;

        if (pBuffer && bufferLen)
        {
            if (_pData && dataLen)
            {
                bufferLen--;

                const wchar_t* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
                const uint8_t* pData = static_cast<const uint8_t*>(_pData);

                for (size_t srcIndex = 0; srcIndex < dataLen && dstIndex < bufferLen;)
                {
                    pBuffer[dstIndex++] = pHexDigits[pData[srcIndex] >> NibbleLenInBits];
                    if (dstIndex < bufferLen)
                    {
                        pBuffer[dstIndex++] = pHexDigits[pData[srcIndex++] & LowNibble];

                        if (separator && srcIndex < dataLen && dstIndex < bufferLen)
                        {
                            pBuffer[dstIndex++] = separator;
                        }
                    }
                }
            }

            pBuffer[dstIndex] = 0;
        }

        return dstIndex;
    }

    wstring& AppendHex(wstring& str, const void* pData_, size_t dataLen, bool useUpperCase, wchar_t separator)
    {
        if (pData_ && dataLen)
        {
            const wchar_t* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
            const uint8_t* pData = static_cast<const uint8_t*>(pData_);

            str.reserve(str.size() + dataLen * (separator ? 3 : 2));

            for (size_t srcIndex = 0; srcIndex < dataLen; ++srcIndex)
            {
                if (separator && srcIndex)
                {
                    str += separator;
                }

                str += pHexDigits[pData[srcIndex] >> NibbleLenInBits];
                str += pHexDigits[pData[srcIndex] & LowNibble];
            }
        }

        return str;
    }

    wostream& AppendHex(wostream& stream, const void* pData_, size_t dataLen, bool useUpperCase, wchar_t separator)
    {
        if (pData_ && dataLen)
        {
            const wchar_t* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
            const uint8_t* pData = static_cast<const uint8_t*>(pData_);

            for (size_t srcIndex = 0; srcIndex < dataLen; ++srcIndex)
            {
                if (separator && srcIndex)
                {
                    stream << separator;
                }

                stream << pHexDigits[pData[srcIndex] >> NibbleLenInBits];
                stream << pHexDigits[pData[srcIndex] & LowNibble];
            }
        }

        return stream;
    }

} // namespace Convert

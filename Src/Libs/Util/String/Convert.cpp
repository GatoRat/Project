// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Convert.h"
#include "../Misc/Defines.Misc.h"
#include "../Misc/countof.h"
#include "../System/_lean_windows_.h"
#include "ASCII.h"
#include "Str.h"

using namespace std;

namespace Convert
{
    string ToStr(const char* pStr, size_t strLen)
    {
        return pStr && *pStr && strLen ? string(pStr, strLen) : string();
    }

    string_view ToStrView(const char* pStr)
    {
        return pStr && *pStr ? string_view(pStr) : string_view();
    }

    string_view ToStrView(const char* pStr, size_t strLen)
    {
        return pStr && *pStr && strLen ? string_view(pStr, strLen) : string_view();
    }

    string_view ToStrViewMaxLen(const char* pStr, size_t maxLen)
    {
        return pStr && *pStr && maxLen ? string_view(pStr, Str::GetLenMax(pStr, maxLen)) : string_view();
    }
/*
    string ToStr(const char8_t* pStr, size_t strLen)
    {
        return pStr && *pStr && strLen ? string(reinterpret_cast<const char*>(pStr), strLen) : string();
    }

    string_view ToStrView(const char8_t* pStr)
    {
        return pStr && *pStr ? string_view(reinterpret_cast<const char*>(pStr)) : string_view();
    }

    string_view ToStrView(const char8_t* pStr, size_t strLen)
    {
        return pStr && *pStr && strLen ? string_view(reinterpret_cast<const char*>(pStr), strLen) : string_view();
    }

    string_view ToStrViewMaxLen(const char8_t* pStr, size_t maxLen)
    {
        return pStr && *pStr && maxLen ? string_view(reinterpret_cast<const char*>(pStr), Str::GetLenMax(pStr, maxLen)) : string_view();
    }
*/
#ifdef _WIN32
    string ToNarrow(const wchar_t* pStr, int len, unsigned codePage)
    {
        string narrowStr;
        if (pStr && *pStr && len)
        {
            if (len < 0)
            {
                len = static_cast<int>(wcslen(pStr));
            }

            int result = 512;
            narrowStr.resize(result);

            result = ::WideCharToMultiByte(codePage, 0, pStr, len, narrowStr.data(), result, nullptr, nullptr);
            if (result == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                result = ::WideCharToMultiByte(codePage, 0, pStr, len, nullptr, 0, nullptr, nullptr);
                narrowStr.resize(result); //-V106
                result = ::WideCharToMultiByte(codePage, 0, pStr, len, narrowStr.data(), result, nullptr, nullptr);
            }

            if (result)
            {
                narrowStr.resize(result); //-V106
            }
            else
            {
                narrowStr.clear();
            }
        }

        return narrowStr;
    }

    string ToNarrow(wstring_view str, unsigned codePage)
    {
        return ToNarrow(str.data(), static_cast<int>(str.size()), codePage);
        //string narrowStr;
        //if (!str.empty())
        //{
        //    int result = 512;
        //    narrowStr.resize(result);

        //    result = ::WideCharToMultiByte(codePage, 0, str.data(), static_cast<int>(str.size()), narrowStr.data(), result, nullptr, nullptr);
        //    if (result == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        //    {
        //        result = ::WideCharToMultiByte(codePage, 0, str.data(), static_cast<int>(str.size()), nullptr, 0, nullptr, nullptr);
        //        narrowStr.resize(result); //-V106
        //        result = ::WideCharToMultiByte(codePage, 0, str.data(), static_cast<int>(str.size()), narrowStr.data(), result, nullptr, nullptr);
        //    }

        //    if (result)
        //    {
        //        narrowStr.resize(result); //-V106
        //    }
        //    else
        //    {
        //        narrowStr.clear();
        //    }
        //}

        //return narrowStr;
    }

    wstring ToWide(string_view str, unsigned codePage)
    {
        wstring wideStr;
        if (!str.empty())
        {
            int result = 512;
            wideStr.resize(result);

            result = MultiByteToWideChar(codePage, 0, str.data(), static_cast<int>(str.size()), wideStr.data(), result);
            if (result == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                result = MultiByteToWideChar(codePage, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
                wideStr.resize(result); //-V106
                result = MultiByteToWideChar(codePage, 0, str.data(), static_cast<int>(str.size()), wideStr.data(), result);
            }

            if (result)
            {
                wideStr.resize(result); //-V106
            }
            else
            {
                wideStr.clear();
            }
        }

        return wideStr;
    }
#endif

/* UTF8 conversions are deprecated in C++17

    static wstring_convert<codecvt_utf8<wchar_t>> wstrToUtf8converter;

    string ToStr(const wchar_t* pStr)
    {
        return pStr && *pStr ? wstrToUtf8converter.to_bytes(pStr) : string();
    }

    string ToStr(const wstring& narrowStr)
    {
        return wstrToUtf8converter.to_bytes(narrowStr.c_str());
    }

    wstring ToWStr(const char* pStr)
    {
        return pStr && *pStr ? wstrToUtf8converter.from_bytes(pStr) : wstring();
    }

    wstring ToWStr(const string& narrowStr)
    {
        return wstrToUtf8converter.from_bytes(narrowStr.c_str());
    }
*/
    uint64_t ToUint64(const char* pStr, ToUint64Err& err, const char** pEnd, uint64_t maxValue, bool allowCommas)
    {
        uint64_t val = 0;
        err = ToUint64_Success;

        if (pStr)
        {
            if (ASCII::IsDigit(*pStr))
            {
                const uint32_t lastDigit = maxValue % BASE_TEN;
                maxValue /= BASE_TEN;

                while (*pStr)
                {
                    if (val <= maxValue)
                    {
                        if (ASCII::IsDigit(*pStr))
                        {
                            const uint32_t chVal = static_cast<uint32_t>(ASCII::DigitToVal(*pStr));
                            if (val == maxValue && chVal > lastDigit)
                            {
                                err = ToUint64_Overflow;
                                break;
                            }

                            val *= BASE_TEN;
                            val += chVal;
                        }
                        else if (!(allowCommas && *pStr == ASCII::Comma))
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

    bool ToUint64(const char* pStr, uint64_t& val, const char** pEnd, bool allowCommas)
    {
        bool success = false;

        if (pStr)
        {
            while (ASCII::IsWhitespace(*pStr))
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

    bool ToInt64(const char* pStr, int64_t& val, const char** pEnd, bool allowCommas)
    {
        bool success = false;

        if (pStr)
        {
            while (ASCII::IsWhitespace(*pStr))
            {
                ++pStr;
            }

            ToUint64Err err = ToUint64_Success;

            if (*pStr == ASCII::Minus)
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

    bool ToUint32(const char* pStr, uint32_t& val, const char** pEnd, bool allowCommas)
    {
        bool success = false;

        if (pStr)
        {
            while (ASCII::IsWhitespace(*pStr))
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

    bool ToInt32(const char* pStr, int32_t& val, const char** pEnd, bool allowCommas)
    {
        bool success = false;

        if (pStr)
        {
            while (ASCII::IsWhitespace(*pStr))
            {
                ++pStr;
            }

            ToUint64Err err = ToUint64_Success;

            if (*pStr == ASCII::Minus)
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

    uint64_t ToUint64_FromView(string_view& str, ToUint64Err& err, bool skipLeadingWhitespace, uint64_t maxValue, bool allowCommas)
    {
        uint64_t val = 0;
        err = ToUint64_Success;

        if (skipLeadingWhitespace)
        {
            while (!str.empty() && ASCII::IsWhitespace(str.front()))
            {
                str.remove_prefix(1);
            }
        }

        if (!str.empty())
        {
            if (ASCII::IsDigit(str.front()))
            {
                const uint32_t lastDigit = maxValue % BASE_TEN;
                maxValue /= BASE_TEN;

                for (; !str.empty(); str.remove_prefix(1))
                {
                    if (val <= maxValue)
                    {
                        const char ch = str.front();
                        if (ASCII::IsDigit(ch))
                        {
                            const uint32_t chVal = static_cast<uint32_t>(ASCII::DigitToVal(ch));
                            if (val == maxValue && chVal > lastDigit)
                            {
                                err = ToUint64_Overflow;
                                break;
                            }

                            val *= BASE_TEN;
                            val += chVal;
                        }
                        else if (!(allowCommas && ch == ASCII::Comma))
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

    bool ToUint64_FromView(string_view str, uint64_t& val, bool skipLeadingWhitespace, uint64_t maxValue, bool allowCommas)
    {
        ToUint64Err err;
        val = ToUint64_FromView(str, err, skipLeadingWhitespace, maxValue, allowCommas);
        return err >= ToUint64_Success;
    }

    int Quick4(const char*& pStr)
    {
        int val = -1;

        // This must be done first so a failure will happen if string is terminated
        if (ASCII::IsDigit(pStr[0]) && ASCII::IsDigit(pStr[1]) &&
            ASCII::IsDigit(pStr[2]) && ASCII::IsDigit(pStr[3]))
        {
            val = ASCII::DigitToVal(pStr[0]) * 1000 + ASCII::DigitToVal(pStr[1]) * 100 +
                  ASCII::DigitToVal(pStr[2]) * 10 + ASCII::DigitToVal(pStr[3]);
            pStr += 4; //-V112
        }

        return val;
    }

    int Quick3(const char*& pStr)
    {
        int val = -1;

        // This must be done first so a failure will happen if string is terminated
        if (ASCII::IsDigit(pStr[0]) && ASCII::IsDigit(pStr[1]) &&
            ASCII::IsDigit(pStr[2]))
        {
            val = ASCII::DigitToVal(pStr[0]) * 100 + ASCII::DigitToVal(pStr[1]) * 10 +
                ASCII::DigitToVal(pStr[2]);
            pStr += 3; //-V112
        }

        return val;
    }

    int Quick2(const char*& pStr)
    {
        int val = -1;

        // This must be done first so a failure will happen if string is terminated
        if (ASCII::IsDigit(pStr[0]) && ASCII::IsDigit(pStr[1]))
        {
            val = ASCII::DigitToVal(pStr[0]) * 10 + ASCII::DigitToVal(pStr[1]);
            pStr += 2;
        }

        return val;
    }

    ///////////////////////////////////////////////////////////////////////

    char hexDigits[2][16] =
    {
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' },
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' }
    };

    char NibbleToHex(uint8_t byteVal, bool upperCase)
    {
        return hexDigits[upperCase ? 1 : 0][byteVal & LowNibble];
    }

    char HighNibbleToHex(uint8_t byteVal, bool upperCase)
    {
        return hexDigits[upperCase ? 1 : 0][byteVal >> NibbleLenInBits];
    }

    string& AppendHex(string& str, uint64_t val, bool useUpperCase, unsigned padLen)
    {
        if (padLen > MaxLen_Uint64HexStr)
        {
            padLen = MaxLen_Uint64HexStr;
        }

        if (!val)
        {
            if (padLen)
            {
                str.append(padLen, ASCII::Zero); //-V106
            }
            else
            {
                str += ASCII::Zero;
            }
        }
        else
        {
            const char* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
            char hexStr[MaxLen_Uint64HexStr];
            unsigned offset = static_cast<unsigned>(sizeof(hexStr));

            do
            {
                hexStr[--offset] = pHexDigits[val & LowestNibble];
                val >>= NibbleLenInBits;

            } while (val);

            const unsigned padOffset = static_cast<unsigned>(sizeof(hexStr) - padLen); //-V104
            while (offset > padOffset)
            {
                hexStr[--offset] = ASCII::Zero;
            }

            str.append(&hexStr[offset], sizeof(hexStr) - offset); //-V104
        }

        return str;
    }

    string ToHex(uint64_t val, bool useUpperCase, unsigned padLen)
    {
        string str;
        str.reserve(MaxLen_Uint64HexStr);
        AppendHex(str, val, useUpperCase, padLen);
        return str;
    }

    string& BinToHex(const void* _pData, size_t dataLen, string& str, bool useUpperCase, char separator)
    {
        if (_pData && dataLen)
        {
            str.reserve(dataLen * (CharactersPerByte + (separator ? 1 : 0)));
            const char* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
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

    string BinToHex(const void* _pData, size_t dataLen, bool useUpperCase, char separator)
    {
        string str;
        BinToHex(_pData, dataLen, str, useUpperCase, separator);
        return str;
    }

    size_t BinToHex(const void* _pData, size_t dataLen, char* pBuffer, size_t bufferLen, bool useUpperCase, char separator)
    {
        size_t dstIndex = 0;

        if (pBuffer && bufferLen)
        {
            if (_pData && dataLen)
            {
                bufferLen--;

                const char* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
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

    string& AppendHex(string& str, const void* pData_, size_t dataLen, bool useUpperCase, char separator)
    {
        if (pData_ && dataLen)
        {
            const char* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
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

    ostream& AppendHex(ostream& stream, const void* pData_, size_t dataLen, bool useUpperCase, char separator)
    {
        if (pData_ && dataLen)
        {
            const char* pHexDigits = hexDigits[useUpperCase ? 1 : 0];
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

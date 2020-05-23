// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "PlainStringBuilder.h"

using namespace std;

PlainStringBuilder::PlainStringBuilder(char* pBuffer, size_t maxLenInChars)
    : _pBuffer(pBuffer)
    , _maxLen(pBuffer && maxLenInChars ? maxLenInChars - 1 : 0)
{
    if (_maxLen)
    {
        _pBuffer[0] = 0;
    }
}

string PlainStringBuilder::str() const
{
    return _pBuffer && _offset ? string(_pBuffer, _offset) : string();
}

char* PlainStringBuilder::setLen(size_t len)
{
    if (_pBuffer && len < _offset)
    {
        _offset = len;
        _pBuffer[_offset] = 0;
    }
    return _pBuffer;
}

char* PlainStringBuilder::append(char ch, size_t count)
{
    if (_pBuffer)
    {
        while (_offset < _maxLen && count--)
        {
            _pBuffer[_offset++] = ch;
        }
        _pBuffer[_offset] = 0;
    }
    return _pBuffer;
}

char* PlainStringBuilder::append(const char* pStr, size_t strLen)
{
    if (_pBuffer && pStr && strLen)
    {
        while (_offset < _maxLen && strLen--)
        {
            _pBuffer[_offset++] = *pStr++;
        }
        _pBuffer[_offset] = 0;
    }
    return _pBuffer;
}

char* PlainStringBuilder::operator+=(const char* pStr)
{
    if (_pBuffer && pStr)
    {
        while (_offset < _maxLen && *pStr)
        {
            _pBuffer[_offset++] = *pStr++;
        }
        _pBuffer[_offset] = 0;
    }
    return _pBuffer;
}

char* PlainStringBuilder::operator+=(const string& str)
{
    return append(str.c_str(), str.size());
}

char* PlainStringBuilder::operator+=(char ch)
{
    if (_pBuffer && _offset < _maxLen)
    {
        _pBuffer[_offset++] = ch;
        _pBuffer[_offset] = 0;
    }
    return _pBuffer;
}

char* PlainStringBuilder::operator+=(int64_t val)
{
    if (_pBuffer && _offset < _maxLen)
    {
        char tmpStr[Convert::MinBufferLen];
        operator+=(Convert::ToStr64(val, tmpStr));
    }
    return _pBuffer;
}

char* PlainStringBuilder::operator+=(int32_t val)
{
    if (_pBuffer && _offset < _maxLen)
    {
        char tmpStr[Convert::MinBufferLen];
        operator+=(Convert::ToStr(val, tmpStr));
    }
    return _pBuffer;
}

char* PlainStringBuilder::operator+=(uint64_t val)
{
    if (_pBuffer && _offset < _maxLen)
    {
        char tmpStr[Convert::MinBufferLen];
        operator+=(Convert::ToStr64(val, tmpStr));
    }
    return _pBuffer;
}

char* PlainStringBuilder::operator+=(uint32_t val)
{
    if (_pBuffer && _offset < _maxLen)
    {
        char tmpStr[Convert::MinBufferLen];
        operator+=(Convert::ToStr(val, tmpStr));
    }
    return _pBuffer;
}

char* PlainStringBuilder::append(int64_t val, char groupSeparatorChar)
{
    if (_pBuffer && _offset < _maxLen)
    {
        char tmpStr[Convert::MinBufferLen];
        operator+=(Convert::ToStr64(val, tmpStr, groupSeparatorChar));
    }
    return _pBuffer;
}

char* PlainStringBuilder::append(int32_t val, char groupSeparatorChar)
{
    if (_pBuffer && _offset < _maxLen)
    {
        char tmpStr[Convert::MinBufferLen];
        operator+=(Convert::ToStr(val, tmpStr, groupSeparatorChar));
    }
    return _pBuffer;
}

char* PlainStringBuilder::append(uint64_t val, char groupSeparatorChar)
{
    if (_pBuffer && _offset < _maxLen)
    {
        char tmpStr[Convert::MinBufferLen];
        operator+=(Convert::ToStr64(val, tmpStr, groupSeparatorChar));
    }
    return _pBuffer;
}

char* PlainStringBuilder::append(uint32_t val, char groupSeparatorChar)
{
    if (_pBuffer && _offset < _maxLen)
    {
        char tmpStr[Convert::MinBufferLen];
        operator+=(Convert::ToStr(val, tmpStr, groupSeparatorChar));
    }
    return _pBuffer;
}

char* PlainStringBuilder::append(uint64_t val, size_t minWidth, char fillChar)
{
    if (_pBuffer)
    {
        intAppend(val, minWidth, fillChar);
    }
    return _pBuffer;
}

char* PlainStringBuilder::append(uint32_t val, size_t minWidth, char fillChar)
{
    if (_pBuffer)
    {
        intAppend(val, minWidth, fillChar);
    }
    return _pBuffer;
}

char* PlainStringBuilder::appendHex(uint64_t val, bool useUpperCase, unsigned padLen)
{
    if (_pBuffer)
    {
        if (padLen > Convert::MaxLen_Uint64HexStr)
        {
            padLen = Convert::MaxLen_Uint64HexStr;
        }

        if (!val)
        {
            append(ASCII::Zero, padLen ? padLen : 1);
        }
        else
        {
            const char* pHexDigits = Convert::hexDigits[useUpperCase ? 1 : 0];
            char hexStr[Convert::MaxLen_Uint64HexStr];
            unsigned offset = static_cast<unsigned>(sizeof(hexStr));

            do
            {
                hexStr[--offset] = pHexDigits[val & Convert::LowestNibble];
                val >>= Convert::NibbleLenInBits;

            } while (val);

            const unsigned padOffset = static_cast<unsigned>(sizeof(hexStr)) - padLen;
            while (offset > padOffset)
            {
                hexStr[--offset] = ASCII::Zero;
            }

            append(&hexStr[offset], sizeof(hexStr) - offset);
        }
    }

    return _pBuffer;
}

char* PlainStringBuilder::appendHex(const void* pData, size_t dataLen, bool useUpperCase, char separator)
{
    if (pData && dataLen && _pBuffer && _offset < _maxLen)
    {
        _offset += Convert::BinToHex(pData, dataLen, &_pBuffer[_offset], _maxLen - _offset, useUpperCase, separator);
    }
    return _pBuffer;
}

char* PlainStringBuilder::operator+=(const DateTime& dateTime)
{
    return append(dateTime);
}

char* PlainStringBuilder::append(const DateTime& dateTime, bool useFourDigitYear)
{
    if (_pBuffer && _offset < _maxLen)
    {
        if (useFourDigitYear)
        {
            intAppend(static_cast<uint32_t>(dateTime.year()), 4, ASCII::Zero); //-V112
        }
        else
        {
            intAppend(static_cast<uint32_t>(dateTime.year() % 100), 2, ASCII::Zero);
        }
        intAppend(ASCII::Slash);
        intAppend(static_cast<uint32_t>(dateTime.month() + 1), 2, ASCII::Zero);
        intAppend(ASCII::Slash);
        intAppend(static_cast<uint32_t>(dateTime.day()), 2, ASCII::Zero);

        intAppend(ASCII::Space);

        intAppend(static_cast<uint32_t>(dateTime.hour()), 2, ASCII::Zero);
        intAppend(ASCII::Colon);
        intAppend(static_cast<uint32_t>(dateTime.minute()), 2, ASCII::Zero);
        intAppend(ASCII::Colon);
        intAppend(static_cast<uint32_t>(dateTime.second()), 2, ASCII::Zero);
        intAppend(ASCII::Period);
        intAppend(static_cast<uint32_t>(dateTime.milliseconds()), 3, ASCII::Zero);
    }
    return _pBuffer;
}

void PlainStringBuilder::intAppend(char ch)
{
    if (_offset < _maxLen)
    {
        _pBuffer[_offset++] = ch;
    }
}

void PlainStringBuilder::intAppend(uint64_t val, size_t minWidth, char fillChar)
{
    if (_offset < _maxLen)
    {
        char tmpStr[Convert::MaxStrLen64];

        char* pEnd;
        const char* pStr = Convert::ToStr64(val, tmpStr, 0, false, &pEnd);

        const size_t width = pEnd - pStr;
        if (width < minWidth)
        {
            append(fillChar, minWidth - width);
        }

        operator+=(pStr);
    }
}

void PlainStringBuilder::intAppend(uint32_t val, size_t minWidth, char fillChar)
{
    if (_offset < _maxLen)
    {
        char tmpStr[Convert::MaxStrLen32];

        char* pEnd;
        const char* pStr = Convert::ToStr(val, tmpStr, 0, false, &pEnd);

        const size_t width = pEnd - pStr;
        if (width < minWidth)
        {
            append(fillChar, minWidth - width);
        }

        operator+=(pStr);
    }
}

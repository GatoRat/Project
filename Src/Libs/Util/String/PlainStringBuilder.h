// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "../Time/DateTime.h"
#include "ASCII.h"
#include "Convert.h"

/** A plain string builder.
    For speed reasons, the maximum length is stored modified to allow for a
    terminating zero. As a result to value returned by GetMaxLen() will be zero
    or one less than the value passed in the constructor.
 */
struct PlainStringBuilder
{
public:
    /** PlainStringBuilder constructor.
       @param pBuffer The buffer.
       @param maxLenInChars The maximum length in characters, including the
            terminating zero. For speed reasons, maxLenInChars is stored
            modified to allow for a terminating zero. As a result to value
            returned by GetMaxLen() will be zero or one less than the value
            passed in the constructor.
     */
    PlainStringBuilder(char* pBuffer, size_t maxLenInChars);

    /** PlainStringBuilder constructor.
       @param buffer The buffer.
     */
    template <typename BufferT, size_t maxLenInChars>
    explicit PlainStringBuilder(BufferT (&buffer)[maxLenInChars])
        : _pBuffer((char*) buffer)
        , _maxLen(maxLenInChars ? maxLenInChars - 1 : 0)
    {
        static_assert(sizeof(BufferT) == sizeof(char), "buffer type size must be same as char.");

        if (_maxLen)
        {
            _pBuffer[0] = 0;
        }
    }

    PlainStringBuilder(const PlainStringBuilder&) = delete;
    void operator=(const PlainStringBuilder&) = delete;
    ~PlainStringBuilder() = default;

    /** Returns a pointer to the buffer.
     */
    operator const char*() const
    {
        return _pBuffer;
    }

    /** Returns a string of the contents.
     */
    std::string str() const;

    /** Returns the maximum length.
       @returns The maximum length NOT including the terminating zero.
     */
    size_t getMaxLen() const
    {
        return _maxLen;
    }

    /** Returns the current length of the string.
       @returns The current length of the string.
     */
    size_t getLen() const
    {
        return _offset;
    }

    /** Sets the current length of the string.
       @param The new length. If this is longer than the current length,
            nothing happens.
       @returns A pointer to the buffer.
     */
    char* setLen(size_t len);

    /** Appends a specified number of copies of the character.
       @param ch The character to append.
       @param count The number of times to append the character.
       @returns A pointer to the buffer.
     */
    char* append(char ch, size_t count = 1);

    /** Appends an array of data.
       @param data The data to append.
       @returns A pointer to the buffer.
     */
    template <typename T, size_t len>
    char* append(const T (&data)[len])
    {
        static_assert(sizeof(T) == sizeof(char), "data type size must be same as char.");

        return append(reinterpret_cast<const char*>(data), len);
    }

    /** Appends a string.
       @param pStr The data to append.
       @param strLen The length of the data to append.
       @returns A pointer to the buffer.
       @remarks This will copy any zero characters that are within the strLen.
     */
    char* append(const char* pStr, size_t strLen);

    /** Appends a string.
       @param pStr The string to append.
       @returns A pointer to the buffer.
     */
    char* operator+=(const char* pStr);

    /** Appends a string.
       @param str The string to append.
       @returns A pointer to the buffer.
     */
    char* operator+=(const std::string& str);

    /** Appends a character.
       @param ch The character to append.
       @returns A pointer to the buffer.
     */
    char* operator+=(char ch);

    /** Appends a signed 64-bit value as an ASCII string.
       @param val The value to append.
       @returns A pointer to the buffer.
     */
    char* operator+=(int64_t val);

    /** Appends a signed 32-bit value as an ASCII string.
       @param val The value to append.
       @returns A pointer to the buffer.
     */
    char* operator+=(int32_t val);

    /** Appends an unsigned 64-bit value as an ASCII string.
       @param val The value to append.
       @returns A pointer to the buffer.
     */
    char* operator+=(uint64_t val);

    /** Appends an unsigned 32-bit value as an ASCII string.
       @param val The value to append.
       @returns A pointer to the buffer.
     */
    char* operator+=(uint32_t val);

    /** Appends a signed 64-bit value as an ASCII string using group separators.
       @param val The value to append.
       @param groupSeparatorChar The group separator character.
       @returns A pointer to the buffer.
     */
    char* append(int64_t val, char groupSeparatorChar = ASCII::Comma);

    /** Appends a signed 32-bit value as an ASCII string using group separators.
       @param val The value to append.
       @param groupSeparatorChar The group separator character.
       @returns A pointer to the buffer.
     */
    char* append(int32_t val, char groupSeparatorChar = ASCII::Comma);

    /** Appends an unsigned 64-bit value as an ASCII string using group separators.
       @param val The value to append.
       @param groupSeparatorChar The group separator character.
       @returns A pointer to the buffer.
     */
    char* append(uint64_t val, char groupSeparatorChar = ASCII::Comma);

    /** Appends an unsigned 32-bit value as an ASCII string using group separators.
       @param val The value to append.
       @param groupSeparatorChar The group separator character.
       @returns A pointer to the buffer.
     */
    char* append(uint32_t val, char groupSeparatorChar = ASCII::Comma);

    /** Appends a padded unsigned 64-bit value as an ASCII string.
       @param val The value to append.
       @param minWidth The minimum width of the number.
       @param fillChar The character to use to pad number to width.
       @returns A pointer to the buffer.
     */
    char* append(uint64_t val, size_t minWidth, char fillChar);

    /** Appends a padded unsigned 32-bit value as an ASCII string.
       @param val The value to append.
       @param minWidth The minimum width of the number.
       @param fillChar The character to use to pad number to width.
       @returns A pointer to the buffer.
     */
    char* append(uint32_t val, size_t minWidth, char fillChar);

    /** Appends a value as a hexadecimal string.
       @param val The value.
       @param useUpperCase If \b true use uppercase characters for A-F,
            else use lower case.
       @param padLen If greater than the length of the resulting string,
            ASCII '0's will be pre-pended.
       @returns A pointer to the buffer.
     */
    char* appendHex(uint64_t val, bool useUpperCase = false, unsigned padLen = 0);

    /** Appends data as a hexadecimal string.
       @param pData A pointer to the data.
       @param dataLen The data length.
       @param useUpperCase If \b true use uppercase characters for A-F, else use lower case.
       @param separator If not zero, the character to place between hexadecimal pairs.
       @returns A pointer to the buffer.
     */
    char* appendHex(const void* pData, size_t dataLen, bool useUpperCase = false, char separator = 0);

    /** Appends a DateTime value.
       @param dateTime The dateTime to append.
       @returns A pointer to the buffer.
       @remarks Formats date time as [YY]YY/MM/DD HH:MM:SS.mmm
     */
    char* operator+=(const DateTime& dateTime);

    /** Appends a DateTime value.
       @param dateTime The dateTime to append.
       @param useFourDigitYear If \b true, use four digit year.
       @returns A pointer to the buffer.
       @remarks Formats date time as [YY]YY/MM/DD HH:MM:SS.mmm
     */
    char* append(const DateTime& dateTime, bool useFourDigitYear = false);

private:
    char*  _pBuffer;
    size_t _maxLen;
    size_t _offset = 0;

    void intAppend(char ch);
    void intAppend(uint64_t val, size_t minWidth, char fillChar);
    void intAppend(uint32_t val, size_t minWidth, char fillChar);
};

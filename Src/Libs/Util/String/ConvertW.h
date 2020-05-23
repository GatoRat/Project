// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <ostream>
#include <string>
#include <string_view>

/// <summary>
/// Conversion functions.
/// </summary>
namespace ConvertW
{
    /// <summary>
    /// Safely constructs a std::wstring.
    /// </summary>
    /// <param name="pStr">A pointer to the wchar_t string.</param>
    /// <returns>A std::wstring.</returns>
    /// <remarks>
    /// This simplifies constructing a string with a source which may be a nullptr.
    /// This is especially useful in constructors.
    /// </remarks>
    inline std::wstring ToStr(const wchar_t* pStr)
    {
        return std::wstring(pStr && *pStr ? pStr : L"");
    }

    /// <summary>
    /// Safely constructs a std::wstring.
    /// </summary>
    /// <param name="pStr">  A pointer to the wchar_t string.</param>
    /// <param name="strLen">The string length.</param>
    /// <returns>A std::wstring.</returns>
    /// <remarks>
    /// This simplifies constructing a string with a source which may be a nullptr.
    /// This is especially useful in constructors.
    /// </remarks>
    std::wstring ToStr(const wchar_t* pStr, size_t strLen);

    /// <summary>
    /// Safely constructs a std::wstring_view.
    /// </summary>
    /// <param name="pStr">A pointer to the char string.</param>
    /// <returns>A std::wstring_view.</returns>
    /// <remarks>
    /// This simplifies constructing a string with a source which may be a nullptr.
    /// This is especially useful in constructors.
    /// </remarks>
    std::wstring_view ToStrView(const wchar_t* pStr);

    /// <summary>
    /// Safely constructs a std::wstring_view.
    /// </summary>
    /// <param name="pStr">A pointer to the char string.</param>
    /// <param name="strLen">The string length.</param>
    /// <returns>A std::wstring_view.</returns>
    /// <remarks>
    /// This simplifies constructing a string with a source which may be a nullptr.
    /// This is especially useful in constructors.
    /// </remarks>
    std::wstring_view ToStrView(const wchar_t* pStr, size_t strLen);

    /// <summary>
    /// Safely constructs a std::wstring_view from a buffer which may or may not be null terminated.
    /// </summary>
    /// <param name="pStr">A pointer to the char string.</param>
    /// <param name="strLen">The maximum string length.</param>
    /// <returns>A std::wstring_view.</returns>
    /// <remarks>
    /// This simplifies constructing a string with a source which may be a nullptr.
    /// This is especially useful in constructors.
    /// </remarks>
    std::wstring_view ToStrViewMaxLen(const wchar_t* pStr, size_t maxLen);

    enum ToUint64Err
    {
        ToUint64_Null        = -4,
        ToUint64_Empty       = -3,
        ToUint64_Overflow    = -2,
        ToUint64_NotNumber   = -1,
        ToUint64_Success     =  0,
        ToUint64_EndWithChar =  1
    };

    /// <summary>
    /// Converts a string to a uint64_t.
    /// </summary>
    /// <param name="pStr">The string to convert.</param>
    /// <param name="result">Result indicator.</param>
    /// <param name="pEnd">Optional pointer to character which ends scan.</param>
    /// <returns>The converted value.</returns>
    /// <remarks>
    /// This function is very strict. It does NOT skip leading whitespace
    /// and "fails" on any non-digit.
    /// </remarks>
    uint64_t ToUint64(const wchar_t* pStr, ToUint64Err& err, const wchar_t** pEnd = nullptr, uint64_t maxValue = UINT64_MAX, bool allowCommas = false);

    /// <summary>
    /// Converts a string to a uint64_t.
    /// </summary>
    /// <param name="pStr">The string to convert</param>
    /// <param name="val"> The value calculated until the error.</param>
    /// <param name="pEnd">Optional pointer to character which ends scan.</param>
    /// <returns><b>true</b> if conversion was successful, else <b>false</b>.</returns>
    /// <remarks>
    /// This will skip all leading whitespace until a digit or minus or plus sign is
    /// encountered. It will then convert digits until a non-digit character is encountered.
    /// </remarks>
    bool ToUint64(const wchar_t* pStr, uint64_t& val, const wchar_t** pEnd = nullptr, bool allowCommas = false);

    /// <summary>
    /// Converts a string to an int64_t.
    /// </summary>
    /// <param name="pStr">The string to convert.</param>
    /// <param name="val"> The value calculated until the error.</param>
    /// <param name="pEnd">Optional pointer to character which ends scan.</param>
    /// <returns><b>true</b> if conversion was successful, else <b>false</b>.</returns>
    /// <remarks>
    /// This will skip all leading whitespace until a digit or minus or plus sign is
    /// encountered. It will then convert digits until a non-digit character is encountered.
    /// </remarks>
    bool ToInt64(const wchar_t* pStr, int64_t& val, const wchar_t** pEnd = nullptr, bool allowCommas = false);

    /// <summary>
    /// Converts a string to a uint32_t.
    /// </summary>
    /// <param name="pStr">The string to convert.</param>
    /// <param name="val"> The value calculated until the error.</param>
    /// <param name="pEnd">Optional pointer to character which ends scan.</param>
    /// <returns><b>true</b> if conversion was successful, else <b>false</b>.</returns>
    /// <remarks>
    /// This will skip all leading whitespace until a digit or minus or plus sign is
    /// encountered. It will then convert digits until a non-digit character is encountered.
    /// </remarks>
    bool ToUint32(const wchar_t* pStr, uint32_t& val, const wchar_t** pEnd = nullptr, bool allowCommas = false);

    /// <summary>
    /// Converts a string to an int32_t.
    /// </summary>
    /// <param name="pStr">The string to convert.</param>
    /// <param name="val"> The value calculated until the error.</param>
    /// <param name="pEnd">Optional pointer to character which ends scan.</param>
    /// <returns><b>true</b> if conversion was successful, else <b>false</b>.</returns>
    /// <remarks>
    /// This will skip all leading whitespace until a digit or minus or plus sign is
    /// encountered. It will then convert digits until a non-digit character is encountered.
    /// </remarks>
    bool ToInt32(const wchar_t* pStr, int32_t& val, const wchar_t** pEnd = nullptr, bool allowCommas = false);

    /// <summary>
    /// Converts a string to a uint64_t.
    /// </summary>
    /// <param name="str">The string to convert. After completion, this will "reference" the
    ///     the terminating character. If str encounters an ASCII zero, parsing will terminate
    ///     and err will be set to Convert::ToUint64_EndWithChar. In this case, str.front()
    ///     will return zero.</param>
    /// <param name="err">Error value.</param>
    /// <param name="skipLeadingWhitespace">If <b>true</b>, skip leading whitespace.</param>
    /// <param name="maxValue">The maximum value.</param>
    /// <returns>The converted value.</returns>
    uint64_t ToUint64_FromView(std::wstring_view& str, ToUint64Err& err, bool skipLeadingWhitespace = false, uint64_t maxValue = UINT64_MAX, bool allowCommas = false);

    bool ToUint64_FromView(std::wstring_view str, uint64_t& val, bool skipLeadingWhitespace = false, uint64_t maxValue = UINT64_MAX, bool allowCommas = false);

    /// <summary>
    /// Converts a string of four digits to an int.
    /// </summary>
    /// <param name="pStr">The string to convert; it must be at least 4 digits long.
    ///     On success, this will point to the fifth character of pStr. On failure,
    //      this will not be changed.</param>
    /// <returns>If successful, a number greater or equal to zero, else a negative number.</returns>
    /// <remarks>
    /// This was written for basic date conversions. It was included here for consistency
    /// and testing.
    /// </remarks>
    int Quick4(const wchar_t*& pStr);

    /// <summary>
    /// Converts a string of three digits to an int.
    /// </summary>
    /// <param name="pStr">The string to convert; it must be at least 3 digits long.
    ///     On success, this will point to the fourth character of pStr. On failure,
    //      this will not be changed.</param>
    /// <returns>If successful, a number greater or equal to zero, else a negative number.</returns>
    /// <remarks>
    /// This was written for basic date conversions. It was included here for consistency
    /// and testing.
    /// </remarks>
    int Quick3(const wchar_t*& pStr);

    /// <summary>
    /// Converts a string of two digits to an int.
    /// </summary>
    /// <param name="pStr">The string to convert; it must be at least 2 digits long.
    ///     On success, this will point to the third character of pStr. On failure,
    //      this will not be changed.</param>
    /// <returns>If successful, a number greater or equal to zero, else a negative number.</returns>
    /// <remarks>
    /// This was written for basic date conversions. It was included here for consistency
    /// and testing.
    /// </remarks>
    int Quick2(const wchar_t*& pStr);

    ///////////////////////////////////////////////////////////////////////

    extern wchar_t hexDigits[2][16];

    constexpr uint64_t LowestNibble      = 0x000000000000000f;
    constexpr uint8_t  LowNibble         = 0x0f;
    constexpr int      NibbleLenInBits   = 4; //-V112
    constexpr int      CharactersPerByte = 2;

    /** Converts the low nibble of a byte to an ASCII hex character.
        \param byteVal Byte containing the value to convert.
        \param upperCase Use upper case ('A' - 'Z') characters.
        \returns A character representing ASCII hex character of the low nibble
            of the passed byte.
        \see HighNibbleToHex
        \see ConvertNibble
     */
    wchar_t NibbleToHex(uint8_t byteVal, bool upperCase = false);

    /** Converts the high nibble of a byte to an ASCII hex character.
        \param byteVal Byte containing the value to convert.
        \param upperCase Use upper case ('A' - 'Z') characters.
        \returns A character representing ASCII hex character of the high nibble
            of the passed byte.
        \see NibbleToHex
        \see ConvertNibble
     */
    wchar_t HighNibbleToHex(uint8_t byteVal, bool upperCase = false);

    /// <summary>
    /// The maximum length of a uint32_t as a hexadecimal string.
    /// </summary>
    const unsigned MaxLen_Uint32HexStr = 8;

    /// <summary>
    /// The maximum length of a uint64_t as a hexadecimal string.
    /// </summary>
    const unsigned MaxLen_Uint64HexStr = 16;

    /// <summary>
    /// Converts a value to hexadecimal characters, appending the result to a string.
    /// </summary>
    /// <param name="val">         The value to convert.</param>
    /// <param name="str">         The string to append.</param>
    /// <param name="useUpperCase">If <b>true</b> use uppercase characters.</param>
    /// <param name="padLen">      If greater than the length of the resulting string,
    ///     ASCII '0's will be pre-pended.</param>
    /// <returns>A reference to str.</returns>
    /// <remarks>
    /// This exists to minimize the use of the CRT and/or the OS.
    /// </remarks>
    std::wstring& AppendHex(std::wstring& str, uint64_t val, bool useUpperCase = false, unsigned padLen = 0);

    /// <summary>
    /// Converts a value to hexadecimal characters.
    /// </summary>
    /// <param name="val">         The value to convert.</param>
    /// <param name="useUpperCase">If <b>true</b> use uppercase characters.</param>
    /// <param name="padLen">      If greater than the length of the resulting string,
    ///     ASCII '0's will be pre-pended.</param>
    /// <returns>A hexadecimal string.</returns>
    std::wstring ToHex(uint64_t val, bool useUpperCase = false, unsigned padLen = 0);

    /// <summary>
    /// Converts data to a hexadecimal string.
    /// </summary>
    /// <param name="_pData">      The data which to convert.</param>
    /// <param name="dataLen">     The length of the data to convert.</param>
    /// <param name="str">         The string to receive the converted data.</param>
    /// <param name="useUpperCase">If <b>true</b> use uppercase characters.</param>
    /// <param name="separator">   If non-zero, the character to inserted between
    ///  each byte of data.</param>
    /// <returns>A hexadecimal string representing the data.</returns>
    std::wstring& BinToHex(const void* _pData, size_t dataLen, std::wstring& str, bool useUpperCase = false, wchar_t separator = 0);

    /// <summary>
    /// Converts data to a hexadecimal string.
    /// </summary>
    /// <param name="pData">       The data which to convert.</param>
    /// <param name="dataLen">     The length of the data to convert.</param>
    /// <param name="useUpperCase">If <b>true</b> use uppercase characters.</param>
    /// <param name="separator">   If non-zero, the character to inserted between
    ///     each byte of data.</param>
    /// <returns>A hexadecimal string representing the data.</returns>
    std::wstring BinToHex(const void* pData, size_t dataLen, bool useUpperCase = false, wchar_t separator = 0);

    /// <summary>
    /// Converts data to a hexadecimal string.
    /// </summary>
    /// <param name="pData">       The data which to convert.</param>
    /// <param name="dataLen">     The length of the data to convert.</param>
    /// <param name="pBuffer">     The buffer to receive the resulting string.</param>
    /// <param name="bufferLen">   The length of pBuffer.</param>
    /// <param name="useUpperCase">If <b>true</b> use uppercase characters.</param>
    /// <param name="separator">   If non-zero, the character to insert between
    ///     each byte of data.</param>
    /// <returns>The length of the hexadecimal string.</returns>
    size_t BinToHex(const void* pData, size_t dataLen, wchar_t* pBuffer, size_t bufferLen, bool useUpperCase = false, wchar_t separator = 0);

    /// <summary>
    /// Converts data to a hexadecimal string.
    /// </summary>
    /// <param name="str">         The string to append.</param>
    /// <param name="pData">       The data which to convert.</param>
    /// <param name="dataLen">     The length of the data to convert.</param>
    /// <param name="useUpperCase">If <b>true</b> use uppercase characters.</param>
    /// <param name="separator">   If non-zero, the character to insert between
    ///     each byte of data.</param>
    /// <returns>The length of the hexadecimal string.</returns>
    std::wstring& AppendHex(std::wstring& str, const void* pData, size_t dataLen, bool useUpperCase = false, wchar_t separator = 0);

    /// <summary>
    /// Converts data to a hexadecimal string.
    /// </summary>
    /// <param name="stream">      The stream to append.</param>
    /// <param name="pData">       The data which to convert.</param>
    /// <param name="dataLen">     The length of the data to convert.</param>
    /// <param name="useUpperCase">If <b>true</b> use uppercase characters.</param>
    /// <param name="separator">   If non-zero, the character to insert between
    ///     each byte of data.</param>
    /// <returns>The length of the hexadecimal string.</returns>
    std::wostream& AppendHex(std::wostream& stream, const void* pData, size_t dataLen, bool useUpperCase = false, wchar_t separator = 0);

    /// <summary>
    /// The maximum length of formatted 32-bit unsigned number without groupings.
    /// </summary>
    constexpr size_t MaxStrLen32               = 11; //                 4294967295
    /// <summary>
    /// The maximum length of formatted 32-bit signed number without groupings.
    /// </summary>
    constexpr size_t MaxStrLen32_Signed        = 12; //                -2147483648
    /// <summary>
    /// The maximum length of formatted 32-bit unsigned number with groupings.
    /// </summary>
    constexpr size_t MaxStrLen32_Commas        = 14; //              4,294,967,295
    /// <summary>
    /// The maximum length of formatted 32-bit signed number with groupings.
    /// </summary>
    constexpr size_t MaxStrLen32_Commas_Signed = 15; //             -2,147,483,648

    /// <summary>
    /// The maximum length of formatted 64-bit unsigned number without groupings.
    /// </summary>
    constexpr size_t MaxStrLen64               = 21; //       18446744073709551615
    /// <summary>
    /// The maximum length of formatted 64-bit signed number without groupings.
    /// </summary>
    constexpr size_t MaxStrLen64_Signed        = 21; //       -9223372036854775807
    /// <summary>
    /// The maximum length of formatted 64-bit unsigned number with groupings.
    /// </summary>
    constexpr size_t MaxStrLen64_Commas        = 27; // 18,446,744,073,709,551,615
    /// <summary>
    /// The maximum length of formatted 64-bit signed number with groupings.
    /// </summary>
    constexpr size_t MaxStrLen64_Commas_Signed = 27; // -9,223,372,036,854,775,807

    /// <summary>
    /// The minimum buffer length required to hold a fully formatted 32-bit number.
    /// </summary>
    constexpr size_t MinBufferLen32            = MaxStrLen32_Commas_Signed;
    /// <summary>
    /// The minimum buffer length required to hold a fully formatted 64-bit number.
    /// </summary>
    constexpr size_t MinBufferLen64            = MaxStrLen64_Commas_Signed;
    /// <summary>
    /// The minimum buffer length required to hold a fully formatted number.
    /// </summary>
    constexpr size_t MinBufferLen              = MinBufferLen64;

    /// <summary>
    /// Convert a 32-bit unsigned value to a string.
    /// </summary>
    /// <param name="val">               The value to convert.</param>
    /// <param name="pBuffer">           The buffer to receive the formatted string.
    ///  This <b>must</b> be at least as long as the expected result (MinBufferLen32
    ///  will allow all values up to 32-bit to be fully formatted.)</param>
    /// <param name="groupSeparatorChar">If non-zero, the digits will be grouped
    ///  separated by this character.</param>
    /// <param name="isNegative">        If <b>true</b> prepend a negative sign.</param>
    /// <param name="pEnd">              If non-null, a pointer to the result's
    ///  terminating null.</param>
    /// <returns>
    /// A pointer to the formatted string. The resulting string is <b>not</b> aligned
    /// to pBuffer; you <b>must</b> use the return pointer.
    /// </returns>
    wchar_t* ToStr32U(uint32_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar = 0, bool isNegative = false, wchar_t** pEnd = nullptr);

    /// <summary>
    /// Convert a 32-bit signed value to a string.
    /// </summary>
    /// <param name="val">               The value to convert.</param>
    /// <param name="pBuffer">           The buffer to receive the formatted string.
    ///  This <b>must</b> be at least as long as the expected result (MinBufferLen32
    ///  will allow all values up to 32-bit to be fully formatted.)</param>
    /// <param name="groupSeparatorChar">If non-zero, the digits will be grouped
    ///  separated by this character.</param>
    /// <returns>
    /// A pointer to the formatted string. The resulting string is <b>not</b> aligned
    /// to pBuffer; you <b>must</b> use the return pointer.
    /// </returns>
    wchar_t* ToStr32(int32_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar = 0);

    /// <summary>
    /// Convert a 32-bit unsigned value to a string.
    /// </summary>
    /// <param name="val">               The value to convert.</param>
    /// <param name="pBuffer">           The buffer to receive the formatted string.
    ///  This <b>must</b> be at least as long as the expected result (MinBufferLen32
    ///  will allow all values up to 32-bit to be fully formatted.)</param>
    /// <param name="groupSeparatorChar">If non-zero, the digits will be grouped
    ///  separated by this character.</param>
    /// <param name="isNegative">        If <b>true</b> prepend a negative sign.</param>
    /// <param name="pEnd">              If non-null, a pointer to the result's
    ///  terminating null.</param>
    /// <returns>
    /// A pointer to the formatted string. The resulting string is aligned to pBuffer.
    /// </returns>
    wchar_t* ToStrAligned32U(uint32_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar = 0, bool isNegative = false, wchar_t** pEnd = nullptr);

    /// <summary>
    /// Convert a 32-bit signed value to a string.
    /// </summary>
    /// <param name="val">               The value to convert.</param>
    /// <param name="pBuffer">           The buffer to receive the formatted string.
    ///  This <b>must</b> be at least as long as the expected result (MinBufferLen32
    ///  will allow all values up to 32-bit to be fully formatted.)</param>
    /// <param name="groupSeparatorChar">If non-zero, the digits will be grouped
    ///  separated by this character.</param>
    /// <returns>
    /// A pointer to the formatted string. The resulting string is aligned to pBuffer.
    /// </returns>
    wchar_t* ToStrAligned32(int32_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar = 0);

    /// <summary>
    /// Convert an unsigned value to a string.
    /// </summary>
    /// <param name="val">               The value to convert.</param>
    /// <param name="pBuffer">           The buffer to receive the formatted string.
    ///  This <b>must</b> be at least as long as the expected result (MinBufferLen64
    ///  will allow all values up to 64-bit to be fully formatted.)</param>
    /// <param name="groupSeparatorChar">If non-zero, the digits will be grouped
    ///  separated by this character.</param>
    /// <param name="isNegative">        If <b>true</b> prepend a negative sign.</param>
    /// <param name="pEnd">              If non-null, a pointer to the result's
    ///  terminating null.</param>
    /// <returns>
    /// A pointer to the formatted string. The resulting string is <b>not</b> aligned
    /// to pBuffer; you <b>must</b> use the return pointer.
    /// </returns>
    wchar_t* ToStr64U(uint64_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar = 0, bool isNegative = false, wchar_t** pEnd = nullptr);

    wchar_t* ToStrPadded64U(uint64_t val, wchar_t* pBuffer, int minWidth, wchar_t padChar = L' ', wchar_t groupSeparatorChar = 0, bool isNegative = false, wchar_t** pEnd = nullptr);

    /// <summary>
    /// Convert a 64-bit signed value to a string.
    /// </summary>
    /// <param name="val">               The value to convert.</param>
    /// <param name="pBuffer">           The buffer to receive the formatted string.
    ///  This <b>must</b> be at least as long as the expected result (MinBufferLen64
    ///  will allow all values up to 64-bit to be fully formatted.)</param>
    /// <param name="groupSeparatorChar">If non-zero, the digits will be grouped
    ///  separated by this character.</param>
    /// <returns>
    /// A pointer to the formatted string. The resulting string is <b>not</b> aligned
    /// to pBuffer; you <b>must</b> use the return pointer.
    /// </returns>
    wchar_t* ToStr64(int64_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar = 0);

    /// <summary>
    /// Convert a 64-bit unsigned value to a string.
    /// </summary>
    /// <param name="val">               The value to convert.</param>
    /// <param name="pBuffer">           The buffer to receive the formatted string.
    ///  This <b>must</b> be at least as long as the expected result (MinBufferLen64
    ///  will allow all values up to 64-bit to be fully formatted.)</param>
    /// <param name="groupSeparatorChar">If non-zero, the digits will be grouped
    ///  separated by this character.</param>
    /// <param name="isNegative">        If <b>true</b> prepend a negative sign.</param>
    /// <param name="pEnd">              If non-null, a pointer to the result's
    ///  terminating null.</param>
    /// <returns>
    /// A pointer to the formatted string. The resulting string is aligned to pBuffer.
    /// </returns>
    wchar_t* ToStrAligned64U(uint64_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar = 0, bool isNegative = false, wchar_t** pEnd = nullptr);

    /** Convert a 64-bit signed value to a string.
        @param val     The value to convert.
        @param pBuffer The buffer to receive the formatted string.
            This \b must \b be at least as long as the expected result
            (MinBufferLen64 will allow all values up to 64-bit to be fully
            formatted.)
        @param groupSeparatorChar If non-zero, the digits will be grouped
            separated by this character.
        @returns A pointer to the formatted string. The resulting string is
            aligned to \p pBuffer.
     */

    /// <summary>
    /// Convert a 64-bit signed value to a string.
    /// </summary>
    /// <param name="val">               The value to convert.</param>
    /// <param name="pBuffer">           The buffer to receive the formatted string.
    ///  This <b>must</b> be at least as long as the expected result (MinBufferLen64
    ///  will allow all values up to 64-bit to be fully formatted.)</param>
    /// <param name="groupSeparatorChar">If non-zero, the digits will be grouped
    ///  separated by this character.</param>
    /// <returns>
    /// A pointer to the formatted string. The resulting string is aligned to pBuffer.
    /// </returns>
    wchar_t* ToStrAligned64(int64_t val, wchar_t* pBuffer, wchar_t groupSeparatorChar = 0);

    std::wstring& ToStr64U(std::wstring& str, uint64_t val, size_t minWidth, wchar_t fillChar = ' ', wchar_t groupSeparatorChar = 0);

    /// <returns>
    /// A pointer to the character after width.
    /// </returns>
    /// <remarks>
    /// This does NOT null terminate pStr.
    /// </remarks>
    wchar_t* ToStrFixedFast(wchar_t* pStr, uint64_t val, size_t width);

    std::wstring& AppendFixedFast(std::wstring& str, uint64_t val, size_t width);

} // namespace ConvertW

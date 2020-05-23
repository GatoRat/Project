// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

namespace ASCIIW
{
    constexpr wchar_t NUL   = 0x00;
    constexpr wchar_t STX   = 0x02; // Start of Text
    constexpr wchar_t ETX   = 0x03; // End of Text
    constexpr wchar_t ACK   = 0x06;
    constexpr wchar_t BEL   = 0x07;
    constexpr wchar_t BELL  = BEL;
    constexpr wchar_t BS    = 0x08; // Back space
    constexpr wchar_t TAB   = 0x09; // '\t'
    constexpr wchar_t LF    = 0x0a; // '\n'
    constexpr wchar_t VT    = 0x0b;
    constexpr wchar_t VTAB  = VT;
    constexpr wchar_t FF    = 0x0c;
    constexpr wchar_t CR    = 0x0d; // '\r'
    constexpr wchar_t DLE   = 0x10; // Date Line Escape
    constexpr wchar_t NAK   = 0x15;
    constexpr wchar_t SYN   = 0x16; // Synchronous Idle
    constexpr wchar_t ESC   = 0x1b;

    constexpr wchar_t* pTAB    = L"\t";
    constexpr wchar_t* pCR     = L"\r";
    constexpr wchar_t* pLF     = L"\n";
    constexpr wchar_t* pCRLF   = L"\r\n";

    constexpr wchar_t Accent           = L'`';
    constexpr wchar_t Ampersand        = L'&';
    constexpr wchar_t Apostrophe       = L'\'';
    constexpr wchar_t Asterix          = L'*';
    constexpr wchar_t At               = L'@';
    constexpr wchar_t BackSlash        = L'\\';
    constexpr wchar_t Caret            = L'^';
    constexpr wchar_t Colon            = L':';
    constexpr wchar_t Comma            = L',';
    constexpr wchar_t Dash             = L'-';
    constexpr wchar_t Dollar           = L'$';
    constexpr wchar_t DoubleQuote      = L'"';
    constexpr wchar_t Equal            = L'=';
    constexpr wchar_t Exclamation      = L'!';
    constexpr wchar_t Greather         = L'>';
    constexpr wchar_t Hash             = L'#';
    constexpr wchar_t LeftBrace        = L'{';
    constexpr wchar_t LeftBracket      = L'[';
    constexpr wchar_t LeftChevron      = L'<';
    constexpr wchar_t LeftParenthesis  = L'(';
    constexpr wchar_t Less             = L'<';
    constexpr wchar_t Minus            = L'-';
    constexpr wchar_t Percent          = L'%';
    constexpr wchar_t Period           = L'.';
    constexpr wchar_t Plus             = L'+';
    constexpr wchar_t Pound            = L'#';
    constexpr wchar_t Question         = L'?';
    constexpr wchar_t Quote            = L'"';
    constexpr wchar_t RightBrace       = L'}';
    constexpr wchar_t RightBracket     = L']';
    constexpr wchar_t RightChevron     = L'>';
    constexpr wchar_t RightParenthesis = L')';
    constexpr wchar_t Semicolon        = L';';
    constexpr wchar_t SingleQuote      = L'\'';
    constexpr wchar_t Slash            = L'/';
    constexpr wchar_t Space            = L' ';
    constexpr wchar_t Tilda            = L'~';
    constexpr wchar_t Underscore       = L'_';
    constexpr wchar_t VerticalBar      = L'|';

    constexpr wchar_t Zero             = L'0';
    constexpr wchar_t Nine             = L'9';

    constexpr wchar_t UpperA           = L'A';
    constexpr wchar_t UpperF           = L'F';
    constexpr wchar_t UpperT           = L'T';
    constexpr wchar_t UpperZ           = L'Z';

    constexpr wchar_t LowerA           = L'a';
    constexpr wchar_t LowerF           = L'f';
    constexpr wchar_t LowerZ           = L'z';

    /// <summary>
    /// Indicates whether the character is ASCII whitespace.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is ASCII whitespace, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::isspace().)
    /// </remarks>
    constexpr bool IsWhitespace(wchar_t ch)
    {
        return ch > NUL && ch <= Space;
    }

    /// <summary>
    /// Indicates whether the character is a control character.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is a control character, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::isspace().)
    /// </remarks>
    constexpr bool IsCtrl(wchar_t ch)
    {
        return ch > NUL && ch < Space;
    }

    /// <summary>
    /// Indicates whether the character is a control or NUL character.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is a control or NUL character, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::isspace().)
    /// </remarks>
    constexpr bool IsCtrlOrNul(wchar_t ch)
    {
        // VC++ treads wchar_t as unsigned
        return /*ch >= NUL &&*/ ch < Space;
    }

    constexpr bool IsCtrlNulOr127(wchar_t ch)
    {
        // VC++ treads wchar_t as unsigned
        return (/*ch >= NUL &&*/ ch < Space) || (ch == 127);
    }

    /// <summary>
    /// Indicates whether the character is an EOL character.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is zero (0), CR or LF, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale.
    /// </remarks>
    constexpr bool IsEOL(wchar_t ch)
    {
        return ch == 0 || ch == CR || ch == LF;
    }

    /// <summary>
    /// Indicates whether the character is ASCII whitespace.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is ASCII whitespace, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::isdigit().)
    /// </remarks>
    constexpr bool IsDigit(wchar_t ch)
    {
        return ch >= Zero && ch <= Nine;
    }

    /// <summary>
    /// Indicates whether the character is ASCII upper case.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is ASCII upper case, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::isupper().)
    /// </remarks>
    constexpr bool IsUpper(wchar_t ch)
    {
        return ch >= UpperA && ch <= UpperZ;
    }

    /// <summary>
    /// Indicates whether the character is ASCII lower case.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is ASCII lower case, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::islower().)
    /// </remarks>
    constexpr bool IsLower(wchar_t ch)
    {
        return ch >= LowerA && ch <= LowerZ;
    }

    /// <summary>
    /// Indicates whether the character is ASCII alpha character.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is ASCII alpha, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::isalpha().)
    /// </remarks>
    constexpr bool IsAlpha(wchar_t ch)
    {
        return IsUpper(ch) || IsLower(ch);
    }

    /// <summary>
    /// Indicates whether the character is ASCII alpha or numberic character.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is ASCII alpha or numeric, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::isalnum().)
    /// </remarks>
    constexpr bool IsAlphaNumeric(wchar_t ch)
    {
        return IsAlpha(ch) || IsDigit(ch);
    }

    /// <summary>
    /// Indicates whether the character is ASCII hexadecimal character.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is ASCII hexadecimal, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::isxdigit().)
    /// </remarks>
    constexpr bool IsHex(wchar_t ch)
    {
        return IsDigit(ch) || (ch >= UpperA && ch <= UpperF) || (ch >= LowerA && ch <= LowerF);
    }

    /// <summary>
    /// Indicates whether the character is a printable ASCII character.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is a printable ASCII character, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale.
    /// </remarks>
    constexpr bool IsPrintable(wchar_t ch)
    {
        return ch >= Space && ch < 127;
    }

    /// <summary>
    /// Indicates whether the character is a visible printable ASCII character.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is a visible printable ASCII character, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale.
    /// </remarks>
    constexpr bool IsPrintableNoSpace(wchar_t ch)
    {
        return ch > Space && ch < 127;
    }

#if __has_include(<qstring.h>)
    constexpr bool IsCtrlOrNonChar(QChar ch)
    {
        return (ch >= NUL && ch < Space) || (ch == 127) || ch.isNonCharacter();
    }

    constexpr bool IsPrintable(QChar ch)
    {
        return ch >= Space && ch < 127 && !ch.isNonCharacter();
    }

    constexpr bool IsPrintableNoSpace(QChar ch)
    {
        return ch > Space && ch < 127 && !ch.isNonCharacter();
    }
#endif

    /// <summary>
    /// Non-checking conversion of ASCII digit to a value.
    /// </summary>
    /// <param name="ch">The character to convert.</param>
    /// <returns>The value of the digit.</returns>
    /// <remarks>
    /// This assumes that IsDigit() returns <b>true</b>, otherwise the return value
    /// will be meaningless.
    /// </remarks>
    constexpr int DigitToVal(wchar_t ch)
    {
        return static_cast<int>(ch - Zero);
    }

    /// <summary>
    /// Checking conversion of ASCII digit to a value.
    /// </summary>
    /// <param name="ch">The character to convert.</param>
    /// <returns>The value of the digit.</returns>
    /// <remarks>
    /// If ch is a digit, this returns number >= zero (0), otherwise it returns
    /// a negative number.
    /// </remarks>
    constexpr int DigitToValChecked(wchar_t ch)
    {
        return IsDigit(ch) ? DigitToVal(ch) : -1;
    }

    /// <summary>
    /// Quick conversion of ASCII hexadecimal to a value.
    /// </summary>
    /// <param name="ch">The character to convert.</param>
    /// <returns>
    /// The value of the character or 0 if ch is not an ASCII hexadecimal character.
    /// </returns>
    constexpr int HexToVal(wchar_t ch)
    {
        int val = 0;

        if (ch >= UpperA && ch <= UpperF)
        {
            val = 10 + (ch - UpperA);
        }
        else if (ch >= LowerA && ch <= LowerF)
        {
            val = 10 + (ch - LowerA);
        }
        else if (IsDigit(ch))
        {
            val = DigitToVal(ch);
        }

        return val;
    }
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

namespace ASCII
{
    constexpr char NUL   = 0x00;
    constexpr char STX   = 0x02; // Start of Text
    constexpr char ETX   = 0x03; // End of Text
    constexpr char ACK   = 0x06;
    constexpr char BEL   = 0x07;
    constexpr char BELL  = BEL;
    constexpr char BS    = 0x08; // Back space
    constexpr char TAB   = 0x09; // '\t'
    constexpr char LF    = 0x0a; // '\n'
    constexpr char VT    = 0x0b;
    constexpr char VTAB  = VT;
    constexpr char FF    = 0x0c;
    constexpr char CR    = 0x0d; // '\r'
    constexpr char DLE   = 0x10; // Date Line Escape
    constexpr char NAK   = 0x15;
    constexpr char SYN   = 0x16; // Synchronous Idle
    constexpr char ESC   = 0x1b;

    //extern const char* pTAB;
    //extern const char* pCR;
    //extern const char* pLF;
    //extern const char* pCRLF;

    constexpr const char* pTAB    = "\t";
    constexpr const char* pCR     = "\r";
    constexpr const char* pLF     = "\n";
    constexpr const char* pCRLF   = "\r\n";

    constexpr char Accent           = '`';
    constexpr char Ampersand        = '&';
    constexpr char Apostrophe       = '\'';
    constexpr char Asterix          = '*';
    constexpr char At               = '@';
    constexpr char BackSlash        = '\\';
    constexpr char Caret            = '^';
    constexpr char Colon            = ':';
    constexpr char Comma            = ',';
    constexpr char Dash             = '-';
    constexpr char Dollar           = '$';
    constexpr char DoubleQuote      = '"';
    constexpr char Equal            = '=';
    constexpr char Exclamation      = '!';
    constexpr char Greather         = '>';
    constexpr char Hash             = '#';
    constexpr char LeftBrace        = '{';
    constexpr char LeftBracket      = '[';
    constexpr char LeftChevron      = '<';
    constexpr char LeftParenthesis  = '(';
    constexpr char Less             = '<';
    constexpr char Minus            = '-';
    constexpr char Percent          = '%';
    constexpr char Period           = '.';
    constexpr char Plus             = '+';
    constexpr char Pound            = '#';
    constexpr char Question         = '?';
    constexpr char Quote            = '"';
    constexpr char RightBrace       = '}';
    constexpr char RightBracket     = ']';
    constexpr char RightChevron     = '>';
    constexpr char RightParenthesis = ')';
    constexpr char Semicolon        = ';';
    constexpr char SingleQuote      = '\'';
    constexpr char Slash            = '/';
    constexpr char Space            = ' ';
    constexpr char Tilda            = '~';
    constexpr char Underscore       = '_';
    constexpr char VerticalBar      = '|';

    constexpr char Zero             = '0';
    constexpr char Nine             = '9';

    constexpr char UpperA           = 'A';
    constexpr char UpperF           = 'F';
    constexpr char UpperT           = 'T';
    constexpr char UpperZ           = 'Z';

    constexpr char LowerA           = 'a';
    constexpr char LowerF           = 'f';
    constexpr char LowerZ           = 'z';

    /// <summary>
    /// Indicates whether the character is ASCII whitespace.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is ASCII whitespace, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale (unlike std::isspace().)
    /// </remarks>
    constexpr bool IsWhitespace(char ch)
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
    constexpr bool IsCtrl(char ch)
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
    constexpr bool IsCtrlOrNul(char ch)
    {
        return ch >= NUL && ch < Space;
    }

    constexpr bool IsCtrlNulOr127(char ch)
    {
        return (ch >= NUL && ch < Space) || (ch == 127);
    }

    /// <summary>
    /// Indicates whether the character is an EOL character.
    /// </summary>
    /// <param name="ch">The character to check.</param>
    /// <returns><b>true</b> if ch is zero (0), CR or LF, else <b>false</b>.</returns>
    /// <remarks>
    /// This is optimized for ASCII, ignoring locale.
    /// </remarks>
    constexpr bool IsEOL(char ch)
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
    constexpr bool IsDigit(char ch)
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
    constexpr bool IsUpper(char ch)
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
    constexpr bool IsLower(char ch)
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
    constexpr bool IsAlpha(char ch)
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
    constexpr bool IsAlphaNumeric(char ch)
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
    constexpr bool IsHex(char ch)
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
    constexpr bool IsPrintable(char ch)
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
    constexpr bool IsPrintableNoSpace(char ch)
    {
        return ch > Space && ch < 127;
    }

    /// <summary>
    /// Converts a character in the range a-z to upper case.
    /// </summary>
    /// <param name="ch">The character to convert.</param>
    /// <returns>If the character is in the range a-z, the comparable character
    ///     in the range A-Z, else the original character.</returns>
    constexpr char ToUpper(char ch)
    {
        return IsLower(ch) ? (ch - (LowerA - UpperA)) : ch;
    }

    /// <summary>
    /// Converts a character in the range A-Z to lower case.
    /// </summary>
    /// <param name="ch">The character to convert.</param>
    /// <returns>If the character is in the range A-Z, the comparable character
    ///     in the range a-z, else the original character.</returns>
    constexpr char ToLower(char ch)
    {
        return IsUpper(ch) ? (ch + (LowerA - UpperA)) : ch;
    }

    /// <summary>
    /// Non-checking conversion of ASCII digit to a value.
    /// </summary>
    /// <param name="ch">The character to convert.</param>
    /// <returns>The value of the digit.</returns>
    /// <remarks>
    /// This assumes that IsDigit() returns <b>true</b>, otherwise the return value
    /// will be meaningless.
    /// </remarks>
    constexpr int DigitToVal(char ch)
    {
        return static_cast<int>(ch - Zero);
    }

    /// <summary>
    /// Non-checking conversion of ASCII digit to a size_t value.
    /// </summary>
    /// <param name="ch">The character to convert.</param>
    /// <returns>The value of the digit.</returns>
    /// <remarks>
    /// This assumes that IsDigit() returns <b>true</b>, otherwise the return value
    /// will be meaningless.
    /// </remarks>
    constexpr size_t DigitToIndex(char ch)
    {
        return static_cast<size_t>(ch - Zero);
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
    constexpr int DigitToValChecked(char ch)
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
    constexpr int HexToVal(char ch)
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
        else if (IsDigit(ch)) //-V2516
        {
            val = DigitToVal(ch);
        }

        return val;
    }
}

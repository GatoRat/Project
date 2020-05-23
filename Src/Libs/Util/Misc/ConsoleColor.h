// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include <ostream>
#include "../System/_lean_windows_.h"

namespace ConsoleColor
{
    struct color
    {
        color(uint16_t attribute) : _color(attribute) {};
        uint16_t _color;
    };

    template <class _Elem, class _Traits>
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& i, color& c)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout, c._color);
        return i;
    }

    inline std::ostream& setcolor(std::ostream& ostr, WORD attributes)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout, attributes);
        return ostr;
    }

    // Poor readability
    inline std::ostream& redLo(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_RED);
    }

    inline std::ostream& greenLo(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_GREEN);
    }

    // VERY hard to read
    inline std::ostream& blueLo(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_BLUE);
    }

    inline std::ostream& yellowLo(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_GREEN | FOREGROUND_RED);
    }

    inline std::ostream& cyanLo(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    inline std::ostream& purpleLo(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_BLUE | FOREGROUND_RED);
    }

    // Excellent readability
    inline std::ostream& redHi(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }

    // Excellent readability
    inline std::ostream& greenHi(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }

    // hard to read
    inline std::ostream& blueHi(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    // Excellent readability
    inline std::ostream& yellowHi(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    }

    // Excellent readability
    inline std::ostream& cyanHi(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    inline std::ostream& purpleHi(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    }

    inline std::ostream& white(std::ostream& ostr)
    {
        return setcolor(ostr, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
} // namespace ConsoleColor

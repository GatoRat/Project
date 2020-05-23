// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "../Time/TimeStamp.h"

TimeStamp GetSysTime     (bool local = true, int* pDayOfWeek = nullptr);
TimeStamp GetSysLocalTime(int* pDayOfWeek = nullptr);
TimeStamp GetSysUtcTime  (int* pDayOfWeek = nullptr);

wchar_t* FormatLongDate (TimeStamp timeStamp, wchar_t* pBuffer, size_t bufferLen, bool includeDayOfWeek = true);
wchar_t* FormatShortDate(TimeStamp timeStamp, wchar_t* pBuffer, size_t bufferLen);
wchar_t* FormatShortTime(TimeStamp timeStamp, wchar_t* pBuffer, size_t bufferLen);

template <size_t _size>
wchar_t* FormatLongDate(TimeStamp timeStamp, wchar_t(&dst)[_size], bool includeDayOfWeek = true)
{
    return FormatLongDate(timeStamp, dst, _size, includeDayOfWeek);
}

template <size_t _size>
wchar_t* FormatShortDate(TimeStamp timeStamp, wchar_t(&dst)[_size])
{
    return FormatShortDate(timeStamp, dst, _size);
}

template <size_t _size>
wchar_t* FormatShortTime(TimeStamp timeStamp, wchar_t(&dst)[_size])
{
    return FormatShortTime(timeStamp, dst, _size);
}

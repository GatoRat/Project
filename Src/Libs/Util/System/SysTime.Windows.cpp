// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SysTime.h"
#include "_lean_windows_.h"

using namespace std;

TimeStamp GetSysTime(bool local, int* pDayOfWeek)
{
    SYSTEMTIME sysTime;
    if (local)
    {
        GetLocalTime(&sysTime);
    }
    else
    {
        GetSystemTime(&sysTime);
    }

    TimeStamp timeStamp;

    timeStamp.dt.year = static_cast<uint8_t>(sysTime.wYear - TimeStamp::BaseYear);
    timeStamp.dt.mon  = static_cast<uint8_t>(sysTime.wMonth);
    timeStamp.dt.day  = static_cast<uint8_t>(sysTime.wDay);
    timeStamp.dt.hour = static_cast<uint8_t>(sysTime.wHour);
    timeStamp.dt.min  = static_cast<uint8_t>(sysTime.wMinute);
    timeStamp.dt.sec  = static_cast<uint8_t>(sysTime.wSecond);
    timeStamp.dt.msec = static_cast<uint16_t>(sysTime.wMilliseconds);

    if (pDayOfWeek)
    {
        *pDayOfWeek = static_cast<int>(sysTime.wDayOfWeek);
    }

    return timeStamp;
}

constexpr const wchar_t* pLongDateFormat[2]
{
    L"ddd MMM d',' yyyy",
    L"MMM d',' yyyy"
};

wchar_t* FormatLongDate(const SYSTEMTIME& sysTime, wchar_t* pBuffer, size_t bufferLen, const wchar_t* pDateFormat)
{
    int len = GetDateFormatW(LOCALE_USER_DEFAULT, 0, &sysTime, pDateFormat, pBuffer, static_cast<int>(bufferLen));
    GetTimeFormatW(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &sysTime, nullptr, &pBuffer[len], static_cast<int>(bufferLen) - len); //-V108

    return pBuffer;
}

wchar_t* FormatLongDate(const SYSTEMTIME& sysTime, wchar_t* pBuffer, size_t bufferLen, bool includeDayOfWeek)
{
    return FormatLongDate(sysTime, pBuffer, bufferLen, pLongDateFormat[includeDayOfWeek ? 0 : 1]);
}

wchar_t* FormatLongDate(TimeStamp timeStamp, wchar_t* pBuffer, size_t bufferLen, bool includeDayOfWeek)
{
    SYSTEMTIME sysTime;

    sysTime.wYear         = timeStamp.getYear();
    sysTime.wMonth        = timeStamp.dt.mon;
    sysTime.wDayOfWeek    = 0;
    sysTime.wDay          = timeStamp.dt.day;
    sysTime.wHour         = timeStamp.dt.hour;
    sysTime.wMinute       = timeStamp.dt.min;
    sysTime.wSecond       = timeStamp.dt.sec;
    sysTime.wMilliseconds = timeStamp.dt.msec;

    return FormatLongDate(sysTime, pBuffer, bufferLen, includeDayOfWeek);
}

wchar_t* FormatShortDate(TimeStamp timeStamp, wchar_t* pBuffer, size_t bufferLen)
{
    SYSTEMTIME sysTime;
    memset(&sysTime, 0, sizeof(sysTime));

    sysTime.wYear  = timeStamp.getYear();
    sysTime.wMonth = timeStamp.dt.mon;
    sysTime.wDay   = timeStamp.dt.day;

    GetDateFormatW(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &sysTime, nullptr, pBuffer, static_cast<int>(bufferLen));

    return pBuffer;
}

wchar_t* FormatShortTime(TimeStamp timeStamp, wchar_t* pBuffer, size_t bufferLen)
{
    SYSTEMTIME sysTime;
    memset(&sysTime, 0, sizeof(sysTime));

    sysTime.wHour   = timeStamp.dt.hour;
    sysTime.wMinute = timeStamp.dt.min;

    GetTimeFormatW(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &sysTime, nullptr, pBuffer, static_cast<int>(bufferLen));

    return pBuffer;
}

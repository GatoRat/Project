// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TimeStamp.h"
#include <algorithm>
#include <ctime>
#include "../String/ASCII.h"
#include "../String/Convert.h"
#include "../String/Str.h"
#include "../System/_lean_windows_.h"
#include "../Time/Defines.Time.h"

using namespace std;

uint8_t TimeStamp::ToInternalYear(uint16_t year)
{
    if (year > UINT8_MAX)
    {
        if (year < BaseYear)
        {
            year = 0;
        }
        else
        {
            year -= BaseYear;
            if (year > UINT8_MAX)
            {
                year = UINT8_MAX;
            }
        }
    }

    return static_cast<uint8_t>(year);
}

uint16_t TimeStamp::ToActualYear(uint8_t year)
{
    return year + BaseYear;
}

TimeStamp::TimeStamp(Set setVal)
{
    set(setVal);
}

TimeStamp::TimeStamp(int64_t val_)
    : val{ static_cast<uint64_t>(val_) }
{
}

TimeStamp::TimeStamp(const char* pStr, bool secondsRequired, const char** pEnd, bool* pError)
{
    bool result = set(pStr, secondsRequired, pEnd);
    if (pError)
    {
        *pError = !result;
    }
}

TimeStamp::TimeStamp(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint16_t msec)
    : dt{ msec, sec, min, hour, day, mon, ToInternalYear(year) }
{
}

TimeStamp::TimeStamp(const tm& tm)
{
    operator=(tm);
}

TimeStamp::TimeStamp(TimeStamp lhs, TimeStamp rhs)
{
    if (lhs.isTimeOnly())
    {
        swap(lhs, rhs);
    }

    if (rhs.isTimeOnly())
    {
        val = lhs.val;

        dt.msec = rhs.dt.msec;
        dt.sec  = rhs.dt.sec;
        dt.min  = rhs.dt.min;
        dt.hour = rhs.dt.hour;
    }
    else
    {
        val = rhs.val;
    }
}

void TimeStamp::operator=(const tm& tm)
{
    dt.year = ToInternalYear(static_cast<uint16_t>(tm.tm_year + BaseYear_tm));
    dt.mon  = static_cast<uint8_t>(tm.tm_mon + 1);
    dt.day  = static_cast<uint8_t>(tm.tm_mday);
    dt.hour = static_cast<uint8_t>(tm.tm_hour);
    dt.min  = static_cast<uint8_t>(tm.tm_min);
    dt.sec  = static_cast<uint8_t>(tm.tm_sec);
    dt.msec = 0;
}

void TimeStamp::set(Set setVal)
{
    switch (setVal)
    {
        case Set::ToCurTime:
        {
            setCurTime();
            break;
        }

        case Set::ToMin:
            val = 0;
            break;

        case Set::ToMax:
            val = UINT64_MAX;
            break;
    }
}

void TimeStamp::set(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint16_t msec)
{
    dt.year = ToInternalYear(year);
    dt.mon  = mon;
    dt.day  = day;
    dt.hour = hour;
    dt.min  = min;
    dt.sec  = sec;
    dt.msec = msec;
}

int TimeStamp::setCurTime()
{
#ifdef _WIN32
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);

    dt.year = ToInternalYear(sysTime.wYear);
    dt.mon  = static_cast<uint8_t>(sysTime.wMonth);
    dt.day  = static_cast<uint8_t>(sysTime.wDay);
    dt.hour = static_cast<uint8_t>(sysTime.wHour);
    dt.min  = static_cast<uint8_t>(sysTime.wMinute);
    dt.sec  = static_cast<uint8_t>(sysTime.wSecond);
    dt.msec = sysTime.wMilliseconds;

    return static_cast<int>(sysTime.wDayOfWeek);
#else
    const time_t curTime = time(nullptr);
    struct tm tm;
    localtime_s(&tm, &curTime);
    operator=(tm);
    return tm.tm_wday;
#endif
}

// "2017-10-04T22:59:33.000Z"

bool TimeStamp::set(const char* pStr, bool secondsRequired, const char** pEnd)
{
    bool success = false;

    set(Set::ToMin);

    if (pStr)
    {
        bool hasSeconds = false;

        dt.year = ToInternalYear(static_cast<uint16_t>(Convert::Quick4(pStr)));
        if (*pStr == ASCII::Dash)
        {
            dt.mon = static_cast<uint8_t>(Convert::Quick2(++pStr));
            if (*pStr == ASCII::Dash)
            {
                dt.day = static_cast<uint8_t>(Convert::Quick2(++pStr));
                if (*pStr == ASCII::Space || *pStr == ASCII::UpperT)
                {
                    dt.hour = static_cast<uint8_t>(Convert::Quick2(++pStr));
                    if (*pStr == ASCII::Colon)
                    {
                        dt.min = static_cast<uint8_t>(Convert::Quick2(++pStr));

                        if (*pStr == ASCII::Colon)
                        {
                            dt.sec = static_cast<uint8_t>(Convert::Quick2(++pStr));
                            hasSeconds = true;
                            if (*pStr == ASCII::Period)
                            {
                                dt.msec = static_cast<uint16_t>(Convert::Quick3(++pStr));
                            }
                        }
                        success = true;
                    }

                    if (*pStr == ASCII::UpperZ)
                    {
                        // Zulu time: for now ignore
                        ++pStr;
                    }
                }
            }
        }

        if (success)
        {
            success = validate();
            if (secondsRequired && !hasSeconds)
            {
                success = false;
            }
        }
    }

    if (pEnd)
    {
        *pEnd = pStr;
    }

    return success;
}

bool TimeStamp::set2(const char* pStr, bool secondsRequired, const char** pEnd)
{
    bool success = false;

    set(Set::ToMin);

    if (pStr)
    {
        if (*pStr == 'T')
        {
            success = setFromTime(++pStr);
        }
        else if (!(pStr[0] && pStr[1] && pStr[2] == ASCII::Colon))
        {
            success = set(pStr, secondsRequired, pEnd);
        }
        else
        {
            success = setFromTime(pStr);
        }
    }

    return success;
}

bool TimeStamp::setFromTime(const char* pStr, bool secondsRequired, const char** pEnd)
{
    bool success = false;

    set(Set::ToMin);

    if (pStr)
    {
        bool hasSeconds = false;

        dt.hour = static_cast<uint8_t>(Convert::Quick2(pStr));
        if (*pStr == ASCII::Colon)
        {
            dt.min = static_cast<uint8_t>(Convert::Quick2(++pStr));
            if (*pStr == ASCII::Colon)
            {
                dt.sec = static_cast<uint8_t>(Convert::Quick2(++pStr));
                hasSeconds = true;
                if (*pStr == ASCII::Period)
                {
                    dt.msec = static_cast<uint16_t>(Convert::Quick3(++pStr));
                }
            }
            success = true;
        }

        if (*pStr == ASCII::UpperZ)
        {
            // Zulu time: for now ignore
            ++pStr;
        }

        if (success)
        {
            success = validateTimeOnly();
            if (secondsRequired && !hasSeconds)
            {
                success = false;
            }
        }
    }

    if (pEnd)
    {
        *pEnd = pStr;
    }

    return success;
}

bool TimeStamp::validate() const
{
    bool success = false;

    if (val > 0 && val < UINT64_MAX &&
        dt.mon  >= MinMonth   && dt.mon  <= MaxMonth  &&
        dt.day  >= MinDay     && dt.day  <= MaxDay    &&
        dt.hour <= MaxHour    &&
        dt.min  <= MaxMinute  &&
        dt.sec  <= MaxSeconds &&
        dt.msec <= MaxMilliseconds)
    {
        static int daysInMon[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        const uint16_t actualYear = ToInternalYear(dt.year);

        if (dt.mon == February &&
            ((actualYear % 100) != 0 || ((actualYear % 400) == 0 && (actualYear % 4) == 0))) //-V112
        {
            success = dt.day <= 29;
        }
        else if (dt.day <= daysInMon[dt.mon - 1]) //-V557
        {
            success = true;
        }
    }

    return success;
}

bool TimeStamp::validateTimeOnly() const
{
    return dt.year == 0 &&
           dt.mon == 0 &&
           dt.day == 0 &&
           dt.hour <= MaxHour &&
           dt.min <= MaxMinute &&
           dt.sec <= MaxSeconds &&
           dt.msec <= MaxMilliseconds;
}

bool TimeStamp::isTimeOnly() const
{
    return dt.year == 0 &&
           dt.mon == 0 &&
           dt.day == 0;
}

string TimeStamp::toStr(StrStyle strStyle) const
{
    string str;
    toStrAppend(str, strStyle);
    //str.reserve(MaxStrLen);

    //// YYYY-MM-DD HH:MM[:SS[.mmm]][Z]
    //// YYYY-MM-DDTHH:MM:SS.mmmZ

    //Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.year) + BaseYear, 4); //-V112
    //str += ASCII::Dash;
    //Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.mon), 2);
    //str += ASCII::Dash;
    //Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.day), 2);
    //str += strict8601 ? ASCII::UpperT : ASCII::Space;
    //Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.hour), 2);
    //str += ASCII::Colon;
    //Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.min), 2);
    //if (includeSeconds)
    //{
    //    str += ASCII::Colon;
    //    Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.sec), 2);

    //    if (includeMilliseconds)
    //    {
    //        str += ASCII::Period;
    //        Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.msec), 3);
    //    }
    //}
    //if (strict8601)
    //{
    //    str += ASCII::UpperZ;
    //}

    return str;
}

string& TimeStamp::toStrAppend(string& str, StrStyle strStyle) const
{
    Str::ReserveAdditional(str, MaxStrLen);

    if (strStyle & UseStrict8601)
    {
        strStyle = static_cast<StrStyle>(UseT | UseZ | UseSeconds | (strStyle & UseMilliseconds));
    }

    if (!(strStyle & TimeOnly))
    {
        // YYYY-MM-DD HH:MM[:SS[.mmm]][Z]
        // YYYY-MM-DDTHH:MM:SS.mmmZ

        if (strStyle & UseTwoDigitYear)
        {
            Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.year), 2); //-V112
        }
        else
        {
            Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.year) + BaseYear, 4); //-V112
        }
        str += ASCII::Dash;
        Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.mon), 2);
        str += ASCII::Dash;
        Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.day), 2);
        str += strStyle & UseT ? ASCII::UpperT : ASCII::Space;
    }

    Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.hour), 2);
    str += ASCII::Colon;
    Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.min), 2);
    if (strStyle & (UseSeconds | UseMilliseconds))
    {
        str += ASCII::Colon;
        Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.sec), 2);

        if (strStyle & UseMilliseconds)
        {
            str += ASCII::Period;
            Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.msec), 3);
        }
    }

    if (strStyle & UseZ)
    {
        str += ASCII::UpperZ;
    }

    return str;
}

string& TimeStamp::toStrPackedAppend(string& str, StrStyle strStyle, char dateTimeSeparator) const
{
    Str::ReserveAdditional(str, MaxPackedStrLen);

    // YYMMDD_HHMM[SS[mmm]]

    Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.year), 2);
    Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.mon) , 2);
    Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.day) , 2);

    if (ASCII::IsPrintable(dateTimeSeparator))
    {
        str += dateTimeSeparator;
    }

    Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.hour), 2);
    Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.min) , 2);
    if (strStyle & (UseSeconds | UseMilliseconds))
    {
        Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.sec), 2);

        if (strStyle & UseMilliseconds)
        {
            Convert::AppendFixedFast(str, static_cast<uint64_t>(dt.msec), 3);
        }
    }

    return str;
}

string TimeStamp::toStrPacked(StrStyle strStyle, char dateTimeSeparator) const
{
    string str;
    toStrPackedAppend(str, strStyle, dateTimeSeparator);
    return str;
}

uint16_t TimeStamp::getYear() const
{
    return ToActualYear(dt.year);
}

uint64_t TimeStamp::GetMillisecondsFrom1970() const
{
    struct tm tm1;
    memset(&tm1, 0, sizeof(tm1));

    tm1.tm_year = 1970 - 1900;
    tm1.tm_mday = 1;

    struct tm tm2;
    memset(&tm2, 0, sizeof(tm1));

    tm2.tm_year = static_cast<int>((dt.year + 2000) - 1900);
    tm2.tm_mon  = static_cast<int>(dt.mon - 1             );
    tm2.tm_mday = static_cast<int>(dt.day                 );

    tm2.tm_hour = static_cast<int>(dt.hour                );
    tm2.tm_min  = static_cast<int>(dt.min                 );
    tm2.tm_sec  = static_cast<int>(dt.sec                 );

    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);

    return (static_cast<uint64_t>(time2 - time1) * MILLISECONDS_PER_SECOND) + dt.msec;
}

uint64_t TimeStamp::GetCurTime()
{
    TimeStamp ts(TimeStamp::Set::ToCurTime);
    return ts.val;
}

int64_t TimeStamp::GetCurTimeI()
{
    TimeStamp ts(TimeStamp::Set::ToCurTime);
    return static_cast<int64_t>(ts.val);
}
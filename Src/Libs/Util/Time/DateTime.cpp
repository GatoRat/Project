// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "DateTime.h"
#include <chrono>
#include <iomanip>
#include <locale>
#include <sstream>
#include "../Error/XceptionT.h"
#include "../Misc/Defines.Misc.h"
#include "../String/ASCII.h"
#include "../String/Convert.h"
#include "../String/Str.h"
#include "ParseDate.h"

using namespace std;
using chrono::system_clock;

const char* DateTime::DefaultFormat = "%F %T";

DateTime::DateTime(bool setToNow)
{
    if (setToNow)
    {
        reset();
    }
    else
    {
        clear();
    }
}

DateTime::DateTime(const tm& tm_, uint32_t microseconds)
{
    reset(tm_, microseconds);
}

DateTime::DateTime(int64_t microsecondsSinceEpoch)
{
    reset(microsecondsSinceEpoch);
}

DateTime::DateTime(const char* pStr, Precision precision, const char** pEnd)
{
    if (Str::IsNullOrWhiteSpace(pStr))
    {
        throw Xception("Invalid Argument 'pStr'");
    }

    if (!reset(pStr, precision, pEnd))
    {
        throw Xception("Invalid date string");
    }
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint32_t microseconds)
{
    reset(year, month, day, hour, minute, second, microseconds);
}

DateTime::operator TimeStamp() const
{
    return toTimeStamp();
}

TimeStamp DateTime::toTimeStamp(bool includeMilliseconds) const
{
    TimeStamp timeStamp(_tm);
    if (includeMilliseconds)
    {
        timeStamp.dt.msec = static_cast<uint16_t>(_microseconds / MICROSECONDS_PER_MILLISECOND);
    }
    return timeStamp;
}

TimeStamp DateTime::toTimeStamp(Precision precision) const
{
    TimeStamp timeStamp(getTm(precision));
    if (precision >= Precision::Microsecond)
    {
        timeStamp.dt.msec = static_cast<uint16_t>(_microseconds / MICROSECONDS_PER_MILLISECOND);
    }
    return timeStamp;
}

int DateTime::compare(const DateTime& other, Precision precision) const
{
    int result = 0;

    const int64_t thisTime = microsecondsSince1970(precision);
    const int64_t otherTime = other.microsecondsSince1970(precision);

    if (thisTime > otherTime)
    {
        result = 1;
    }
    else if (thisTime < otherTime)
    {
        result = -1;
    }

    return result;
}

int DateTime::compare(const tm& other, Precision precision) const
{
    const DateTime otherDateTime(other, 0);
    return this->compare(otherDateTime, precision);
}

bool DateTime::isSame(const DateTime& other) const
{
    return _microseconds == other._microseconds && memcmp(&_tm, &other._tm, sizeof(_tm)) == 0;
}

bool DateTime::operator==(const DateTime& other) const
{
    return compare(other, Precision::Microsecond) == 0;
}

bool DateTime::operator<(const DateTime& other) const
{
    return compare(other, Precision::Microsecond) < 0;
}

bool DateTime::operator<=(const DateTime& other) const
{
    return compare(other, Precision::Microsecond) <= 0;
}

bool DateTime::operator>(const DateTime& other) const
{
    return compare(other, Precision::Microsecond) > 0;
}

bool DateTime::operator>=(const DateTime& other) const
{
    return compare(other, Precision::Microsecond) >= 0;
}

void DateTime::setToCurTime()
{
#ifdef _WIN32
    const system_clock::duration duration = chrono::system_clock::now().time_since_epoch();

    const chrono::seconds seconds = chrono::duration_cast<chrono::seconds>(duration);
    _microseconds = static_cast<uint32_t>(chrono::duration_cast<chrono::microseconds>(duration - seconds).count() & UINT32_MAX);
    const time_t timet = static_cast<time_t>(seconds.count() & UINT32_MAX);
    localtime_s(&_tm, &timet);
#else
    timespec timeSpec;
    clock_gettime(CLOCK_REALTIME, &timeSpec);

    _microseconds = timeSpec.tv_nsec / NANOSECONDS_PER_MICROSECOND;
    localtime_r(&timeSpec.tv_sec, &_tm);
#endif
}

void DateTime::reset()
{
    setToCurTime();
}

void DateTime::reset(int64_t microsecondsSinceEpoch)
{
    _microseconds = microsecondsSinceEpoch % MICROSECONDS_PER_SECOND;
    const time_t seconds = static_cast<time_t>(microsecondsSinceEpoch / MICROSECONDS_PER_SECOND);
    localtime_s(&_tm, &seconds);
}

void DateTime::reset(const tm& tm_, uint32_t microseconds)
{
    _tm = tm_;
    _microseconds = microseconds % MICROSECONDS_PER_SECOND;
}

bool DateTime::reset(const char* pStr, bool secondsRequired, const char** pEnd)
{
    _microseconds = 0;
    return ParseDate::Basic(pStr, &_tm, secondsRequired, pEnd);
}

bool DateTime::reset(const string& str, bool secondsRequired)
{
    const char* pEnd;
    return reset(str.data(), secondsRequired, &pEnd) && *pEnd == 0;
}

void DateTime::reset(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint32_t microseconds)
{
    memset(&_tm, 0, sizeof(_tm));

    _tm.tm_year = year - Epoch;
    _tm.tm_mon  = month;
    _tm.tm_mday = day;
    _tm.tm_hour = hour;
    _tm.tm_min  = minute;
    _tm.tm_sec  = second;
    _microseconds = microseconds;
}

void DateTime::resetTime(uint8_t hour, uint8_t minute, uint8_t second, uint32_t microseconds)
{
    _tm.tm_hour = hour;
    _tm.tm_min  = minute;
    _tm.tm_sec  = second;
    _microseconds = microseconds;
}

void DateTime::resetUsingFILETIME(const void* pFileTime)
{
    if (pFileTime)
    {
        constexpr uint64_t EPOCH_DIFFERENCE_MICROSECONDS = 11644473600000000ull;

        const uint64_t microseconds = ((*reinterpret_cast<const uint64_t*>(pFileTime)) / 10) - EPOCH_DIFFERENCE_MICROSECONDS; //-V206
        _microseconds = microseconds % MICROSECONDS_PER_SECOND;

        const time_t seconds = static_cast<time_t>(microseconds / MICROSECONDS_PER_SECOND);
#ifdef _WIN32
        localtime_s(&_tm, &seconds);
#else
        // localtime_r(&seconds, &_tm);
#endif
    }
    else
    {
        reset();
    }
}

uint32_t DateTime::secondOfDay() const
{
    return (static_cast<uint32_t>(_tm.tm_hour) * SECONDS_PER_HOUR) +
           (static_cast<uint32_t>(_tm.tm_min) * SECONDS_PER_MINUTE) + _tm.tm_sec;
}

int64_t DateTime::secondsSince1970() const
{
    tm tm_(_tm);

//#ifdef _WIN32
//    auto val = _mkgmtime(&tm_);
//#else
    const auto val = mktime(&tm_);
//#endif
    return static_cast<int64_t>(val < 0 ? 0 : val);
}

int64_t DateTime::microsecondsSince1970() const
{
    return (static_cast<int64_t>(secondsSince1970()) * MICROSECONDS_PER_SECOND) + _microseconds;
}

int64_t DateTime::secondsSinceEpoch() const
{
    return static_cast<int64_t>(secondsSince1970()) + SecondsFromEpochTo1970;
}

int64_t DateTime::microsecondsSinceEpoch() const
{
    return (static_cast<int64_t>(secondsSinceEpoch()) * MICROSECONDS_PER_SECOND) + _microseconds;
}

tm DateTime::getTm(Precision precision) const
{
    tm tm_;
    memset(&tm_, 0, sizeof(tm_));
    tm_.tm_mday = 1;

    if (precision >= Precision::Year)
    {
        tm_.tm_year = _tm.tm_year;
        if (precision >= Precision::Month)
        {
            tm_.tm_mon = _tm.tm_mon;
            if (precision >= Precision::Day)
            {
                tm_.tm_mday = _tm.tm_mday;
                if (precision >= Precision::Hour)
                {
                    tm_.tm_hour = _tm.tm_hour;
                    if (precision >= Precision::Minute)
                    {
                        tm_.tm_min = _tm.tm_min;
                        if (precision >= Precision::Second)
                        {
                            tm_.tm_sec = _tm.tm_sec;
                        }
                    }
                }
            }
        }
    }

    return tm_;
}

int64_t DateTime::secondsSince1970(Precision precision) const
{
    tm tm_(getTm(precision));
    return static_cast<int64_t>(mktime(&tm_));
}

int64_t DateTime::microsecondsSince1970(Precision precision) const
{
    return static_cast<int64_t>(secondsSince1970(precision)) * MICROSECONDS_PER_SECOND +
           (precision >= Precision::Microsecond ? _microseconds : 0);
}

int64_t DateTime::secondsSinceEpoch(Precision precision) const
{
    tm tm_(getTm(precision));
    return static_cast<int64_t>(mktime(&tm_)) + SecondsFromEpochTo1970;
}

int64_t DateTime::microsecondsSinceEpoch(Precision precision) const
{
    return static_cast<int64_t>(secondsSinceEpoch(precision)) * MICROSECONDS_PER_SECOND +
           (precision >= Precision::Microsecond ? _microseconds : 0);
}

bool DateTime::addDays(int days)
{
    const int64_t seconds = secondsSince1970() + (days * SECONDS_PER_DAY);
    const bool success = localtime_s(&_tm, &seconds) == 0;
    if (!success)
    {
        clear();
    }
    return success;
}

ostream& DateTime::operator<<(ostream& stream) const
{
    return stream << put_time(&_tm, DefaultFormat);
}

ostream& operator<<(ostream& stream, const DateTime& dateTime)
{
    return dateTime.operator<<(stream);
}

string DateTime::toString(const char* pFormat) const
{
    string result;

    if (pFormat && *pFormat)
    {
        stringstream ss;
        ss << put_time(&_tm, pFormat);
        result = ss.str();
    }

    return result;
}

string DateTime::toString() const
{
    return toString(DefaultFormat);
}

string DateTime::toString(uint32_t places) const
{
    stringstream ss;
    toString(ss, places);
    return ss.str();
}

ostream& DateTime::toString(ostream& stream, uint32_t places) const
{
    stream << put_time(&_tm, "%F %R:");
    secondsToString(stream, places);
    return stream;
}

ostream& DateTime::secondsToString(ostream& stream, uint32_t places) const
{
    if (places)
    {
        constexpr uint32_t extraWidth = 3;
        constexpr uint32_t maxPlaces = MaxPlaces;

        if (places > maxPlaces)
        {
            places = maxPlaces;
        }

        const double seconds = static_cast<double>(_tm.tm_sec) + (static_cast<double>(_microseconds) / MICROSECONDS_PER_SECOND);
        stream << fixed << setfill('0') << setw(places + extraWidth) << setprecision(places) << seconds;
    }
    else
    {
        const uint32_t seconds = static_cast<uint32_t>(_tm.tm_sec);
        stream << fixed << setfill('0') << setw(2) << seconds;
    }

    return stream;
}

string& DateTime::fastAppend(string& str, bool useFourDigitYear, bool useMilliseconds) const
{
    str.reserve(str.size() + FastAppendMaxSize);

    Convert::AppendFixedFast(str, static_cast<uint64_t>(year()), useFourDigitYear ? 4 : 2); //-V112
    str += ASCII::Dash;
    Convert::AppendFixedFast(str, static_cast<uint64_t>(month() + 1), 2);
    str += ASCII::Dash;
    Convert::AppendFixedFast(str, static_cast<uint64_t>(day()), 2);
    str += ASCII::Space;
    Convert::AppendFixedFast(str, static_cast<uint64_t>(hour()), 2);
    str += ASCII::Colon;
    Convert::AppendFixedFast(str, static_cast<uint64_t>(minute()), 2);
    str += ASCII::Colon;
    Convert::AppendFixedFast(str, static_cast<uint64_t>(second()), 2);
    if (useMilliseconds)
    {
        str += ASCII::Period;
        Convert::AppendFixedFast(str, static_cast<uint64_t>(milliseconds()), 3);
    }

    return str;
}

string DateTime::fastToString(bool useFourDigitYear, bool useMilliseconds) const
{
    string str;
    fastAppend(str, useFourDigitYear, useMilliseconds);
    return str;
}

string& DateTime::fastAppend(string& str, Precision precision, bool useFourDigitYear) const
{
    str.reserve(str.size() + FastAppendMaxSize);

    Convert::AppendFixedFast(str, static_cast<uint64_t>(year()), useFourDigitYear ? 4 : 2); //-V112
    if (precision >= Precision::Month)
    {
        str += ASCII::Dash;
        Convert::AppendFixedFast(str, static_cast<uint64_t>(month() + 1), 2);
        if (precision >= Precision::Day)
        {
            str += ASCII::Dash;
            Convert::AppendFixedFast(str, static_cast<uint64_t>(day()), 2);
            if (precision >= Precision::Hour)
            {
                str += ASCII::Space;
                Convert::AppendFixedFast(str, static_cast<uint64_t>(hour()), 2);
                if (precision >= Precision::Minute)
                {
                    str += ASCII::Colon;
                    Convert::AppendFixedFast(str, static_cast<uint64_t>(minute()), 2);
                    if (precision >= Precision::Second)
                    {
                        str += ASCII::Colon;
                        Convert::AppendFixedFast(str, static_cast<uint64_t>(second()), 2);
                        if (precision >= Precision::Microsecond)
                        {
                            str += ASCII::Period;
                            Convert::AppendFixedFast(str, static_cast<uint64_t>(milliseconds()), 3);
                        }
                    }
                }
            }
        }
    }

    return str;
}

string DateTime::fastToString(Precision precision, bool useFourDigitYear) const
{
    string str;
    fastAppend(str, precision, useFourDigitYear);
    return str;
}

void DateTime::clear()
{
    memset(&_tm, 0, sizeof(_tm));
    _tm.tm_year = 70;
    _tm.tm_mday = 1;
    _microseconds = 0;
}

bool DateTime::empty() const
{
    const DateTime dt(false);
    return *this == dt;
}

bool DateTime::reset(const char* pStr, Precision precision, const char** pEnd)
{
    bool success = false;

    clear();

    if (pStr)
    {
        success = true;

        _tm.tm_year = Convert::Quick4(pStr) - 1900;
        if (precision >= Precision::Month)
        {
            if (*pStr != ASCII::Dash)
            {
                success = false;
            }
            else
            {
                _tm.tm_mon = Convert::Quick2(++pStr) - 1;
                if (precision >= Precision::Day)
                {
                    if (*pStr != ASCII::Dash)
                    {
                        success = false;
                    }
                    else
                    {
                        _tm.tm_mday = Convert::Quick2(++pStr);
                        if (precision >= Precision::Hour)
                        {
                            if (*pStr != ASCII::Space)
                            {
                                success = false;
                            }
                            else
                            {
                                _tm.tm_hour = Convert::Quick2(++pStr);
                                if (precision >= Precision::Minute)
                                {
                                    if (*pStr != ASCII::Colon)
                                    {
                                        success = false;
                                    }
                                    else
                                    {
                                        _tm.tm_min = Convert::Quick2(++pStr);
                                        if (precision >= Precision::Second)
                                        {
                                            if (*pStr != ASCII::Colon)
                                            {
                                                success = false;
                                            }
                                            else
                                            {
                                                _tm.tm_sec = Convert::Quick2(++pStr);
                                                if (precision == Precision::Microsecond)
                                                {
                                                    if (!(*pStr == ASCII::Period || *pStr == ASCII::Comma))
                                                    {
                                                        success = false;
                                                    }
                                                    else
                                                    {
                                                        int maxDigits = 6;

                                                        while (maxDigits-- > 0 && ASCII::IsDigit(*++pStr))
                                                        {
                                                            _microseconds *= BASE_TEN;
                                                            _microseconds += static_cast<uint32_t>(ASCII::DigitToVal(*pStr));
                                                        }

                                                        while (maxDigits-- > 0)
                                                        {
                                                            _microseconds *= BASE_TEN;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (success)
        {
            success = validate();
        }
    }

    if (pEnd)
    {
        *pEnd = pStr;
    }

    return success;
}

int DateTime::GetDaysInMonth(int year, int month)
{
    int days = 31;

    if (month >= 0)
    {
        month %= 12;

        if (month == Feb &&
            (year % 100 != 0 ||
            (year % 400 == 0 && year % 4 == 0))) //-V112
        {
            days = 29;
        }
        else
        {
            days = daysInMonth[month];
        }
    }

    return days;
}

int DateTime::getDaysInMonth() const
{
    return GetDaysInMonth(_tm.tm_year + 1900, _tm.tm_mon);
}

bool DateTime::validate(bool allowLeapSeconds) const
{
    bool success = false;

    if (_tm.tm_year >= 0 &&
        _tm.tm_mon  >= 0 && _tm.tm_mon  <= 11 &&
        _tm.tm_mday >= 1 && _tm.tm_mday <= 31 &&
        _tm.tm_hour >= 0 && _tm.tm_hour <= 23 &&
        _tm.tm_min  >= 0 && _tm.tm_min  <= 59 &&
        _tm.tm_sec  >= 0 && _tm.tm_sec  <= (allowLeapSeconds ? 60 : 59) &&
        _microseconds < MICROSECONDS_PER_SECOND)
    {
        success = _tm.tm_mday <= getDaysInMonth();
    }

    return success;
}

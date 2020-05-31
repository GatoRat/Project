// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <ctime>
#include <locale>
#include <ostream>
#include "Defines.Time.h"
#include "TimeStamp.h"

/** A date time class.
    This was created due to design issues with the chrono class which makes
    cross-platform development complicated when actual dates are needed.
    @remarks This assumes time_t represents the seconds elapsed since 1970-01-01 00:00:00
        This class assumes that all times using it are in the same time zone and are more
        or less current and valid. In other words, using this for dates before 1970-01-02
        is problematic.

        For example, when getting seconds from epoch, mktime on Windows first converts tm
        to UTC and then calculates the seconds. mktime on Linux keeps it in local time.
 */
class DateTime
{
public:
    static constexpr uint16_t Epoch = 1900;
    static constexpr uint64_t SecondsFromEpochTo1970        = 2'208'988'800;
    static constexpr uint64_t MicrosecondsFromEpochTo1970_A = 11644473600000000ull;
    static constexpr uint64_t MicrosecondsFromEpochTo1970   = SecondsFromEpochTo1970 * MICROSECONDS_PER_SECOND;
    static constexpr uint32_t MaxPlaces = 6;
    static constexpr size_t   FastAppendMaxSize = 23; // YYYY-MM-DD HH:MM:SS.mmm
    static const char* DefaultFormat;

    enum Month { Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };
    static constexpr int daysInMonth[]{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    enum class Precision
    {
        Year,
        Month,
        Day,
        Hour,
        Minute,
        Second,
        Microsecond
    };

    static int GetDaysInMonth(int year, int month);

protected:
    tm       _tm;
    uint32_t _microseconds = 0;

public:
    /** DateTime constructor.
        @param setToNow Set time to the current time.
    */
    explicit DateTime(bool setToNow = true);

    /** DateTime constructor.
        @param tm_ The time as tm.
        @param microseconds The microseconds.
    */
    DateTime(const tm& tm_, uint32_t microseconds);

    /** DateTime constructor.
        @param microsecondsSinceEpoch since epoch.
    */
    explicit DateTime(int64_t microsecondsSinceEpoch);

    /** DateTime constructor.
        @param year
        @param month
        @param day The day of the month in the range 1 through 31.
        @param hour
        @param minute
        @param second
        @param microseconds
        @remarks This does NO error checking.
    */
    DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0, uint32_t microseconds = 0);

    /** DateTime constructor.
        @param pStr The time as a string in the format: (YYYY-MM-DD HH:MM:SS.mmmmmm).
        @param precision The precision at which to end parsing.
        @param pEnd An optional pointer to what terminated the parsing.
    */
    explicit DateTime(const char* pStr, Precision precision = Precision::Microsecond, const char** pEnd = nullptr);

    /** DateTime copy constructor.
    */
    DateTime(const DateTime&) = default;

    /** DateTime assignment operator.
    */
    DateTime& operator=(const DateTime&) = default;

    /** DateTime destructor.
    */
    ~DateTime() = default;

    /** Resets this object to 1900-01-01 00:00:00.000000.
    */
    void clear();

    /** Indicates if the date is 1 Jan 1970 at midnight.
    */
    bool empty() const;

    /** Sets this object to the current time.
    */
    void setToCurTime();

    /** Resets this object to the current time.
    */
    void reset();

    /** Resets this object to the passed time.
        @param tm_ The time as tm.
        @param microseconds The microseconds.
    */
    void reset(const tm& tm_, uint32_t microseconds);

    /** Resets this object to the passed time.
        @param microsecondsSinceEpoch The microseconds since the epoch.
    */
    void reset(int64_t microsecondsSinceEpoch);

    /** Resets this object to the passed time.
        @param pStr The time as a string in the format: (YYYY-MM-DD HH:MM:SS.mmmmmm).
        @param secondsRequired If \b true, the seconds field must be present.
        @param pEnd An optional pointer to what terminated the parsing.
        @returns \b true if parsing was successful, else \b false.
        @remarks On failure, see remarks for ParseData::Basic.
    */
    bool reset(const char* pStr, bool secondsRequired = true, const char** pEnd = nullptr);

    /** Resets this object to the passed time.
        @param str The time as a string in the format: (YYYY-MM-DD HH:MM[:SS]).
        @param secondsRequired If \b true, the seconds field must be present.
        @returns \b true if parsing was successful, else \b false.
        @remarks On failure, see remarks for ParseData::Basic.
    */
    bool reset(const std::string& str, bool secondsRequired = true);

    /** Resets this object to the passed time.
        @param pStr The time as a string in the format: (YYYY-MM-DD HH:MM:SS.mmmmmm).
        @param precision The precision at which to end parsing.
        @param pEnd An optional pointer to what terminated the parsing.
        @returns \b true if parsing was successful, else \b false.
    */
    bool reset(const char* pStr, Precision precision, const char** pEnd = nullptr);

    /** Sets date and time
        @param year
        @param month
        @param day The day of the month in the range 1 through 31.
        @param hour
        @param minute
        @param second
        @param microseconds
        @remarks This does NO error checking.
    */
    void reset(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0, uint32_t microseconds = 0);

    /** Resets time
        @param hour
        @param minute
        @param second
        @param microseconds
        @remarks This does NO error checking.
    */
    void resetTime(uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0, uint32_t microseconds = 0);

    /** Resets this object to the passed FILETIME.
        @param pFileTime The time as FILETIME. (Contains a 64-bit value representing
            the number of 100-nanosecond intervals since January 1, 1601 (UTC).)
            The FILETIME MUST have been resolved to a valid 64-bit value.
    */
    void resetUsingFILETIME(const void* pFileTime);

    /** Validates the date/time
        @param allowLeapSeconds If \b true, seconds can go to 60, else they are limited to 59.
        @returns \b true if validated, else \b false.
    */
    bool validate(bool allowLeapSeconds = false) const;

    /** Compares only the date/time.
        @param other The DateTime to compare.
        @param precision The precision to use.
        @returns zero (0) if the times are the same, a negative number if this time is
            less than other, else a positive number.
        @remarks This uses microsecondsSince1970.
    */
    int compare(const DateTime& other, Precision precision = Precision::Microsecond) const;

    /** Compares only the date/time.
        @param other The tm to compare.
        @param precision The precision to use.
        @returns zero (0) if the times are the same, a negative number if this time is
            less than other, else a positive number.
        @remarks This uses microsecondsSince1970.
    */
    int compare(const tm& other, Precision precision = Precision::Microsecond) const;

    /** Equality operator.
        @param other The DateTime to compare.
        @returns \b true if the times are exactly the same, else \b false.
        @remarks This comparison includes the ancilary fields, such as
            tm_wday (days since sunday). To compare the date time only,
            use the compare function.
    */
    bool isSame(const DateTime& other) const;

    /** Equality operator.
        @param other The DateTime to compare.
        @returns \b true if the times are exactly the same, else \b false.
        @remarks This operator includes the ancilary fields, such as
            tm_wday (days since sunday). To compare the date time only,
            use the compare function.
    */
    bool operator==(const DateTime& other) const;

    /** Less than operator.
        @param rhs The DateTime to compare.
        @returns \b true if this is less than rhs, else \b false.
        @remarks This operator DOES NOT includes the ancilary fields, such as
            tm_wday (days since sunday).
    */
    bool operator<(const DateTime& rhs) const;

    /** Less than or equal to operator.
        @param rhs The DateTime to compare.
        @returns \b true if this is less than, or equal to, rhs, else \b false.
        @remarks This operator DOES NOT includes the ancilary fields, such as
            tm_wday (days since sunday).
    */
    bool operator<=(const DateTime& rhs) const;

    /** Greater than operator.
        @param rhs The DateTime to compare.
        @returns \b true if this is greater than rhs, else \b false.
        @remarks This operator DOES NOT includes the ancilary fields, such as
            tm_wday (days since sunday).
    */
    bool operator>(const DateTime& rhs) const;

    /** Greater than or equal to operator.
        @param rhs The DateTime to compare.
        @returns \b true if this is greater than, or equal to, rhs, else \b false.
        @remarks This operator DOES NOT includes the ancilary fields, such as
            tm_wday (days since sunday).
    */
    bool operator>=(const DateTime& rhs) const;

    /** Returns a pointer to the internal tm structure.
        @returns A pointer to the internal tm structure.
    */
    operator const tm*() const              { return &_tm; }

    /** Returns a tm structure with the precision requested.
        \precision The precision to use.
        @returns A tm structure with the precision requested.
    */
    tm getTm(Precision precision) const;

    /** Convert to TimeStamp,
        @returns A TimeStamp.
     */
    operator TimeStamp() const;

    /** Convert to TimeStamp,
        \includeMilliseconds Include milliseconds in the conversion.
        @returns A TimeStamp.
    */
    TimeStamp toTimeStamp(bool includeMilliseconds = true) const;

    /** Convert to TimeStamp,
        \precision The precision to use.
        @returns A TimeStamp.
    */
    TimeStamp toTimeStamp(Precision precision) const;

    /** Returns the year.
        @returns The year.
    */
    uint16_t year() const                   { return static_cast<uint16_t>(_tm.tm_year + Epoch); }

    /** Returns the month.
        @returns The month where zero (0) is January and 11 is December.
    */
    uint8_t month() const                   { return static_cast<uint8_t>(_tm.tm_mon); }

    /** Returns the day of the month.
        @returns The day of the month in the range 1 through 31.
    */
    uint8_t day() const                     { return static_cast<uint8_t>(_tm.tm_mday); }

    /** Returns the hour of the day.
        @returns The number of hours since midnight in the range 0 through 23.
    */
    uint8_t hour() const                    { return static_cast<uint8_t>(_tm.tm_hour); }

    /** Returns the minute of the hour.
        @returns The minute in the range 0 through 59.
    */
    uint8_t minute() const                  { return static_cast<uint8_t>(_tm.tm_min); }

    /** Returns the second of the minute.
        @returns The second in the range 0 through 59.
    */
    uint8_t second() const                  { return static_cast<uint8_t>(_tm.tm_sec); }

    /** Returns the tenth of the second.
        @returns The tenth of the second in the range 0 through 9.
    */
    uint8_t deciseconds() const             { return static_cast<uint8_t>(_microseconds / MICROSECONDS_PER_DECISECOND);  }

    /** Returns the millisecond of the second.
        @returns The millisecond in the range 0 through 999.
    */
    uint16_t milliseconds() const           { return static_cast<uint16_t>(_microseconds / MICROSECONDS_PER_MILLISECOND); }

    /** Returns the microsecond of the second.
        @returns The microsecond in the range 0 through 999999.
    */
    uint32_t microseconds() const           { return _microseconds; }

    /** Returns the second of the day.
        @returns The number of seconds since midnight in the range 0 through 86,399.
    */
    uint32_t secondOfDay() const;

    /** Returns the day of the week.
        @returns The day of the week where 0 is Sunday and 6 is Saturday.
    */
    uint8_t dayOfWeek() const               { return static_cast<uint8_t>(_tm.tm_wday); }

    /** Returns the day of the year.
        @returns The day of the year in the range 0 through 365.
    */
    uint16_t dayOfYear() const              { return static_cast<uint16_t>(_tm.tm_yday); }

    /** Indicates whether daylight savings time is in effect.
        @returns \b true if daylight savings time is in effect, else \b false.
    */
    bool isDst() const                      { return _tm.tm_isdst != 0; }

    int getDaysInMonth() const;

    /** Returns the number of seconds since 1970.
        @returns The number of seconds since 1970.
    */
    int64_t secondsSince1970() const;

    /** Returns the number of microseconds since 1970.
        @returns The number of microseconds since 1970.
    */
    int64_t microsecondsSince1970() const;

    /** Returns the number of seconds since the epoch.
        @returns The number of seconds since the epoch.
    */
    int64_t secondsSinceEpoch() const;

    /** Returns the number of microseconds since the epoch.
        @returns The number of microseconds since the epoch.
    */
    int64_t microsecondsSinceEpoch() const;

    /** Returns the number of seconds since 1970.
        @param precision The precision.
        @returns The number of seconds since 1970.
    */
    int64_t secondsSince1970(Precision precision) const;

    /** Returns the number of microseconds since 1970.
        @param precision The precision.
        @returns The number of microseconds since 1970.
    */
    int64_t microsecondsSince1970(Precision precision) const;

    /** Returns the number of seconds since the epoch.
        @param precision The precision.
        @returns The number of seconds since the epoch.
    */
    int64_t secondsSinceEpoch(Precision precision) const;

    /** Returns the number of microseconds since the epoch.
        @param precision The precision.
        @returns The number of microseconds since the epoch.
    */
    int64_t microsecondsSinceEpoch(Precision precision) const;

    /** Add days to date.
        @param days Number of days to add or subtract.
        @returns \b true on success, else \b false. On failure, clear() will
            have been called.
     */
    bool addDays(int days);

    /** Stream insertion operator.
        @param stream The stream.
        @returns A reference to the passed stream.
        @remarks Formats time as "%F %T", which is: YYYY-MM-DD HH:MM:SS.
            This uses the locale of the stream.
    */
    std::ostream& operator<<(std::ostream& stream) const;

    /** Write date/time to string.
        @param pFormat The format to use. See std::put_time. If nullptr, nothing will
            be written.
        @returns The resulting string.
        @remarks This uses the global locale.
    */
    std::string toString(const char* pFormat) const;

    /** Write date/time to string using default format ("%F %T").
        @returns The resulting string.
    */
    std::string toString() const;

    /** Write date/time/microseconds to string using modifed ISO 8601 format.
        @param places The number of decimal places for microseconds.
        @returns The resulting string.
        @remarks This uses the global locale for decimal point.
            This uses the put_time format "%F %R:" and then calls secondsToString.
    */
    std::string toString(uint32_t places) const;

    /** Write date/time/microseconds to string using modified ISO 8601 format.
        @param stream The stream to which to write.
        @param places The number of decimal places for sub-seconds. If greater
            than MaxPlaces, it will limited toMaxPlaces.
        @returns The stream.
        @remarks This uses the global locale for decimal point.
    */
    std::ostream& toString(std::ostream& stream, uint32_t places = MaxPlaces) const;

    /** Write seconds and microseconds to stream.
        @param stream The stream to which to write.
        @param places The number of decimal places for sub-seconds. If greater
            than MaxPlaces, it will limited to MaxPlaces.
        @returns The stream.
        @remarks This uses the global locale for decimal point.
    */
    std::ostream& secondsToString(std::ostream& stream, uint32_t places = MaxPlaces) const;

    std::string& fastAppend(std::string& str, bool useFourDigitYear = true, bool useMilliseconds = true) const;
    std::string  fastToString(bool useFourDigitYear = true, bool useMilliseconds = true) const;

    std::string& fastAppend(std::string& str, Precision precision, bool useFourDigitYear = true) const;
    std::string  fastToString(Precision precision, bool useFourDigitYear = true) const;
};

/** DateTime stream insertion operator.
    @param stream The stream.
    @param dateTime The date time.
*/
std::ostream& operator<<(std::ostream& stream, const DateTime& dateTime);

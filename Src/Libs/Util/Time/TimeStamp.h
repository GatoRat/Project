// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <ctime>
#include <string>

#pragma pack(push, 1)
union TimeStamp
{
    static constexpr int      BaseYear_tm     = 1900;
    static constexpr uint16_t BaseYear        = 2000;
    static constexpr uint64_t MillisecondsFrom1970ToBase = 946'684'800'000;

    static constexpr uint8_t  MinMonth        =   1;
    static constexpr uint8_t  MaxMonth        =  12;
    static constexpr uint8_t  MinDay          =   1;
    static constexpr uint8_t  MaxDay          =  31;
    static constexpr uint8_t  MaxHour         =  23;
    static constexpr uint8_t  MaxMinute       =  59;
    static constexpr uint8_t  MaxSeconds      =  59;
    static constexpr uint16_t MaxMilliseconds = 999;

    static constexpr uint8_t  February        =   2;

    static constexpr size_t   MaxStrLen       =  24;
    static constexpr size_t   MaxPackedStrLen =  16;

    static constexpr uint8_t  DaysInMonth[]   = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    struct
    {
        uint16_t msec;  // 0-999
        uint8_t  sec;   // 0-59
        uint8_t  min;   // 0-59
        uint8_t  hour;  // 0-23
        uint8_t  day;   // 1-31
        uint8_t  mon;   // 1-12  -- unlike tm, this is 1 based
        uint8_t  year;  // 0-255 -- unlike tm, this is 2000 based
    } dt;
    uint64_t val = 0;

    /** Converts a year to the internal representation.
        @param year The year. If less than 256, it is assumed to be relative to 2000.
            If in the range 256-1999, dt.year will be set to zero. If greater than 2256,
            dt.year will be set to 255.
        \returns A value relative to 2000.
    */
    static uint8_t  ToInternalYear(uint16_t year);
    static uint16_t ToActualYear(uint8_t year);

    enum class Set
    {
        ToCurTime,
        ToMin,
        ToMax
    };

    TimeStamp() = default;
    explicit TimeStamp(Set set);
    explicit TimeStamp(int64_t val);
    TimeStamp(const char* pStr, bool secondsRequired = true, const char** pEnd = nullptr, bool* pError = nullptr);
    TimeStamp(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint16_t msec = 0);
    TimeStamp(const tm& tm);
    TimeStamp(TimeStamp lhs, TimeStamp rhs);

    void operator=(const tm& tm);

    void set(Set set);

    /** Convert an ISO 8601 or relaxed 8601 date.
        @param pStr A string representing an ISO 8601 or relaxed 8601. The form is:
            YYYY[-MM[-DD[[ |T]HH[:MM[:SS[.mmm]]]]]][Z]. The dashes and colons must
            be used even though the standard allows them to be omitted.
        @param secondsRequired If \b true, seconds must be present.
        @param pEnd An option pointer to where the parsing stopped.
        @remarks ISO 8601 specifies that a 'T' be used as the delimiter between the date
            and time; this function allows a space to be used instead.
        @remarks For ISO 8601, this only handles the Zulu time zone.
        @remarks This is several orders of magnitude faster than most alternate functions.
    */
    bool set(const char* pStr, bool secondsRequired = true, const char** pEnd = nullptr);

    /** Parses a time only stamp or an ISO 8601 or relaxed 8601 date.
        @param pStr A string representing a time, an ISO 8601 or relaxed 8601. The form is:
            [YYYY[-MM[-DD[[ |T]]HH[:MM[:SS[.mmm]]]]]][Z]. The dashes and colons must
            be used even though the standard allows them to be omitted.
        @param secondsRequired If \b true, seconds must be present.
        @param pEnd An option pointer to where the parsing stopped.
        @remarks If the string starts with a 'T', it will parsed as a time.
        @remarks ISO 8601 specifies that a 'T' be used as the delimiter between the date
            and time; this function allows a space to be used instead.
        @remarks For ISO 8601, this only handles the Zulu time zone.
        @remarks This is several orders of magnitude faster than most alternate functions.
    */
    bool set2(const char* pStr, bool secondsRequired = true, const char** pEnd = nullptr);

    /** Parses a time only stamp.
        @param pStr A string representing a time. The form is:
            HH:MM[:SS[.mmm]]]]][Z].
        @param secondsRequired If \b true, seconds must be present.
        @param pEnd An option pointer to where the parsing stopped.
        @remarks On success, the year, month and day will be zero.
        @remarks This is several orders of magnitude faster than most alternate functions.
    */
    bool setFromTime(const char* pStr, bool secondsRequired = true, const char** pEnd = nullptr);

    // setStrict8601() - This may be added to support the strict 8601 format, which uses
    // a T as a date/time delimiter and allows dashes and colons to be omitted. It would not,
    // however, support the "Week" or ordinal date notations.

    /** Sets the internal fields.
        @param year The year. If less than 256, it is assumed to be relative to 2000.
            If in the range 256-1999, dt.year will be set to zero. If greater than 2256,
            dt.year will be set to 255.
        @remarks
    */
    void set(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint16_t msec = 0);
    int  setCurTime();

    bool validate() const;
    bool validateTimeOnly() const;

    bool isTimeOnly() const;

    bool isMin() const { return val == 0; }
    bool isMax() const { return val == UINT64_MAX; }

    bool operator< (const TimeStamp& timeStamp) const     { return val <  timeStamp.val; }
    bool operator<=(const TimeStamp& timeStamp) const     { return val <= timeStamp.val; }

    enum StrStyle : uint32_t
    {
        DontUseSeconds  = 0b000000000,
        UseSeconds      = 0b000000001,
        UseMilliseconds = 0b000000010,
        TimeOnly        = 0b000000100,
        UseTwoDigitYear = 0b000001000,

        UseT            = 0b000100000,
        UseZ            = 0b001000000,
        UseStrict8601   = 0b010000000,
    };

    /** Create a time stamp string.
        @param strStyle The style flags to use.
        @remarks This is over 1000 times faster than strftime.
    */
    std::string toStr(StrStyle strStyle = UseSeconds) const;

    /** Append a time stamp string.
        @param strToAppend The string to append.
        @param strStyle The style flags to use.
        @remarks This is over 1000 times faster than strftime.
    */
    std::string& toStrAppend(std::string& strToAppend, StrStyle strStyle = UseSeconds) const;

    std::string& toStrPackedAppend(std::string& strToAppend, StrStyle strStyle = UseSeconds, char dateTimeSeparator = '_') const;
    std::string toStrPacked(StrStyle strStyle = UseSeconds, char dateTimeSeparator = '_') const;

    uint16_t getYear() const;

    // TODO: Must be tested
    uint64_t GetMillisecondsFrom1970() const;

    static uint64_t GetCurTime ();
    static  int64_t GetCurTimeI();
};

constexpr TimeStamp::StrStyle operator|(TimeStamp::StrStyle a, TimeStamp::StrStyle b)
{
    return static_cast<TimeStamp::StrStyle>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

#pragma pack(pop)

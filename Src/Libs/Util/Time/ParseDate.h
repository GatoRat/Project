// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <ctime>

namespace ParseDate
{
    /** Parse date/time string.
       @param pStr The date/time string.
       @param pTime An optional field for the time.
       @param secondsRequired If \b true, the seconds field must be present.
       @param pEnd Optional pointer; if the format is valid, this will be
            set to the end of the time text. If the format is invalid, this
            will be set to where it was when the format became invalid.
       @returns \b true if parse succeeded, else \b false. On failure and if
            pTime is not nullptr, If the format is correct, but the validation
            fails, tm will contain the parsed values. If the format is
            incorrect, tm will be set to zero.
       @remarks For optimization reasons, if secondsRequired is \b true, the
            actual check for that is done last.

            The resulting date is validated with mktimeSafe set to \b true.
    */
    bool Basic(const char* pStr, tm* pTime = nullptr, bool secondsRequired = true, const char** pEnd = nullptr);

    /** Validate basic tm.
       @param time The time.
       @param allowLeapSeconds If true, seconds can be in the range 0-60.
       @param mktimeSafe If true, the year range is reduced to 71-1098
       @returns \b true if tm is valid, else \b false.
       @remarks This checks if year is in range 70-1099 and that
            the month, day, hour, minute and seconds are within
            their proper ranges.
    */
    bool ValidateBasic(const std::tm& time, bool allowLeapSeconds = false, bool mktimeSafe = false);

} // namespace ParseDate

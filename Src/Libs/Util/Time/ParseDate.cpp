// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ParseDate.h"
#include <cstring>
#include "../String/ASCII.h"
#include "../String/Convert.h"
#include "DateTime.h"

using namespace std;

namespace ParseDate
{
    //static tm highestTime{ 59, 59, 23, 31, 11, 1098, 0, 0,  0 };

    bool Basic(const char* pStr, tm* pTime, bool secondsRequired, const char** pEnd)
    {
        bool success = false;

        tm time;
        if (!pTime)
        {
            pTime = &time;
        }
        memset(pTime, 0, sizeof(tm));

        if (pStr)
        {
            bool hasSeconds = false;

            pTime->tm_year = Convert::Quick4(pStr) - 1900;
            if (*pStr == ASCII::Dash)
            {
                pTime->tm_mon = Convert::Quick2(++pStr) - 1;
                if (*pStr == ASCII::Dash)
                {
                    pTime->tm_mday = Convert::Quick2(++pStr);
                    if (*pStr == ASCII::Space)
                    {
                        pTime->tm_hour = Convert::Quick2(++pStr);
                        if (*pStr == ASCII::Colon)
                        {
                            pTime->tm_min = Convert::Quick2(++pStr);

                            if (*pStr == ASCII::Colon)
                            {
                                pTime->tm_sec = Convert::Quick2(++pStr);
                                hasSeconds = true;
                            }
                            success = true;
                        }
                    }
                }
            }

            if (success)
            {
                success = ValidateBasic(*pTime, false, true);
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

    constexpr bool isYearValid(int year, bool mktimeSafe)
    {
        return mktimeSafe ? (year >  70 && year <  1099) :
                            (year >= 70 && year <= 1099);
    }

    bool ValidateBasic(const tm& time, bool allowLeapSeconds, bool mktimeSafe)
    {
        bool success = false;

        if (isYearValid(time.tm_year, mktimeSafe) &&
            time.tm_mon  >= 0  && time.tm_mon  <=   11 &&
            time.tm_mday >= 1  && time.tm_mday <=   31 &&
            time.tm_hour >= 0  && time.tm_hour <=   23 &&
            time.tm_min  >= 0  && time.tm_min  <=   59 &&
            time.tm_sec  >= 0  && time.tm_sec  <= (allowLeapSeconds ? 60 : 59))
        {
            static int daysInMon[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

            if (   time.tm_mon == 1 &&
                (((time.tm_year + 1900) % 100) != 0 ||
                 ((time.tm_year + 1900) % 400) == 0) &&
                  (time.tm_year % 4) == 0) //-V112
            {
                success = time.tm_mday <= 29;
            }
            else if (time.tm_mday <= daysInMon[time.tm_mon])
            {
                success = true;
            }
        }

        return success;
    }
} // namespace ParseDate

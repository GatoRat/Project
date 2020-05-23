// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SysTime.h"

TimeStamp GetSysLocalTime(int* pDayOfWeek)
{
    return GetSysTime(true, pDayOfWeek);
}

TimeStamp GetSysUtcTime(int* pDayOfWeek)
{
    return GetSysTime(false, pDayOfWeek);
}
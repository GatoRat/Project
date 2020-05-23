// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#ifdef _WIN32

#include "../_lean_windows_.h"
//#include <cstdarg>

void WinReportEventNT(
    LPCTSTR pSource,
    WORD    type,
    DWORD   eventID,
    LPCTSTR pStr,
    DWORD   rawDataLen = 0,
    const void* pRawData = nullptr);

/*
void __cdecl WinReportEventNT(
    LPCTSTR pSource,
    WORD type,
    WORD category,
    DWORD eventID,
    DWORD rawDataLen,
    const void* pRawData,
    WORD numStrings,
    ...);

void __cdecl WinReportEventNTFormat(
    LPCTSTR pSource,
    WORD type,
    WORD category,
    DWORD eventID,
    DWORD rawDataLen,
    const void* pRawData,
    LPCTSTR pTemplate,
    ...);

void __cdecl WinReportEventNTFormatV(
    LPCTSTR pSource,
    WORD type,
    WORD category,
    DWORD eventID,
    DWORD rawDataLen,
    const void* pRawData,
    LPCTSTR pTemplate,
    va_list args);
*/

void WinRegisterEventSource(
    LPCTSTR pSource,
    LPCTSTR pMsgFiles);   // separate multiple files by semicolons

#endif
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "WinReportError.h"
#include <cstdio>
#include <tchar.h>

void WinReportEventNT(
    LPCTSTR pSource,
    WORD    type,
    DWORD   eventID,
    LPCTSTR pStr,
    DWORD   rawDataLen,
    const void* pRawData)
{
    if (pSource && *pSource && pStr && *pStr)
    {
        HANDLE hEventLog = RegisterEventSource(NULL, pSource);
        if (hEventLog)
        {
            LPCTSTR strings[1];
            strings[0] = pStr;
            ReportEvent(hEventLog, type, 0, eventID, nullptr, 1, rawDataLen, strings, const_cast<void*>(pRawData));
            DeregisterEventSource(hEventLog);
        }
    }
}
/*
void __cdecl WinReportEventNT(
    LPCTSTR pSource,
    WORD    type,
    WORD    category,
    DWORD   eventID,
    DWORD   rawDataLen,
    const void* pRawData,
    WORD    numStrings,
    ...)
{
    if (pSource && *pSource)
    {
        HANDLE hEventLog = RegisterEventSource(NULL, pSource);
        if (hEventLog)
        {
            va_list args;
            va_start (args, numStrings);

            LPCTSTR* pStrings = (LPCTSTR*)GlobalAlloc(GMEM_FIXED, numStrings * sizeof(LPCTSTR));

            for (WORD i = 0; i < numStrings; i++)
            {
                pStrings[i] = va_arg(args, LPCTSTR);
            }

            va_end(args);

            ReportEvent(hEventLog, type, category, eventID, nullptr, numStrings, rawDataLen, pStrings, (void*) pRawData);
            GlobalFree(pStrings);
            DeregisterEventSource(hEventLog);
        }
    }
}

void __cdecl WinReportEventNTFormat(
    LPCTSTR pSource,
    WORD    type,
    WORD    category,
    DWORD   eventID,
    DWORD   rawDataLen,
    const void* pRawData,
    LPCTSTR pTemplate,
    ...)
{
    va_list args;
    va_start (args, pTemplate);
    TCHAR buffer[4096];
    _vsntprintf_s(buffer, _TRUNCATE, pTemplate, args);
    WinReportEventNT(pSource, type, category, eventID, rawDataLen, pRawData, 1, buffer);
    va_end(args);
}

void __cdecl WinReportEventNTFormatV(
    LPCTSTR pSource,
    WORD    type,
    WORD    category,
    DWORD   eventID,
    DWORD   rawDataLen,
    const void* pRawData,
    LPCTSTR pTemplate,
    va_list args)
{
    TCHAR buffer[4096];
    _vsntprintf_s(buffer, _TRUNCATE, pTemplate, args);
    WinReportEventNT(pSource, type, category, eventID, rawDataLen, pRawData, 1, buffer);
}
*/

void WinRegisterEventSource(
    LPCTSTR pSource,
    LPCTSTR pMsgFiles)
{
    if (pSource && *pSource && pMsgFiles && *pMsgFiles)
    {
        TCHAR subKey[512] = _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
        _tcscat_s(subKey, pSource);

        HKEY hKey;
        DWORD result;
        if (RegCreateKeyEx(
            HKEY_LOCAL_MACHINE,
            subKey,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_ALL_ACCESS,
            NULL,
            &hKey,
            &result) == ERROR_SUCCESS)
        {
            if (result == REG_CREATED_NEW_KEY)
            {
                RegSetValueEx(hKey, _T("EventMessageFile"), 0, REG_EXPAND_SZ, (CONST BYTE*)pMsgFiles, (DWORD)_tcsclen(pMsgFiles));
                DWORD data = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
                RegSetValueEx(hKey, _T("TypesSupported"), 0, REG_DWORD, (CONST BYTE*)&data, sizeof(DWORD));
            }
            RegCloseKey(hKey);
        }
    }
}

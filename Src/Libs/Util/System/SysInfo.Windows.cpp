// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SysInfo.h"
#include <intrin.h>
#include <stdio.h>
#ifdef __clang__
    #include <cpuid.h>
#endif
#define INCLUDE_SHLOBJ
#include "_lean_windows_.h"
#include <Psapi.h>
#include "../String/FormatSizeFast.h"
#include "../String/constexpr_string.h"

using namespace std;

// Since the processor string won't change, the string is created once and
// returned with each call.
const char* GetProcessorString(bool substituteSpecialChars, bool force)
{
    constexpr size_t MaxProcessorStrLen = 127;
    static char processorStr[MaxProcessorStrLen + 3]{ 0 };

    if (!processorStr[0] || force) //-V560
    {
        constexpr size_t CpuInfoLen = 4; //-V112
#ifdef __clang__
        unsigned cpuInfo[CpuInfoLen]{ 0xffffffff };
#else
        int cpuInfo[CpuInfoLen]{ -1, -1, -1, -1 };
#endif

        constexpr int FunctionId_GetHighestValidExtendedId = 0x80000000; //-V112

#ifdef __clang__
        __cpuid(FunctionId_GetHighestValidExtendedId, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#else
        __cpuid(cpuInfo, FunctionId_GetHighestValidExtendedId);
#endif
        const unsigned int numExIds = cpuInfo[0];

        constexpr size_t MaxCpuBrandStrLen = sizeof(cpuInfo) * 3;

        char cpuBrandString[MaxCpuBrandStrLen]{ 0 };
        for (unsigned i = static_cast<unsigned>(FunctionId_GetHighestValidExtendedId); i <= numExIds; ++i)
        {
#ifdef __clang__
            __cpuid(i, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#else
            __cpuid(cpuInfo, i);
#endif

            constexpr unsigned FunctionId_BrandStringChunk1 = 0x80000002;
            constexpr unsigned FunctionId_BrandStringChunk2 = 0x80000003;
            constexpr unsigned FunctionId_BrandStringChunk3 = 0x80000004;

            constexpr unsigned BrandStringChunk2_WriteOffset = 16;
            constexpr unsigned BrandStringChunk3_WriteOffset = 32; //-V112

            // Interpret CPU brand string and cache information.
            if (i == FunctionId_BrandStringChunk1)
            {
                memcpy(cpuBrandString, cpuInfo, sizeof(cpuInfo));
            }
            else if (i == FunctionId_BrandStringChunk2)
            {
                memcpy(cpuBrandString + BrandStringChunk2_WriteOffset, cpuInfo, sizeof(cpuInfo));
            }
            else if (i == FunctionId_BrandStringChunk3)
            {
                memcpy(cpuBrandString + BrandStringChunk3_WriteOffset, cpuInfo, sizeof(cpuInfo));
            }
        }

        const char* pSrc = cpuBrandString;
        while (*pSrc > 0 && *pSrc <= 32)
        {
            pSrc++;
        }

        size_t processorStrLen = 0;
        while (processorStrLen < MaxProcessorStrLen && *pSrc)
        {
            constexpr_string<char> RegisteredTrademark("(R)");
            constexpr uint8_t RegisteredTrademark_UTF8[]{ 0xC2, 0xAE };

            constexpr_string<char> Copyright("(C)");
            constexpr uint8_t Copyright_UTF8[]{ 0xC2, 0xA9 };

            constexpr_string<char> Trademark("(TM)");
            constexpr uint8_t Trademark_UTF8[]{ 0xE2, 0x84, 0xA2 };

            constexpr_string<char> CPU("CPU ");

            if (RegisteredTrademark == pSrc)
            {
                pSrc += RegisteredTrademark.size();

                if (substituteSpecialChars)
                {
                    processorStr[processorStrLen++] = static_cast<char>(RegisteredTrademark_UTF8[0]);
                    processorStr[processorStrLen++] = static_cast<char>(RegisteredTrademark_UTF8[1]);
                }
            }
            else if (Copyright == pSrc)
            {
                pSrc += Copyright.size();

                if (substituteSpecialChars)
                {
                    processorStr[processorStrLen++] = static_cast<char>(Copyright_UTF8[0]);
                    processorStr[processorStrLen++] = static_cast<char>(Copyright_UTF8[1]);
                }
            }
            else if (Trademark == pSrc)
            {
                pSrc += Trademark.size();

                if (substituteSpecialChars)
                {
                    processorStr[processorStrLen++] = static_cast<char>(Trademark_UTF8[0]);
                    processorStr[processorStrLen++] = static_cast<char>(Trademark_UTF8[1]);
                    processorStr[processorStrLen++] = static_cast<char>(Trademark_UTF8[2]); //-V557
                }
            }
            else if (CPU == pSrc)
            {
                pSrc += CPU.size();
            }
            else if (*pSrc > 0 && *pSrc <= 32)
            {
                while (*pSrc > 0 && *pSrc <= 32)
                {
                    pSrc++;
                }

                processorStr[processorStrLen++] = ' ';
            }
            else
            {
                processorStr[processorStrLen++] = *pSrc++;
            }
        }

        processorStr[processorStrLen] = 0;
    }

    return processorStr;
}

uint64_t GetSystemMemory(uint64_t* pAvailableMemory)
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    GlobalMemoryStatusEx(&memInfo);

    if (pAvailableMemory)
    {
        *pAvailableMemory = memInfo.ullAvailPhys;
    }

    return memInfo.ullTotalPhys;
}

string GetMemoryString()
{
    uint64_t availableMemory;
    const uint64_t systemMemory = GetSystemMemory(&availableMemory);

    string memoryString(FormatMemorySizeFast(systemMemory));

    memoryString += " (";
    memoryString += FormatMemorySizeFast(availableMemory);
    memoryString += ')';

    return memoryString;
}

ProcessMemory::ProcessMemory()
{
    reset();
}

void ProcessMemory::reset()
{
    PROCESS_MEMORY_COUNTERS memoryCounters;
    memset(&memoryCounters, 0, sizeof(memoryCounters));
    memoryCounters.cb = sizeof(memoryCounters);

    HANDLE hProcess = GetCurrentProcess();
    if (GetProcessMemoryInfo(hProcess, &memoryCounters, sizeof(memoryCounters)))
    {
        _workingSetSize     = static_cast<size_t>  (memoryCounters.WorkingSetSize    );
        _peakWorkingSetSize = static_cast<size_t>  (memoryCounters.PeakWorkingSetSize);
        _pagefileUsage      = static_cast<size_t>  (memoryCounters.PagefileUsage     );
        _peakPagefileUsage  = static_cast<size_t>  (memoryCounters.PeakPagefileUsage );
        _pageFaultCount     = static_cast<uint32_t>(memoryCounters.PageFaultCount    );
    }
    else
    {
        _workingSetSize     = 0;
        _peakWorkingSetSize = 0;
        _pagefileUsage      = 0;
        _peakPagefileUsage  = 0;
        _pageFaultCount     = 0;
    }
}

const char* GetOsString()
{
    constexpr size_t MaxOsStrLen = 127;
    static char osStr[MaxOsStrLen]{ 0 };

    if (!osStr[0]) //-V547
    {
#ifdef _WIN64
        const int osBits = 64;
#else
        int osBits;
        BOOL is64 = FALSE;
        if (IsWow64Process(GetCurrentProcess(), &is64) && is64)
        {
            osBits = 64;
        }
        else
        {
            osBits = 32;
        }
#endif

        OSVERSIONINFOEX verInfo;
        verInfo.dwOSVersionInfoSize = sizeof(verInfo);

        #pragma warning(suppress:4996)
        if (GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&verInfo)))
        {
            _snprintf_s(osStr, _TRUNCATE, "Win %d %u.%u.%u.%u",
                osBits,
                verInfo.dwMajorVersion,
                verInfo.dwMinorVersion,
                verInfo.wServicePackMajor,
                verInfo.wServicePackMinor);
        }
        else
        {
            _snprintf_s(osStr, _TRUNCATE, "Win %d", osBits);
        }
    }

    return osStr;
}

bool IsSSE42Supported()
{
    static bool supportsSSE42 = false;
    static bool checkedSSE42 = false;
    if (!checkedSSE42)
    {
        checkedSSE42 = true;
#ifdef __clang__
        unsigned cpuInfo[4]{ 0xffffffff };
        __cpuid(1, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#else
        int cpuInfo[4]{ -1, -1, -1 ,-1 };
        __cpuid(cpuInfo, 1);
#endif
        supportsSSE42 = (cpuInfo[2] & 0x100000) != 0;
        //supportsSSE42 = qCpuHasFeature(SSE4_2);
        //supportsSSE42 = (qCpuFeatures() & SSE4_2) != 0;
    }
    return supportsSSE42;
}

static constexpr bool IsDriveLetter(wchar_t ch)
{
    return (ch >= L'A' && ch <= L'Z') ||
           (ch >= L'a' && ch <= L'z');
}

static constexpr bool IsColon(wchar_t ch)
{
    return ch == L':';
}

static constexpr bool IsSlash(wchar_t ch)
{
    return ch == L'\\' || ch == L'/';
}

static inline PWSTR MakeRootPathName(PWSTR pPath)
{
    if (pPath)
    {
        if (IsDriveLetter(pPath[0]) && IsColon(pPath[1]) && IsSlash(pPath[2]))
        {
            pPath[3] = 0;
        }
        else if (IsSlash(pPath[0]) && IsSlash(pPath[1]))
        {
            // may be UNC path

            int numSlashes = 0;
            for (size_t offset = 2; pPath[offset];)
            {
                if (IsSlash(pPath[offset++]))
                {
                    if (++numSlashes == 2) //-V127
                    {
                        pPath[offset] = 0;
                        break;
                    }
                }
            }

            if (numSlashes != 2)
            {
                pPath = nullptr;
            }
        }
        else
        {
            pPath = nullptr;
        }
    }
    return pPath;
}

static DWORD GetSystemVolumeSerialNumber()
{
    DWORD serialNumber = 0;

    PWSTR pSystemPath;
    const HRESULT hResult = SHGetKnownFolderPath(FOLDERID_System, KF_FLAG_NOT_PARENT_RELATIVE | KF_FLAG_DEFAULT_PATH, NULL, &pSystemPath);
    if (SUCCEEDED(hResult))
    {
        const PWSTR pRootPathName = MakeRootPathName(pSystemPath);

        const DWORD rval = GetVolumeInformation(pRootPathName, nullptr, 0, &serialNumber, nullptr, nullptr, nullptr, 0);
        if (!rval && pRootPathName)
        {
            GetVolumeInformation(nullptr, nullptr, 0, &serialNumber, nullptr, nullptr, nullptr, 0);
        }

        CoTaskMemFree(pSystemPath);
    }

    return serialNumber;
}

string GetSystemIdentifier()
{
    const DWORD serialNumber = GetSystemVolumeSerialNumber();
    return serialNumber ? to_string(serialNumber) : string();
}

uint32_t GetSystemIdentifier32()
{
    return GetSystemVolumeSerialNumber();
}

/*
#include <setupapi.h>
#pragma comment(lib, "SetupAPI.lib")
//#include <Usbiodef.h>
#include <initguid.h>
#include <ntddvdeo.h>

const char* GetVideoDriverString()
{
    static string videoDriverString;

    HDEVINFO hDevInfoSet = SetupDiGetClassDevs(&GUID_DEVINTERFACE_DISPLAY_ADAPTER, NULL, NULL,
                                               DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    for (int i = 0; ; ++i)
    {
        SP_DEVINFO_DATA devInfo;
        devInfo.cbSize = sizeof(SP_DEVINFO_DATA);
        if (!SetupDiEnumDeviceInfo(hDevInfoSet, i, &devInfo))
        {
            break;
        }

        // Get the first info item for this driver
        SP_DRVINFO_DATA drvInfo;
        drvInfo.cbSize = sizeof(SP_DRVINFO_DATA);
        if (SetupDiEnumDriverInfo(devInfoSet, &devInfo, SPDIT_COMPATDRIVER, 0, &drvInfo))
        {

        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfoSet);
}*/
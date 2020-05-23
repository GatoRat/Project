// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include <cpuid.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include "SysInfo.h"
#include "../misc/crc32c.h"
#include "../misc/_lean_windows_.h"
#include "../string/constexpr_string.h"
#include "../string/FormatSize.h"
#include "../string/PlainStringBuilder.h"
#include "../string/Str.h"

using namespace std;

// This implementation creates the string once and then returns that string
// each call. In the program in which this was first used, the processor
// string was always displayed on a window. Since the processor string won't
// change, I chose to create the string once and just return that with each
// call. I see no reason to change it now.
const char* GetProcessorString()
{
    constexpr size_t MaxProcessorStrLen = 127;
    constexpr size_t MaxProcessorStrSafetyLen = 5; // 4 for safety when adding UTF8 and 1 for terminating zero
    static char processorStr[MaxProcessorStrLen + MaxProcessorStrSafetyLen] = { 0 };

    if (!processorStr[0])
    {
        constexpr size_t CpuInfoLen = 4;
        unsigned cpuInfo[CpuInfoLen] = { 0xffffffff };

        constexpr int FunctionId_GetHighestValidExtendedId = 0x80000000;

        __cpuid(FunctionId_GetHighestValidExtendedId, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
        unsigned int numExIds = cpuInfo[0];

        constexpr size_t MaxCpuBrandStrLen = sizeof(cpuInfo) * 3;
        constexpr size_t MaxCpuBrandStrSafetyLen = 5; // 4 for safety when adding UTF8 and 1 for terminating zero

        char cpuBrandString[MaxCpuBrandStrLen + MaxCpuBrandStrSafetyLen] = { 0 };
        for (unsigned i = (unsigned int) FunctionId_GetHighestValidExtendedId; i <= numExIds; ++i)
        {
            __cpuid(i, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);

            constexpr unsigned FunctionId_BrandStringChunk1 = 0x80000002;
            constexpr unsigned FunctionId_BrandStringChunk2 = 0x80000003;
            constexpr unsigned FunctionId_BrandStringChunk3 = 0x80000004;

            constexpr unsigned BrandStringChunk2_WriteOffset = 16;
            constexpr unsigned BrandStringChunk3_WriteOffset = 32;

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
            pSrc++;

        size_t processorStrLen = 0;
        while (processorStrLen < MaxProcessorStrLen && *pSrc)
        {
            constexpr_string<char> RegisteredTrademark("(R)");
            constexpr uint8_t RegisteredTrademark_UTF8[] = { 0xC2, 0xAE };

            constexpr_string<char> Copyright("(C)");
            constexpr uint8_t Copyright_UTF8[] = { 0xC2, 0xA9 };

            constexpr_string<char> Trademark("(TM)");
            constexpr uint8_t Trademark_UTF8[] = { 0xE2, 0x84, 0xA2 };

            constexpr_string<char> CPU("CPU ");

            if (RegisteredTrademark == pSrc)
            {
                pSrc += RegisteredTrademark.size();

                processorStr[processorStrLen++] = (char) RegisteredTrademark_UTF8[0];
                processorStr[processorStrLen++] = (char) RegisteredTrademark_UTF8[1];
            }
            else if (Copyright == pSrc)
            {
                pSrc += Copyright.size();

                processorStr[processorStrLen++] = (char) Copyright_UTF8[0];
                processorStr[processorStrLen++] = (char) Copyright_UTF8[1];
            }
            else if (Trademark == pSrc)
            {
                pSrc += Trademark.size();

                processorStr[processorStrLen++] = (char) Trademark_UTF8[0];
                processorStr[processorStrLen++] = (char) Trademark_UTF8[1];
                processorStr[processorStrLen++] = (char) Trademark_UTF8[2];
            }
            else if (CPU == pSrc)
            {
                pSrc += CPU.size();
            }
            else if (*pSrc > 0 && *pSrc <= 32)
            {
                while (*pSrc > 0 && *pSrc <= 32)
                    pSrc++;

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
    uint64_t totalRam;
    
    struct sysinfo info;
    if (sysinfo(&info) == 0)
    {
        totalRam = info.totalram * info.mem_unit;
        
        if (pAvailableMemory)
            *pAvailableMemory = info.freeram  * info.mem_unit + info.bufferram  * info.mem_unit + info.freeswap * info.mem_unit;
    }
    else
    {
        totalRam = 0;
        
        if (pAvailableMemory)
            *pAvailableMemory = 0;
    }
    
    return totalRam;
}

string GetOsString(bool /*useShort*/)
{
    string osStr;
    
    utsname name;
    if (uname(&name) == 0)
    {
        osStr += name.sysname;
        osStr += ASCII::Space;
        osStr += name.release;

        //char release[];    /* Operating system release (e.g., "2.6.28") */
        //char version[];    /* Operating system version */
        //char machine[];    /* Hardware identifier */ 
    }
    else
    {
        osStr = "Linux";
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

        unsigned cpuInfo[4] = { 0xffffffff };
        __cpuid(1, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);

        supportsSSE42 = (cpuInfo[2] & 0x100000) != 0;
    }
    return supportsSSE42;
}

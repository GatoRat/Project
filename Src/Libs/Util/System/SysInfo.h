// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

/** Gets processor string.
   @param substituteSpecialChars Substitutes (R), (C) and (TM) with their
        equivalent UTF-8 single-glyphs.
   @param force Forces the string to be rebuilt. Used for testing.
   @returns The processor string. Always returns a valid string, even if empty.
*/
const char* GetProcessorString(bool substituteSpecialChars = false, bool force = false);

/** Gets system memory.
   @param pAvailableMemory If non-null, is set to the available memory.
   @returns The system memory.
*/
uint64_t GetSystemMemory(uint64_t* pAvailableMemory = nullptr);

/** Gets memory string.
   @returns The memory string in the format available memory(system memory).
*/
std::string GetMemoryString();

/** Gets process memory usage.*/
class ProcessMemory
{
public:
    ProcessMemory();
    ~ProcessMemory() = default;

    void reset();

    size_t   getWorkingSetSize    () const { return _workingSetSize    ; }
    size_t   getPeakWorkingSetSize() const { return _peakWorkingSetSize; }
    size_t   getPagefileUsage     () const { return _pagefileUsage     ; }
    size_t   getPeakPagefileUsage () const { return _peakPagefileUsage ; }
    uint32_t getPageFaultCount    () const { return _pageFaultCount    ; }

    size_t   getProcessMemory     () const { return _workingSetSize + _pagefileUsage; }

private:
    size_t   _workingSetSize;
    size_t   _peakWorkingSetSize;
    size_t   _pagefileUsage;
    size_t   _peakPagefileUsage;
    uint32_t _pageFaultCount;
};

/** Gets operating system string.
   @returns The operating system string. Always returns a valid string, even if empty.
*/
const char* GetOsString();

/** Indicates whether SSE 4.2 is supported.
   @returns \b true if SSE 4.2 is supported, else \b false.
*/
bool IsSSE42Supported();

/** Returns a string which identifies this device.
   @returns A string which identifies this device. If empty, an identifier
        could not be obtained.
   @remarks For Windows, this returns the serial number of the system volume.
*/
std::string GetSystemIdentifier();

/** Returns a value which identifies this device.
   @returns A value which identifies this device. If zero, an identifier
        could not be obtained.
   @remarks For Windows, this returns the serial number of the system volume.
*/
uint32_t GetSystemIdentifier32();

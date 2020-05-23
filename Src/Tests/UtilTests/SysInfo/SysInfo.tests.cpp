// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/System/SysInfo.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("SysInfo/GetProcessorString")
{
    SECTION("pass false")
    {
        const char* pProcessorString = GetProcessorString(false, true);
        REQUIRE(pProcessorString != nullptr); //-V521
    }
    SECTION("pass true")
    {
        const char* pProcessorString = GetProcessorString(true, true);
        REQUIRE(pProcessorString != nullptr); //-V521
    }
}

TEST_CASE("SysInfo/GetSystemMemory")
{
    SECTION("pass pointer")
    {
        uint64_t memoryAvailable = 0;
        uint64_t totalMemory = GetSystemMemory(&memoryAvailable);

        REQUIRE(memoryAvailable != 0); //-V521
        REQUIRE(totalMemory != 0); //-V521
    }
    SECTION("pass nullptr")
    {
        uint64_t totalMemory = GetSystemMemory(nullptr);

        REQUIRE(totalMemory != 0); //-V521
    }
}

TEST_CASE("SysInfo/GetMemoryString")
{
    string memoryStr = GetMemoryString();
    REQUIRE(!memoryStr.empty()); //-V521
}

TEST_CASE("SysInfo/GetOsString")
{
    const char* pStr = GetOsString();
    REQUIRE(pStr != nullptr); //-V521
}

TEST_CASE("SysInfo/IsSSE42Supported")
{
    REQUIRE_NOTHROW(IsSSE42Supported());
}
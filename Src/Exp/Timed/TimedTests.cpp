// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "Util/Detect/DetectCompiler.h"
#include "Util/Misc/Defines.Misc.h"
#include "Util/Misc/TimedTest.h"
#include "Util/String/Str.h"

namespace fs = std::filesystem;

using namespace std;

#ifdef _MSC_VER
    #pragma warning(disable : 4748) // /GS can not protect parameters and local variables from local buffer overrun because optimizations are disabled in function
#endif

#ifdef OPTIMIZE_ON
    #undef  OPTIMIZE_ON
    #define OPTIMIZE_ON
    #undef  OPTIMIZE_OFF
    #define OPTIMIZE_OFF
#endif

struct TestAll : public TimedTest
{
    const char* GetName() override { return "All"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
    }
    OPTIMIZE_ON

    bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
    }
    OPTIMIZE_ON
};

struct Test1 : public TimedTest
{
    const char* GetName() override { return "Test1"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
    }
    OPTIMIZE_ON

    bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
    }
    OPTIMIZE_ON
};

struct Test2 : public TimedTest
{
    const char* GetName() override { return "Test2"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
    }
    OPTIMIZE_ON

    bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
    }
    OPTIMIZE_ON
};

struct Test3 : public TimedTest
{
    const char* GetName() override { return "Test3"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
    }
    OPTIMIZE_ON

    virtual bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
    }
    OPTIMIZE_ON
};

struct Test4 : public TimedTest
{
    const char* GetName() override { return "Test4"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
    }
    OPTIMIZE_ON

    bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
    }
    OPTIMIZE_ON
};

static TestAll testAll;
static Test1 test1;
static Test2 test2;
static Test3 test3;
static Test4 test4;

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

#include <map>
#include <set>

struct Data
{
    string type;
    int    numActive  { 0    };
    int    menuOrder  { 0    };
    bool   isPromoOnly{ true };

    explicit Data(const string& type_) : type(type_) {}

    bool operator<(const Data& rhs) const
    {
        return type < rhs.type;
    }
};

map<string, Data> types;
vector<Data> types2;

vector<bool> results1;

vector<string> typeNames;
vector<string> rawNames{ "Spin", "Match", "Punch", "Scratch", "Pick" };

struct TestAll : public TimedTest
{
    const char* GetName() override { return "All"; }

    bool InitTest() override
    {
        //types["Spin"]    = Data();
        //types["Match"]   = Data();
        //types["Punch"]   = Data();
        //types["Scratch"] = Data();
        //types["Pick"]    = Data();

        for (auto& s : rawNames)
        {
            for (int r = 0; r < 8; ++r)
            {
                typeNames.push_back(s);
                results1.push_back(false);
            }
        }

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
        types.clear();
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
        size_t i = 0;
        for (auto& s : typeNames)
        {
            results1[i++] = types.try_emplace(s, Data(s)).second;
        }
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
        types.clear();
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
        size_t i = 0;
        for (auto& s : typeNames)
        {
            results1[i++] = types.try_emplace(s, Data(s)).second;
        }
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
        types.clear();
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
        size_t i = 0;
        for (auto& s : typeNames)
        {
            auto it = find_if(types2.begin(), types2.end(), [&s](const Data& data)->bool
            {
                return data.type == s;
            });

            if (it != types2.end())
            {
                results1[i++] = false;
            }
            else
            {
                results1[i++] = true;
                types2.emplace_back(Data(s));
            }
        }
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
        types.clear();
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
        size_t i = 0;
        for (auto& s : typeNames)
        {
            results1[i++] = types.try_emplace(s, Data(s)).second;
        }
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

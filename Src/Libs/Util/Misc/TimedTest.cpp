// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TimedTest.h"
#include <iomanip>
#include <iostream>
#include <thread>
#include "../String/ASCII.h"
#include "../Time/Stopwatch.h"

#ifdef _DEBUG
    #define OPTIMIZE_OFF
    #define OPTIMIZE_ON
#else
    #define OPTIMIZE_OFF __pragma(optimize("", off))
    #define OPTIMIZE_ON  __pragma(optimize("", on))
#endif

using namespace std;

OPTIMIZE_OFF
struct _EmptyTest : public TimedTest
{
    _EmptyTest()
        : TimedTest(false)
    {
    }

    const char* GetName() override
    {
        return nullptr;
    }

    void Test() override
    {
        this_thread::sleep_for(10ms);
    }
};

static _EmptyTest emptyTest;
OPTIMIZE_ON

/////////////////////////////////////////////////////

static TimedTest* pTimedTests = nullptr;

struct _TimedTestList
{
    void Add(TimedTest* pTest)
    {
        if (pTest)
        {
            if (!pTimedTests)
            {
                pTimedTests = pTest;
            }
            else
            {
                TimedTest* pCurTest = pTimedTests;
                for (;;)
                {
                    if (!pCurTest->pNext)
                    {
                        pCurTest->pNext = pTest;
                        break;
                    }
                    pCurTest = pCurTest->pNext;
                }
            }
        }
    }

    void Run(uint32_t numTests, uint32_t loopsPerTest, bool silent)
    {
        TimedTest* pTest = pTimedTests;
        while (pTest)
        {
            pTest->Run(numTests, loopsPerTest, silent);
            pTest = pTest->pNext;
        }
    }
};

static _TimedTestList timedTestList;

void RunTimedTests(uint32_t numTests, uint32_t loopsPerTest, bool silent)
{
    if (!silent)
    {
        cout << ASCII::LF << "Running " << numTests <<
        " tests with " << loopsPerTest << " loops per test" << endl;
    }

    timedTestList.Run(numTests, loopsPerTest, silent);
}

TimedTest::TimedTest()
{
    timedTestList.Add(this);
}

TimedTest::TimedTest(bool add)
{
    if (add)
    {
        timedTestList.Add(this);
    }
}

TimedTest::~TimedTest()
{
}

static constexpr uint32_t emptyTests = 10;

size_t TimedTest::Run(uint32_t numTests, uint32_t loopsPerTest, bool silent) //-V2506
{
    static bool haveBaseTime = false;
    if (!haveBaseTime)
    {
        haveBaseTime = true;
        emptyTest.Run(emptyTests, 1, true);
    }

    if (!InitTest())
    {
        return 0;
    }

    this_thread::yield();

    Stopwatch stopwatch(emptyTest.GetElapsedSeconds());

    for (uint32_t test = 0; test < numTests; test++)
    {
        if (!PreTest())
        {
            break;
        }

        this_thread::yield();
        {
            stopwatch.start();

            for (uint32_t loop = 0; loop < loopsPerTest; loop++)
            {
                Test();
            }

            stopwatch.end();
        }

        if (!PostTest())
        {
            break;
        }
    }

    DeinitTest();

    stopwatch.calculate();

    if (!silent)
    {
        cout << setw(6) << GetName() << setw(0) << ": " << fixed
            << setprecision(6) << stopwatch.getElapsedSeconds() << endl;
    }

    return stopwatch.getNumTimes();
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>

struct _TimedTestList;

struct TimedTest
{
friend _TimedTestList;
public:
    TimedTest();
    explicit TimedTest(bool add);
    virtual ~TimedTest();

    size_t Run(uint32_t numTests = 1, uint32_t loopsPerTest = 1, bool silent = false);

    double GetElapsedSeconds() const        { return elapsedSeconds; }

protected:
    virtual const char* GetName() = 0;

    virtual bool InitTest()                 { return true; }
    virtual bool PreTest()                  { return true; }
    virtual void Test() = 0;
    virtual bool PostTest()                 { return true; }
    virtual void DeinitTest()               {}

private:
    TimedTest* pNext = nullptr;
    double elapsedSeconds = 0.0;

    TimedTest(const TimedTest&) = delete;
    void operator=(const TimedTest&) = delete;
};

void RunTimedTests(uint32_t numTests = 1, uint32_t loopsPerTest = 1, bool silent = false);

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "WinHandle.h"

typedef unsigned (__stdcall* PWTHREAD_FUNCTION)(void*);

class WinThread : public WinHandle
{
public:
    static constexpr int Terminated{ -2 };
    static constexpr int NotRunning{ -1 };

public:
    WinThread() = default;
    explicit WinThread(int priority, bool startSuspended = false, unsigned stackSize = 0);
    WinThread(PWTHREAD_FUNCTION pThread, void* pParam, int priority = -1, bool startSuspended = false, unsigned stackSize = 0);
    WinThread(const WinThread&) = delete;
    void operator=(const WinThread&) = delete;
    virtual ~WinThread();

    bool Create(int priority = -1, bool startSuspended = false, unsigned stackSize = 0);
    bool Create(PWTHREAD_FUNCTION pThread, void* pParam, int priority = -1, bool startSuspended = false, unsigned stackSize = 0);

    virtual int EndThread(DWORD timeOut = 0, bool forceTerminate = false, DWORD exitCode = 0) const;

    BOOL ForceTerminate(DWORD exitCode = 0) const;

    bool  IsRunning() const;
    DWORD Suspend()	const;
    DWORD Resume() const;
    BOOL  SetPriority(int p) const;
    int	  GetPriority() const;
    DWORD GetExitCode() const;

protected:
    virtual unsigned ThreadMain();

private:
    static unsigned __stdcall _thread(void *pClass);
};

///////////////////////////////////////////////////////////////////////////

class WinThreadTerm : public WinThread
{
public:
    WinThreadTerm();
    explicit WinThreadTerm(int priority, bool startSuspended = false, unsigned stackSize = 0);
    explicit WinThreadTerm(HANDLE hEndThreadEvent, int priority = -1, bool startSuspended = false, unsigned stackSize = 0);
    ~WinThreadTerm() override;

    bool SetEndEventHandle(HANDLE h, bool dupHandle);
    HANDLE GetEndEventHandle() const;

    void SetEndThreadEvent() const;
    bool IsEndThreadEventSet(DWORD timeout = 0) const;
    bool KeepRunning(DWORD timeout = 0) const;

    int EndThread(DWORD timeOut = 0, bool forceTerminate = true, DWORD exitCode = 0) const override;

protected:
    WinEvent _endThreadEvent;
};

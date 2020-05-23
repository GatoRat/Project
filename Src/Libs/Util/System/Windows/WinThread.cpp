// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "WinThread.h"
#include <process.h>

unsigned __stdcall WinThread::_thread(void *pClass)
{
	return reinterpret_cast<WinThread*>(pClass)->ThreadMain();
}

WinThread::WinThread(int priority, bool startSuspended, unsigned stackSize)
{
    Create(priority, startSuspended, stackSize);
}

WinThread::WinThread(PWTHREAD_FUNCTION pThread, void* pParam, int priority, bool startSuspended, unsigned stackSize)
{
    Create(pThread, pParam, priority, startSuspended, stackSize);
}

WinThread::~WinThread()
{
    EndThread(10, true, 0);
}

int WinThread::EndThread(DWORD timeOut, bool forceTerminate, DWORD exitCode) const
{
    int result = 0;

    if (!IsRunning())
    {
        result = NotRunning;
    }
    else if (WinHandle::WaitFor(timeOut) == WAIT_TIMEOUT && forceTerminate)
    {
        ForceTerminate(exitCode);
        result = Terminated;
    }

    return result;
}

BOOL WinThread::ForceTerminate(DWORD exitCode) const
{
    return IsRunning() ? ::TerminateThread(_handle, exitCode) : FALSE;
}

bool WinThread::IsRunning() const
{
    return GetExitCode() == STILL_ACTIVE;
}

DWORD WinThread::Suspend()	const
{
    return IsRunning() ? ::SuspendThread(_handle) : static_cast<DWORD>(-1); //-V720
}

DWORD WinThread::Resume() const
{
    return IsRunning() ? ::ResumeThread(_handle) : static_cast<DWORD>(-1);
}

BOOL WinThread::SetPriority(int p) const
{
    return IsRunning() ? ::SetThreadPriority(_handle, p) : FALSE;
}

int	WinThread::GetPriority() const
{
    return IsRunning() ? ::GetThreadPriority(_handle) : THREAD_PRIORITY_ERROR_RETURN;
}

DWORD WinThread::GetExitCode() const
{
    DWORD code = 0;
    return _handle && ::GetExitCodeThread(_handle, &code) ? code : 0;
}

bool WinThread::Create(int priority, bool startSuspended, unsigned stackSize)
{
	return Create(_thread, this, priority, startSuspended, stackSize);
}

bool WinThread::Create(PWTHREAD_FUNCTION pThread, void* pParam, int priority, bool startSuspended, unsigned stackSize)
{
    bool success = false;

    if (!IsRunning())
    {
        _handle = (HANDLE)_beginthreadex(nullptr, stackSize, pThread, pParam, startSuspended ? CREATE_SUSPENDED : 0, nullptr);
        if (_handle)
        {
            success = true;

            if (priority >= 0)
            {
                SetPriority(priority);
            }
        }
    }

	return true;
}

unsigned WinThread::ThreadMain()
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////

WinThreadTerm::WinThreadTerm()
    : _endThreadEvent(TRUE)
{
}

WinThreadTerm::WinThreadTerm(int priority, bool startSuspended, unsigned stackSize)
    : _endThreadEvent(TRUE)
{
    Create(priority, startSuspended, stackSize);
}

WinThreadTerm::WinThreadTerm(HANDLE hEndThreadEvent, int priority, bool startSuspended, unsigned stackSize)
    : _endThreadEvent(hEndThreadEvent)
{
    Create(priority, startSuspended, stackSize);
}

WinThreadTerm::~WinThreadTerm()
{
    EndThread(10, true, 0);
}

bool WinThreadTerm::SetEndEventHandle(HANDLE h, bool dupHandle)
{
    return _endThreadEvent.SetHandle(h, dupHandle);
}

HANDLE WinThreadTerm::GetEndEventHandle() const
{
    return _endThreadEvent;
}

void WinThreadTerm::SetEndThreadEvent() const
{
    _endThreadEvent.Set();
}

bool WinThreadTerm::IsEndThreadEventSet(DWORD timeout) const
{
    return _endThreadEvent.IsSet(timeout);
}

bool WinThreadTerm::KeepRunning(DWORD timeout) const
{
    return !IsEndThreadEventSet(timeout);
}

int WinThreadTerm::EndThread(DWORD timeOut, bool forceTerminate, DWORD exitCode) const
{
    int result = NotRunning;
    
    if (IsRunning())
    {
        SetEndThreadEvent();

        result = WinThread::EndThread(timeOut, forceTerminate, exitCode);
    }

    return result;
}

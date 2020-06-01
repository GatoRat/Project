// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "WinHandle.h"

///////////////////////////////////////////////////////////////////////////

HANDLE WinDuplicateHandle(HANDLE hOrg, bool inheritable)
{
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hDup;
    return ::DuplicateHandle(hProcess, hOrg, hProcess, &hDup, 0, inheritable, DUPLICATE_SAME_ACCESS) ? hDup : nullptr;
}

///////////////////////////////////////////////////////////////////////////

/*
DWORD WinWaitForMultipleObjects(bool waitAll, DWORD timeOut, DWORD numHandles, ...)
{
    va_list args;
    va_start(args, numHandles);
    unsigned int rval = WWaitForMultipleObjectsV(waitAll, timeOut, numHandles, args);
    va_end(args);
    return rval;
}

DWORD WinWaitForMultipleObjectsV(bool waitAll, DWORD timeOut, DWORD numArgHandles, va_list args)
{
#pragma warning(suppress: 6255)
    HANDLE* pHandles = (HANDLE*) _alloca(sizeof(HANDLE) * numArgHandles);
    DWORD numHandles = 0;
    while (numArgHandles--)
    {
        HANDLE hHandle = va_arg(args, HANDLE);
        if (hHandle)
            pHandles[numHandles++] = hHandle;
    }
    return ::WaitForMultipleObjects(numHandles > MAXIMUM_WAIT_OBJECTS ? MAXIMUM_WAIT_OBJECTS : numHandles, pHandles, waitAll, timeOut);
}
*/

//////////////////////////////////////////////////////////////////

WinHandle::WinHandle(HANDLE hHandle, bool dupHandle)
    : _handle{ dupHandle ? WinDuplicateHandle(hHandle) : hHandle }
{
}

WinHandle::~WinHandle()
{
    Close();
}

BOOL WinHandle::Close()
{
    if (!IsOpen())
    {
        return TRUE;
    }

    HANDLE h = _handle;
    _handle = nullptr;
    return ::CloseHandle(h);
}

bool WinHandle::SetHandle(HANDLE hHandle, bool dupHandle)
{
    Close();
    _handle = dupHandle ? WinDuplicateHandle(hHandle) : hHandle;
    return IsOpen();
}

WinHandle& WinHandle::operator=(HANDLE hHandle)
{
    Close();
    _handle = hHandle;
    return *this;
}

DWORD WinHandle::WaitFor(DWORD timeout) const
{
    return IsOpen() ? ::WaitForSingleObject(_handle, timeout) : WAIT_FAILED;
}

DWORD WinHandle::WaitFor(WinHandle& handle, DWORD timeout, bool waitForAll) const
{
    return WaitForMultiple(timeout, &handle, nullptr, nullptr, waitForAll);
}

DWORD WinHandle::WaitForMultiple(DWORD timeoutMilliseconds, const WinHandle* pHandle1, const WinHandle* pHandle2, const WinHandle* pHandle3, bool waitForAll) const
{
    DWORD result = WAIT_FAILED;

    if (IsOpen())
    {
        DWORD handleCount = 0;
        HANDLE handles[4];

        handles[handleCount++] = _handle;

        if (pHandle1 && pHandle1->IsOpen())
        {
            handles[handleCount++] = *pHandle1;
        }

        if (pHandle2 && pHandle2->IsOpen())
        {
            handles[handleCount++] = *pHandle2;
        }

        if (pHandle3 && pHandle3->IsOpen())
        {
            handles[handleCount++] = *pHandle3;
        }

        if (handleCount > 1)
        {
            result = ::WaitForMultipleObjects(handleCount, handles, waitForAll, timeoutMilliseconds);
        }
        else
        {
            result = ::WaitForSingleObject(_handle, timeoutMilliseconds);
        }
    }

    return result;
}

bool WinHandle::WaitForBool(DWORD timeoutMilliseconds) const
{
    return WaitFor(timeoutMilliseconds) != WAIT_TIMEOUT;
}

//////////////////////////////////////////////////////////////////

WinEvent::WinEvent(HANDLE hHandle, bool dupHandle)
    : WinHandle(hHandle, dupHandle)
{
}

WinEvent::WinEvent(BOOL manualReset, BOOL initState, LPCTSTR pName, PSECURITY_ATTRIBUTES pSA)
{
    Create(manualReset, initState, pName, pSA);
}

bool WinEvent::Create(BOOL manualReset, BOOL initState, LPCTSTR pName, PSECURITY_ATTRIBUTES pSA)
{
    Close();
    _handle = ::CreateEvent(pSA, manualReset, initState, pName);
    return IsOpen();
}

bool WinEvent::Open(LPCTSTR pName, DWORD access, BOOL inheritFlag)
{
    Close();
    _handle = ::OpenEvent(access, inheritFlag, pName);
    return IsOpen();
}

BOOL WinEvent::Pulse() const
{
    return ::PulseEvent(_handle);
}

BOOL WinEvent::Set(bool val) const
{
    return val ? Set() : Reset();
}

BOOL WinEvent::Set() const
{
    return ::SetEvent(_handle);
}

BOOL WinEvent::Reset() const
{
    return ::ResetEvent(_handle);
}

bool WinEvent::IsSet(DWORD timeout) const
{
    return WaitFor(timeout) == WAIT_OBJECT_0;
}

bool WinEvent::IsNotSet(DWORD timeout) const
{
    return WaitFor(timeout) != WAIT_OBJECT_0;
}

const WinEvent& WinEvent::operator=(bool val)
{
    Set(val); return *this;
}

///////////////////////////////////////////////////////////////////////////

WinSemaphore::WinSemaphore(HANDLE hHandle, bool dupHandle)
    : WinHandle(hHandle, dupHandle)
{
}

WinSemaphore::WinSemaphore(LONG maxCount, LONG initCount, LPCTSTR pName, PSECURITY_ATTRIBUTES pSA)
{
    Create(maxCount, initCount, pName, pSA);
}

bool WinSemaphore::Create(LONG maxCount, LONG initCount, LPCTSTR pName, PSECURITY_ATTRIBUTES pSA)
{
    Close();
    _handle = ::CreateSemaphore(pSA, initCount >= 0 && initCount <= maxCount ? initCount : maxCount, maxCount, pName);
    return _handle != nullptr;
}

#ifndef _WIN32_WCE
bool WinSemaphore::Open(LPCTSTR pName, DWORD access, BOOL inheritFlag)
{
    Close();
    _handle = ::OpenSemaphore(access, inheritFlag, pName);
    return _handle != nullptr;
}
#endif

BOOL WinSemaphore::Release(LONG releaseCount, LPLONG pPreviousCount) const
{
    return ::ReleaseSemaphore(_handle, releaseCount, pPreviousCount);
}

void WinSemaphore::releaseAll(long maxCount) //-V126
{
    if (maxCount <= 0)
    {
        while (::ReleaseSemaphore(_handle, 1, nullptr)); //-V2507
    }
    else
    {
        long prevCount = 0; //-V126
        if (::ReleaseSemaphore(_handle, 1, &prevCount))
        {
            ::ReleaseSemaphore(_handle, maxCount - 1 - prevCount, nullptr);
            while (::ReleaseSemaphore(_handle, 1, nullptr)); //-V2507
        }
    }
}

void WinSemaphore::clear() const
{
    while (WaitForSingleObject(_handle, 0) == WAIT_OBJECT_0); //-V2507
}

WinSemaphoreAutoRelease::WinSemaphoreAutoRelease(HANDLE hSemaphore)
    : _hSemaphore{ hSemaphore }
{
}

WinSemaphoreAutoRelease::~WinSemaphoreAutoRelease()
{
    release();
}

BOOL WinSemaphoreAutoRelease::release()
{
    BOOL rval = ::ReleaseSemaphore(_hSemaphore, 1, nullptr);
    _hSemaphore = nullptr;
    return rval;
}

///////////////////////////////////////////////////////////////////////////

WinMutex::WinMutex(HANDLE hHandle, bool dupHandle)
    : WinHandle(hHandle, dupHandle)
{
}

WinMutex::WinMutex(BOOL initialOwner, LPCTSTR pName, PSECURITY_ATTRIBUTES pSA)
{
    Create(initialOwner, pName, pSA);
}

WinMutex::~WinMutex()
{
    Close();
}

BOOL WinMutex::Close()
{
    BOOL success = FALSE;

    if (IsOpen())
    {
        Release();
        success = WinHandle::Close();
    }

    return success;
}

bool WinMutex::Create(BOOL initialOwner, LPCTSTR pName, PSECURITY_ATTRIBUTES pSA)
{
    Close();
    _handle = ::CreateMutex(pSA, initialOwner, pName);
    return _handle != nullptr;
}

int WinMutex::CreateAsOwner(LPCTSTR pName, PSECURITY_ATTRIBUTES pSA)
{
    return Create(TRUE, pName, pSA) ? (GetLastError() == ERROR_ALREADY_EXISTS ? MUTEX_CREATE_NOT_OWNED : MUTEX_CREATE_OWNED) : MUTEX_CREATE_FAILED;
}

#ifndef _WIN32_WCE
bool WinMutex::Open(LPCTSTR pName, DWORD access, BOOL inheritFlag)
{
    Close();
    _handle = ::OpenMutex(access, inheritFlag, pName);
    return _handle != nullptr;
}
#endif
    
BOOL WinMutex::Release() const
{
    return _handle ? ::ReleaseMutex(_handle) : TRUE;
}

WinMutexAutoRelease::WinMutexAutoRelease(HANDLE hMutex)
    : _hMutex{ hMutex }
{
}

WinMutexAutoRelease::~WinMutexAutoRelease()
{
    Release();
}

BOOL WinMutexAutoRelease::Release()
{ 
    HANDLE h = _hMutex;
    _hMutex = nullptr;
    return ::ReleaseMutex(h);
}

///////////////////////////////////////////////////////////////////////////

#ifndef _WIN32_WCE

WinTimerQueue::WinTimerQueue(bool create)
{
    if (create)
    {
        Create();
    }
}

WinTimerQueue::~WinTimerQueue()
{
    Delete();
}

bool WinTimerQueue::Create()
{
    Delete(); 
    _hTimerQueue = ::CreateTimerQueue();
    return _hTimerQueue != nullptr;
}

BOOL WinTimerQueue::Delete()
{
    WinEvent completionEvent(TRUE);
    return Delete(completionEvent);
}

BOOL WinTimerQueue::Delete(HANDLE hCompletionEvent)
{
    BOOL success = TRUE;

    if (_hTimerQueue)
    {
        success = ::DeleteTimerQueueEx(_hTimerQueue, hCompletionEvent);
        _hTimerQueue = nullptr;
    }
    
    return success;
}

HANDLE WinTimerQueue::GetHandle() const
{
    return _hTimerQueue;
}

WinTimerQueue::operator HANDLE() const
{
    return _hTimerQueue;
}

//void CALLBACK WinTimerQueueTimerCallback::TimerCallback(PVOID pThis, BOOLEAN /*timerOrWaitFired*/)
//{
//    ((WinTimerQueueTimerCallback*)pThis)->Callback();
//}

WinTimerQueueTimer::WinTimerQueueTimer(DWORD dueTime, DWORD period, WAITORTIMERCALLBACK callback, PVOID pParameter, HANDLE hTimerQueue, ULONG flags)
    : _hTimer(nullptr)
    , _hTimerQueue(nullptr)
{
    Create(dueTime, period, callback, pParameter, hTimerQueue, flags);
}

WinTimerQueueTimer::~WinTimerQueueTimer()
{
    Delete();
}

BOOL WinTimerQueueTimer::Create(DWORD dueTime, DWORD period, WAITORTIMERCALLBACK callback, PVOID pParameter, HANDLE hTimerQueue, ULONG flags)
{
    Delete();
    _hTimerQueue = hTimerQueue;
    return ::CreateTimerQueueTimer(&_hTimer, hTimerQueue, callback, pParameter, dueTime, period, flags);
}

BOOL WinTimerQueueTimer::Delete()
{
    WinEvent completionEvent(TRUE);
    return Delete(completionEvent);
}

BOOL WinTimerQueueTimer::Delete(HANDLE hCompletionEvent)
{
    BOOL result = FALSE;
    if (_hTimer)
    {
        result = DeleteTimerQueueTimer(_hTimerQueue, _hTimer, hCompletionEvent);
    }

    _hTimer = _hTimerQueue = nullptr;

    return result;
}

HANDLE WinTimerQueueTimer::GetHandle() const
{
    return _hTimer;
}

WinTimerQueueTimer::operator HANDLE() const
{
    return _hTimer;
}

WinTimerQueueTimerCallback::WinTimerQueueTimerCallback(DWORD dueTime, DWORD period, TimerCallback timerCallback, HANDLE hTimerQueue, ULONG flags)
{
    Create(dueTime, period, timerCallback, hTimerQueue, flags);
}

WinTimerQueueTimerCallback::~WinTimerQueueTimerCallback()
{
    Delete();
}

BOOL WinTimerQueueTimerCallback::Create(DWORD dueTime, DWORD period, TimerCallback timerCallback, HANDLE hTimerQueue, ULONG flags)
{
    _timerCallback = timerCallback;
    return WinTimerQueueTimer::Create(dueTime, period, WinTimerCallback, this, hTimerQueue, flags);
}

BOOL WinTimerQueueTimerCallback::Delete()
{
    _timerCallback = nullptr;
    return WinTimerQueueTimer::Delete();
}

BOOL WinTimerQueueTimerCallback::Delete(HANDLE hCompletionEvent)
{
    _timerCallback = nullptr;
    return WinTimerQueueTimer::Delete(hCompletionEvent);
}

void WinTimerQueueTimerCallback::Callback()
{
    if (_timerCallback)
    {
        _timerCallback();
    }
}

void CALLBACK WinTimerQueueTimerCallback::WinTimerCallback(PVOID pThis, BOOLEAN /*timerOrWaitFired*/)
{
    reinterpret_cast<WinTimerQueueTimerCallback*>(pThis)->Callback();
}

#endif // _WIN32_WCE

///////////////////////////////////////////////////////////////////////////

/*
WinChangeNotification(LPCTSTR pPath, BOOL watchSubtree, DWORD notifyFilter)
{
    FindFirst(pPath, watchSubtree, notifyFilter);
}

virtual ~WinChangeNotification()
{
    Close();
}

bool WinChangeNotification::Close()
{
    bool success = true;
    if (IsOpen())
    {
        HANDLE h = _handle;
        _handle = INVALID_HANDLE_VALUE;
        success =  ::FindCloseChangeNotification(h) != 0;
    }
    return success;
}

bool IsOpen() const
{
    return _handle != INVALID_HANDLE_VALUE;
}

HANDLE GetHandle() const
{
    return _handle;
}

operator HANDLE() const
{
    return _handle;
}

bool FindFirst(LPCTSTR pPath, BOOL watchSubtree, DWORD notifyFilter)
{
    Close();
    _handle = ::FindFirstChangeNotification(pPath, watchSubtree, notifyFilter);
    return IsOpen();
}

bool FindNext() const
{
    return ::FindNextChangeNotification(_handle) != FALSE;
}

DWORD WaitFor(DWORD timeOut = INFINITE) const
{
    return IsOpen() ? ::WaitForSingleObject(_handle, timeOut) : WAIT_FAILED;
}
*/

///////////////////////////////////////////////////////////////////////////

static BYTE securityDescriptor[SECURITY_DESCRIPTOR_MIN_LENGTH];
SECURITY_ATTRIBUTES winNullSecurityAttribute;

struct InitSecurityAttribute
{
    InitSecurityAttribute()
    {
        winNullSecurityAttribute.nLength              = sizeof(SECURITY_ATTRIBUTES);
        winNullSecurityAttribute.bInheritHandle       = TRUE;
        winNullSecurityAttribute.lpSecurityDescriptor = &securityDescriptor;

#ifdef _WIN32_WCE
        memset(securityDescriptor, 0, sizeof(securityDescriptor));
#else
        InitializeSecurityDescriptor(&securityDescriptor, SECURITY_DESCRIPTOR_REVISION); //-V530
#pragma warning(suppress: 6248)
        SetSecurityDescriptorDacl(&securityDescriptor, TRUE, /*(PACL)*/ nullptr, FALSE); //-V530
#endif
    }
};

static InitSecurityAttribute sa;

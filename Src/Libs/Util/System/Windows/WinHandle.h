// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "../_lean_windows_.h"
#include <functional>

HANDLE WinDuplicateHandle(HANDLE hOrg, bool inheritable = false);

//////////////////////////////////////////////////////////////////////

#ifndef WAIT_OBJECT_1
#define WAIT_OBJECT_1 (WAIT_OBJECT_0 + 1)
#endif

#ifndef WAIT_OBJECT_2
#define WAIT_OBJECT_2 (WAIT_OBJECT_0 + 2)
#endif

#ifndef WAIT_OBJECT_3
#define WAIT_OBJECT_3 (WAIT_OBJECT_0 + 3)
#endif

#ifndef WAIT_OBJECT_4
#define WAIT_OBJECT_4 (WAIT_OBJECT_0 + 4)
#endif

#ifndef WAIT_OBJECT_5
#define WAIT_OBJECT_5 (WAIT_OBJECT_0 + 5)
#endif

#ifndef WAIT_ABANDONED_1
#define WAIT_ABANDONED_1 (WAIT_ABANDONED_0 + 1)
#endif

#ifndef WAIT_ABANDONED_2
#define WAIT_ABANDONED_2 (WAIT_ABANDONED_0 + 2)
#endif

#ifndef WAIT_ABANDONED_3
#define WAIT_ABANDONED_3 (WAIT_ABANDONED_0 + 3)
#endif

#ifndef WAIT_ABANDONED_4
#define WAIT_ABANDONED_4 (WAIT_ABANDONED_0 + 4)
#endif

#ifndef WAIT_ABANDONED_5
#define WAIT_ABANDONED_5 (WAIT_ABANDONED_0 + 5)
#endif

//DWORD WinWaitForMultipleObjects (bool waitAll, DWORD timeOut, DWORD numHandles, ...);
//DWORD WinWaitForMultipleObjectsV(bool waitAll, DWORD timeOut, DWORD numHandles, va_list args);

//////////////////////////////////////////////////////////////////////

class WinHandle
{
public:
    explicit WinHandle(HANDLE hHandle = nullptr, bool dupHandle = false);
    WinHandle(const WinHandle&) = delete;
    void operator=(const WinHandle&) = delete;
    virtual ~WinHandle();

    virtual BOOL Close();

    HANDLE GetHandle() const                    { return _handle; }
    operator HANDLE() const                     { return _handle; }

    bool IsOpen() const                         { return _handle != nullptr; }

    bool SetHandle(HANDLE hHandle, bool dupHandle = false);
    WinHandle& operator=(HANDLE hHandle);

    DWORD WaitFor(DWORD timeoutMilliseconds = INFINITE) const;
    DWORD WaitFor(WinHandle& handle, DWORD timeoutMilliseconds = INFINITE, bool waitForAll = false) const;
    DWORD WaitForMultiple(DWORD timeoutMilliseconds = INFINITE, WinHandle* pHandle1 = nullptr, WinHandle* pHandle2 = nullptr, WinHandle* pHandle3 = nullptr, bool waitForAll = false) const;

    /**
    */
    bool WaitForBool(DWORD timeoutMilliseconds = INFINITE) const;

public:
    HANDLE _handle;
};

//////////////////////////////////////////////////////////////////////

class WinEvent : public WinHandle
{
public:
    WinEvent(HANDLE hHandle = nullptr, bool dupHandle = false);
    explicit WinEvent(BOOL manualReset, BOOL initState = FALSE, LPCTSTR pName = nullptr, PSECURITY_ATTRIBUTES pSA = nullptr);

    bool Create(BOOL manualReset = FALSE, BOOL initState = FALSE, LPCTSTR pName = nullptr, PSECURITY_ATTRIBUTES pSA = nullptr);
    bool Open(LPCTSTR pName, DWORD access = EVENT_ALL_ACCESS, BOOL inheritFlag = FALSE);

    BOOL Pulse() const;
    BOOL Set(bool val) const;
    BOOL Set() const;
    BOOL Reset() const;

    bool IsSet(DWORD timeout = 0) const;
    bool IsNotSet(DWORD timeout = 0) const;

    const WinEvent& operator=(bool val);
};

//////////////////////////////////////////////////////////////////////

class WinSemaphore : public WinHandle
{
public:
    explicit WinSemaphore(HANDLE hHandle = nullptr, bool dupHandle = false);
    explicit WinSemaphore(LONG maxCount, LONG initCount = -1, LPCTSTR pName = nullptr, PSECURITY_ATTRIBUTES pSA = nullptr);

    bool Create(LONG maxCount, LONG initCount = -1, LPCTSTR pName = nullptr, PSECURITY_ATTRIBUTES pSA = nullptr);

#ifndef _WIN32_WCE
    bool Open(LPCTSTR pName, DWORD access = SEMAPHORE_ALL_ACCESS, BOOL inheritFlag = FALSE);
#endif

    BOOL Release(LONG releaseCount = 1, LPLONG pPreviousCount = nullptr) const;

    // Set count to maximum
    void releaseAll(long maxCount); //-V126

    // Set count to zero (non-signalled state)
    void clear() const;
};

class WinSemaphoreAutoRelease
{
public:
    explicit WinSemaphoreAutoRelease(HANDLE hSemaphore);
    WinSemaphoreAutoRelease(const WinSemaphoreAutoRelease&) = delete;
    void operator=(const WinSemaphoreAutoRelease&) = delete;
    ~WinSemaphoreAutoRelease();

    BOOL release();

protected:
    HANDLE _hSemaphore;
};

//////////////////////////////////////////////////////////////////////

class WinMutex : public WinHandle
{
public:
    enum
    {
        MUTEX_CREATE_FAILED,
        MUTEX_CREATE_NOT_OWNED,
        MUTEX_CREATE_OWNED
    };

public:
    explicit WinMutex(HANDLE hHandle = nullptr, bool dupHandle = false);
    explicit WinMutex(BOOL initialOwner, LPCTSTR pName = nullptr, PSECURITY_ATTRIBUTES pSA = nullptr);
    ~WinMutex() override;

    BOOL Close() override;

    bool Create(BOOL initialOwner = FALSE, LPCTSTR pName = nullptr, PSECURITY_ATTRIBUTES pSA = nullptr);
    int  CreateAsOwner(LPCTSTR pName = nullptr, PSECURITY_ATTRIBUTES pSA = nullptr);

#ifndef _WIN32_WCE
    bool Open(LPCTSTR pName, DWORD access = MUTEX_ALL_ACCESS, BOOL inheritFlag = FALSE);
#endif

    BOOL Release() const;
};

class WinMutexAutoRelease final
{
public:
    explicit WinMutexAutoRelease(HANDLE hMutex);
    WinMutexAutoRelease(const WinMutexAutoRelease&) = delete;
    void operator=(const WinMutexAutoRelease&) = delete;
    ~WinMutexAutoRelease();

    BOOL Release();

protected:
    HANDLE _hMutex;
};

//////////////////////////////////////////////////////////////////////

#ifndef _WIN32_WCE

class WinTimerQueue final
{
public:
    WinTimerQueue(bool create = true);
    WinTimerQueue(const WinTimerQueue&) = delete;
    void operator=(const WinTimerQueue&) = delete;
    ~WinTimerQueue();

    bool Create();
    BOOL Delete();
    BOOL Delete(HANDLE hCompletionEvent);

    HANDLE GetHandle() const;
    operator HANDLE() const;

public:
    HANDLE _hTimerQueue{ nullptr };
};

//////////////////////////////////////////////////////////////////////

class WinTimerQueueTimer
{
public:
    WinTimerQueueTimer() = default;
    WinTimerQueueTimer(DWORD dueTime, DWORD period, WAITORTIMERCALLBACK callback, PVOID pParameter = nullptr, HANDLE hTimerQueue = nullptr, ULONG flags = 0);
    WinTimerQueueTimer(const WinTimerQueueTimer&) = delete;
    void operator=(const WinTimerQueueTimer&) = delete;
    virtual ~WinTimerQueueTimer();

    BOOL Create(DWORD dueTime, DWORD period, WAITORTIMERCALLBACK callback, PVOID pParameter = nullptr, HANDLE hTimerQueue = nullptr, ULONG flags = 0);

    virtual BOOL Delete();
    virtual BOOL Delete(HANDLE hCompletionEvent);

    HANDLE GetHandle() const;
    operator HANDLE() const;

public:
    HANDLE _hTimer     { nullptr };
    HANDLE _hTimerQueue{ nullptr };
};

class WinTimerQueueTimerCallback : public WinTimerQueueTimer
{
public:
    typedef std::function<void()> TimerCallback;

public:
    WinTimerQueueTimerCallback() = default;
    WinTimerQueueTimerCallback(DWORD dueTime, DWORD period, TimerCallback timerCallback, HANDLE hTimerQueue = nullptr, ULONG flags = 0);
    WinTimerQueueTimerCallback(const WinTimerQueueTimerCallback&) = delete;
    void operator=(const WinTimerQueueTimerCallback&) = delete;
    virtual ~WinTimerQueueTimerCallback() override;

    BOOL Create(DWORD dueTime, DWORD period, TimerCallback timerCallback, HANDLE hTimerQueue = nullptr, ULONG flags = 0);

    BOOL Delete() override;
    BOOL Delete(HANDLE hCompletionEvent) override;

protected:
    virtual void Callback();

public:
    TimerCallback _timerCallback{ nullptr };

private:
    static void CALLBACK WinTimerCallback(PVOID pThis, BOOLEAN /*timerOrWaitFired*/);
};

//class WinTimerQueueTimerCallback : public WinTimerQueueTimer
//{
//protected:
//    static void CALLBACK TimerCallback(PVOID pThis, BOOLEAN timerOrWaitFired);
//    virtual void Callback() = 0;
//
//public:
//    WinTimerQueueTimerCallback()                                  {}
//    WinTimerQueueTimerCallback(const WinTimerQueueTimerCallback&) = delete;
//    void operator=(const WinTimerQueueTimerCallback&) = delete;
//
//    WinTimerQueueTimerCallback(DWORD dueTime, DWORD period = 0, HANDLE hTimerQueue = nullptr, ULONG flags = 0);
//
//    BOOL Create(DWORD dueTime, DWORD period = 0, HANDLE hTimerQueue = nullptr, ULONG flags = 0);
//};

#endif // _WIN32_WCE

//////////////////////////////////////////////////////////////////////

//class WinChangeNotification
//{
//public:
//    WinChangeNotification() = default;
//    WinChangeNotification(LPCTSTR pPath, BOOL watchSubtree, DWORD notifyFilter);
//    WinChangeNotification(const WinChangeNotification&) = delete;
//    void operator=(const WinChangeNotification&) = delete;
//    virtual ~WinChangeNotification();
//
//    virtual bool Close();
//    bool IsOpen() const;
//
//    HANDLE GetHandle() const;
//    operator HANDLE() const;
//
//    bool FindFirst(LPCTSTR pPath, BOOL watchSubtree, DWORD notifyFilter);
//    bool FindNext() const;
//
//    DWORD WaitFor(DWORD timeOut = INFINITE) const;
//
//protected:
//    HANDLE _handle{ INVALID_HANDLE_VALUE };
//
//};
//
//////////////////////////////////////////////////////////////////////

extern SECURITY_ATTRIBUTES winNullSecurityAttribute;

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#define WIN_USE_SERVICE
#include "../_lean_windows_.h"
#include <filesystem>
#include <string>
#include <vector>
#include "WinHandle.h"
#include "../../Error/Xception.h"

namespace fs = std::filesystem;

class WinService
{
public:
    WinService();
    WinService(const WinService&) = delete;
    void operator=(const WinService&) = delete;
    virtual ~WinService() = default;

    virtual DWORD main(int argc, wchar_t* argv[]) final;

    void setShutdownEvent()                             { _shutdownEvent.Set(); }
    void resetShutdownEvent()                           { _shutdownEvent.Reset(); }
    WinEvent& getShutdownEvent() const                  { return _shutdownEvent; }

    bool isDebug() const                                { return _debug; }
    bool isRunning() const                              { return _status.dwCurrentState == SERVICE_RUNNING; }
    bool isPaused() const                               { return _status.dwCurrentState == SERVICE_PAUSED; }

    virtual const wchar_t* getServiceName() const;
    virtual const wchar_t* getDisplayName() const;
    virtual const wchar_t* getDescription() const = 0;

    enum class EventType : WORD
    {
        Success = 0,
        Error   = 1,
        Warning = 2,
        Info    = 4,
        Debug   = 8
    };

    void reportEvent(EventType type, const char* pErrStr) const;
    void reportEvent(EventType type, const wchar_t* pErrStr) const;

    DWORD reportError(const char* pWhat) const;
    DWORD reportError(Xception& e) const;
    DWORD reportError(DWORD lastError) const;

private:
    void printUsage() const;

protected:
    virtual void printUsageOptions() const;
    virtual void printUsageDetails() const;

private:
    void printNameAndVersion() const;
    enum class Command
    {
        Exit,
        Install,
        Remove,
        Stop,
        Start,
        Restart,
        Debug,
        Service,
        Custom
    };
    Command parseCommandLine(int argc, wchar_t* _argv[], std::vector<std::wstring>& params);

    virtual void installService(std::vector<std::wstring>& params);
    void stopService(bool deleteService = false) const;
    void startService() const;
    void pauseService() const;
    void serviceCtrl(DWORD ctrlCode);

protected:
    virtual DWORD   getServiceType() const;
    virtual DWORD   getStartType() const;
    virtual DWORD   getErrorControl() const;
    virtual LPCTSTR getLoadOrderGroup() const;
    virtual LPCTSTR getDependencies() const;
    virtual LPCTSTR getAccount() const;
    virtual LPCTSTR getPassword() const;

    virtual bool isRunning();

private:
    void debugMain();
    void serviceMain();

    static void WINAPI StaticServiceMain(DWORD argc, LPTSTR* pArgv);
    static void WINAPI StaticServiceCtrl(DWORD ctrlCode);
    static BOOL WINAPI HandlerRoutine(DWORD /*ctrlType*/);
    static void DebugKeyInputThread();

protected:
    virtual DWORD customMain(std::vector<std::wstring>& params);

    bool reportStatusToSCMgr(DWORD curState, DWORD exitCode, DWORD waitHint);

    virtual void parseInstallParam(const std::wstring& param);
    virtual void parseParams(std::vector<std::wstring>& params);

    virtual DWORD initService();
    /**
        @remarks runService MUST call reportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, 0)
            runService MAY call reportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, 0)
    */
    virtual DWORD runService() = 0;

    virtual bool canPause() const;

    // These are called in response to a serviceCtrl request from Windows
    virtual DWORD shutdownService(DWORD& waitHint);
    virtual DWORD pauseService   (DWORD& waitHint);
    virtual DWORD continueService(DWORD& waitHint);
    virtual DWORD controlsAccepted();

protected:
    static fs::path    _filename;
    static WinEvent    _shutdownEvent;
    static bool        _debug;
    static WinService* _pService;

    SERVICE_STATUS        _status;
    SERVICE_STATUS_HANDLE _hStatusHandle = nullptr;
    WinMutex              _runningMutex;

private:
    DWORD _exitCode = NO_ERROR;
};

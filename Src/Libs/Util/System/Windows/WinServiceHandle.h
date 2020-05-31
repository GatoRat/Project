// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#define WIN_USE_SERVICE
#include "../_lean_windows_.h"
#include <ostream>
#include <string>

class WinServiceHandle
{
public:
    explicit WinServiceHandle(std::wstring_view serviceName);
    WinServiceHandle(const WinServiceHandle&) = delete;
    void operator=(const WinServiceHandle&) = delete;
    virtual ~WinServiceHandle();

    DWORD getStatus(SERVICE_STATUS_PROCESS& status);
    DWORD getCurState();

    bool stopService(SERVICE_STATUS_PROCESS* pStatus = nullptr);

    void stopServiceWait(std::ostream* pStream = nullptr);
    void startServiceWait(std::ostream* pStream = nullptr);

    void deleteService();

    /**
        @returns \b true if successful, \b false if wait timed out.
    */
    bool waitOnState(DWORD stateToWaitOn, SERVICE_STATUS_PROCESS& status, std::ostream* pStream = nullptr, const char* pInitialStr = nullptr);

    const std::string& getServiceName() const  { return _serviceName; }

private:
    std::string _serviceName;
    SC_HANDLE   _hSCManager{ nullptr };
    SC_HANDLE   _hService  { nullptr };
};

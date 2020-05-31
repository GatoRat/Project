// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "WinServiceHandle.h"
#include <sstream>
#include "../../Error/Xception.h"
#include "../../String/Convert.h"
#include "../../Time/Defines.Time.h"

using namespace std;

WinServiceHandle::WinServiceHandle(wstring_view serviceName)
    : _serviceName(Convert::ToUtf8(serviceName))
{
    _hSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (_hSCManager)
    {
        wstring tmpServiceName(serviceName);
        _hService = OpenService(_hSCManager, tmpServiceName.c_str(), SERVICE_ALL_ACCESS);
        if (!_hService)
        {
            DWORD lastError = GetLastError();

            ostringstream ss;
            ss << "WinServiceHandle(" << _serviceName << ")";

            throw Xception(static_cast<intptr_t>(lastError), ss.str(), GetErrStrOS);
        }
    }
    else
    {
        DWORD lastError = GetLastError();

        ostringstream ss;
        ss << "WinServiceHandle(" << _serviceName << ")";

        throw Xception(static_cast<intptr_t>(lastError), ss.str(), GetErrStrOS);
    }
}

WinServiceHandle::~WinServiceHandle()
{
    CloseServiceHandle(_hService);
    CloseServiceHandle(_hSCManager);
}

DWORD WinServiceHandle::getStatus(SERVICE_STATUS_PROCESS& status)
{
    DWORD bytesNeeded = 0;
    if (!QueryServiceStatusEx(
        _hService,
        SC_STATUS_PROCESS_INFO,
        reinterpret_cast<BYTE*>(&status),
        static_cast<DWORD>(sizeof(SERVICE_STATUS_PROCESS)),
        &bytesNeeded))
    {
        throw Xception(GetLastError(), ("QueryServiceStatusEx failed (" + _serviceName + ")").c_str(), GetErrStrOS);
    }

    return status.dwCurrentState;
}

DWORD WinServiceHandle::getCurState()
{
    SERVICE_STATUS_PROCESS status;
    getStatus(status);
    return status.dwCurrentState;
}

bool WinServiceHandle::stopService(SERVICE_STATUS_PROCESS* pStatus)
{
    bool alreadyStopped = false;

    SERVICE_CONTROL_STATUS_REASON_PARAMS params;
    memset(&params, 0, sizeof(SERVICE_CONTROL_STATUS_REASON_PARAMS));

    params.dwReason = SERVICE_STOP_REASON_FLAG_PLANNED | SERVICE_STOP_REASON_MAJOR_NONE | SERVICE_STOP_REASON_MINOR_NONE;

    [[maybe_unused]] DWORD lastError = NO_ERROR;
    if (!ControlServiceEx(_hService, SERVICE_CONTROL_STOP, SERVICE_CONTROL_STATUS_REASON_INFO, &params))
    {
        lastError = GetLastError();
        if (lastError == ERROR_SERVICE_NOT_ACTIVE)
        {
            alreadyStopped = true;
        }
        else
        {
            throw Xception(GetLastError(), ("ControlServiceEx failed (" + _serviceName + ")").c_str(), GetErrStrOS);
        }
    }

    if (pStatus)
    {
        memcpy(pStatus, &params.ServiceStatus, sizeof(SERVICE_STATUS_PROCESS));
    }

    return alreadyStopped;
}

void WinServiceHandle::stopServiceWait(ostream* pStream)
{
    SERVICE_STATUS_PROCESS status;
    bool alreadyStopped = stopService(&status);
    if (!alreadyStopped && status.dwWaitHint && !waitOnState(SERVICE_STOP_PENDING, status, pStream, "Stopping"))
    {
        throw Xception(ERROR_TIMEOUT, ("Stop timed out (" + _serviceName + ")").c_str(), GetErrStrOS);
    }

    if (status.dwCurrentState != SERVICE_STOPPED)
    {
        throw Xception(0, ("Stop failed (" + _serviceName + ")").c_str());
    }

    if (pStream)
    {
        *pStream << (alreadyStopped ? "Already Stopped" : "\nStopped") << endl;
    }
}

void WinServiceHandle::startServiceWait(ostream* pStream)
{
    stopServiceWait();

    if (!StartService(_hService, 0, nullptr))
    {
        throw Xception(GetLastError(), ("StartService failed (" + _serviceName + ")").c_str(), GetErrStrOS);
    }

    SERVICE_STATUS_PROCESS status;
    getStatus(status);
    if (status.dwWaitHint && !waitOnState(SERVICE_START_PENDING, status, pStream, "Starting"))
    {
        throw Xception(ERROR_TIMEOUT, ("Start timed out (" + _serviceName + ")").c_str(), GetErrStrOS);
    }

    if (status.dwCurrentState != SERVICE_RUNNING)
    {
        throw Xception(0, ("Start failed (" + _serviceName + ")").c_str());
    }

    if (pStream)
    {
        *pStream << "\nStarted" << endl;
    }
}

bool WinServiceHandle::waitOnState(DWORD stateToWaitOn, SERVICE_STATUS_PROCESS& status, ostream* pStream, const char* pInitialStr)
{
    bool success = true;

    if (status.dwCurrentState == stateToWaitOn)
    {
        if (pStream)
        {
            if (pInitialStr)
            {
                *pStream << pInitialStr;
                *pStream << ' ';
            }
            *pStream << _serviceName;
        }

        DWORD startTickCount = GetTickCount();
        DWORD oldCheckPoint = status.dwCheckPoint;

        for (;;)
        {
            // Do not wait longer than the wait hint. A good interval is
            // one-tenth of the wait hint but not less than 1/2 second
            // and not more than 10 seconds.

            DWORD waitTime = status.dwWaitHint / 10;

            if (waitTime < (MILLISECONDS_PER_SECOND / 2))
            {
                waitTime = MILLISECONDS_PER_SECOND / 2;
            }
            else if (waitTime > (10 * MILLISECONDS_PER_SECOND))
            {
                waitTime = 10 * MILLISECONDS_PER_SECOND;
            }

            Sleep(waitTime);

            if (getStatus(status) != stateToWaitOn)
            {
                break;
            }

            if (status.dwCheckPoint > oldCheckPoint)
            {
                startTickCount = GetTickCount();
                oldCheckPoint = status.dwCheckPoint;
            }
            else if (GetTickCount() - startTickCount > status.dwWaitHint)
            {
                success = false;
                break;
            }

            if (pStream)
            {
                *pStream << '.';
            }
        }
    }

    return success;
}

void WinServiceHandle::deleteService()
{
    if (DeleteService(_hService))
    {
        throw Xception(0, ("Delete failed (" + _serviceName + ")").c_str());
    }
}

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "WinService.h"
#include <tlhelp32.h>
#include <iostream>
#include <sstream>
#include <tchar.h>
#include <thread>
#include "../../Error/ErrString.h"
#include "../../File/FileHelper.h"
#include "../../Log/Log.h"
#include "../../Misc/AppInfo.h"
#include "../../Time/Defines.Time.h"
#include "../../String/Convert.h"
#include "../../String/Str.h"
#include "WinConsoleGetChar.h"
#include "WinServiceHandle.h"

using namespace std;

fs::path    WinService::_filename;
WinEvent    WinService::_shutdownEvent;
bool        WinService::_debug = false;
WinService* WinService::_pService = nullptr;

static const char* pMustBeAdmin = "You must be an administrator to use this command";

WinService::WinService() //-V730
{
    memset(&_status, 0, sizeof(_status));
    _shutdownEvent.Create(TRUE, TRUE);
}

void WinService::printNameAndVersion() const
{
    wcout << getServiceName() << " (" << AppInfo::GetVersionStr().c_str() << ")" << endl;
}

void WinService::printUsage() const
{
    cout << "Usage: ";
    printNameAndVersion();
    cout << "\n";

    printUsageOptions();
    cout << "]\n";

    printUsageDetails();

    cout << endl;
}

void WinService::printUsageOptions() const
{
    cout <<
        " [/? | install | remove | start | stop | restart ";

    if (canPause())
    {
        cout << "| pause ";
    }

    cout <<
        "| d[ebug] | v[er[sion]";
}

void WinService::printUsageDetails() const
{
    cout <<
        "\n"
        "  /?         Show this information\n\n"
        "  install    Install, but do not start, this service (modifies the registry)\n"
        "             The following parameters can be used with 'install':\n"
        "             (Do not use a slash for these parameters.)\n\n"
        "             start=<auto | manual | disabled> (default:auto)\n"
        "             error=<ignore | normal | severe | critical> (default:normal)\n"
        "             group=<load order group>\n"
        "             depend=<semicolon delimited list of dependencies>\n"
        "                    if dependency is a group, prefix a plus sign ('+')\n"
        "                    if dependency contains spaces, quote entire parameter\n"
        "             account=<login account> If in domain use DomainName\\UserName.\n"
        "             password=<password of login name>\n"
        "             param=<startup parameter> (can be more than one)\n"
        "  remove     Remove this service (modifies the registry)\n"
        "  start      Start this service\n"
        "  stop       Stop this service\n"
        "  restart    Restart this service\n";

    if (canPause())
    {
        cout <<
        "  pause      Pause this service\n";
    }

    cout <<
        "  d[ebug]    Debug this service (Run from command line)\n"
        "  v[ersion]  Display version and exit\n";
}

WinService::Command WinService::parseCommandLine(int argc, wchar_t* argv[], vector<wstring>& params)
{
    Command command = Command::Exit;

    if (argc <= 1 || Str::Compare(L"help", argv[1], true) == 0)
    {
        printUsage();
    }
    else
    {
        for (int arg = 1; arg < argc; ++arg)
        {
            if (wcscmp(argv[arg], L"-?") == 0 ||
                wcscmp(argv[arg], L"/?") == 0 ||
                wcscmp(argv[arg], L"-h") == 0 ||
                wcscmp(argv[arg], L"/h") == 0 ||
                wcscmp(argv[arg], L"-help") == 0 ||
                wcscmp(argv[arg], L"/help") == 0)
            {
                printUsage();
                return command;
            }
        }

        if (Str::Compare(L"install", argv[1], true) == 0)
        {
            command = Command::Install;

            for (int arg = 2; arg < argc; ++arg)
            {
                params.push_back(argv[arg]);
            }
        }
        else if (Str::Compare(L"remove", argv[1], true) == 0)
        {
            command = Command::Remove;
        }
        else if (Str::Compare(L"restart", argv[1], true) == 0)
        {
            command = Command::Restart;
        }
        else if (Str::Compare(L"start", argv[1], true) == 0)
        {
            command = Command::Start;
        }
        else if (Str::Compare(L"stop", argv[1], true) == 0)
        {
            command = Command::Stop;
        }
        else if (canPause() && Str::Compare(L"pause", argv[1], true) == 0)
        {
        }
        else if (Str::Compare(L"debug", argv[1], true) == 0 ||
                 Str::Compare(L"d", argv[1], true) == 0)
        {
            _debug = true;
            command = Command::Debug;

            for (int arg = 2; arg < argc; ++arg)
            {
                params.push_back(argv[arg]);
            }
        }
        else if (Str::Compare(L"version", argv[1], true) == 0 ||
                 Str::Compare(L"ver", argv[1], true) == 0 ||
                 Str::Compare(L"v", argv[1], true) == 0)
        {
            printNameAndVersion();
        }
        else
        {
            _debug = true;
            command = Command::Custom;

            for (int arg = 1; arg < argc; ++arg)
            {
                params.push_back(argv[arg]);
            }
        }
    }

    return command;
}

fs::path GetParentPath(DWORD pid)
{
    fs::path parentPath;

    PROCESSENTRY32 processEntry = { 0 };
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    WinHandle snapshotHandle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    if (Process32First(snapshotHandle, &processEntry))
    {
        do
        {
            if (processEntry.th32ProcessID == pid)
            {
                DWORD parentId = processEntry.th32ParentProcessID;

                WinHandle parentHandle(OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, parentId));
                if (parentHandle.IsOpen())
                {
                    wchar_t path[MAX_PATH];
                    DWORD pathLen = MAX_PATH;
                    if (QueryFullProcessImageName(parentHandle, 0, path, &pathLen))
                    {
                        parentPath = path;
                    }
                }
                break;
            }

        } while (Process32Next(snapshotHandle, &processEntry));
    }

    return parentPath;
}

DWORD WinService::main(int argc, wchar_t* argv[])
{
    _exitCode = NO_ERROR;

    vector<wstring> params;

    Command command = Command::Exit;

    if (EndsWith(GetParentPath(GetCurrentProcessId()), L"services.exe"))
    {
        command = Command::Service;

        for (int arg = 1; arg < argc; ++arg)
        {
            params.push_back(argv[arg]);
        }
    }
    else
    {
        command = parseCommandLine(argc, argv, params);
    }

    if (command != Command::Exit)
    {
        _pService = this;

        switch (command)
        {
            case Command::Remove:
                stopService(true);
                break;

            case Command::Install:
                installService(params);
                break;

            case Command::Start:
            case Command::Restart:
                startService();
                break;

            case Command::Stop:
                stopService(false);
                break;

            case Command::Custom:
                _exitCode = customMain(params);
                break;

            default:
            {
                parseParams(params);

                if (_debug)
                {
                    debugMain();
                }
                else
                {
                    SERVICE_TABLE_ENTRY dispatchTable[] =
                    {
                        { const_cast<wchar_t*>(getServiceName()), StaticServiceMain },
                        { nullptr, nullptr }
                    };

                    if (!StartServiceCtrlDispatcher(dispatchTable))
                    {
                        _exitCode = reportError("StartServiceCtrlDispatcher");
                    }
                }
                break;
            }
        }
    }

    return _exitCode;
}

const wchar_t* WinService::getServiceName() const
{
    return AppInfo::GetExeName().c_str();
}

const wchar_t* WinService::getDisplayName() const
{
    return getServiceName();
}

DWORD WinService::reportError(const char* pWhat) const
{
    DWORD errCode = GetLastError();

    string errStr{ pWhat };
    errStr += ": ";
    errStr += to_string(errCode);
    errStr += ": ";
    errStr += GetErrStrOS(GetLastError()).value_or("Unknown Error");

    reportEvent(EventType::Error, errStr.c_str());

    return errCode ? errCode : static_cast<DWORD>(-1);
}

DWORD WinService::reportError(const Xception& e) const
{
    reportEvent(EventType::Error, e.errStr().c_str());
    return static_cast<DWORD>(e.code() ? e.code() : -1);
}

DWORD WinService::reportError(DWORD lastError) const
{
    if (lastError != NO_ERROR)
    {
        auto errStr = GetErrStrOSW(static_cast<intptr_t>(lastError), L"Unknown Error");
        reportEvent(EventType::Error, errStr.value().c_str());
    }
    return static_cast<DWORD>(lastError);
}

void WinService::reportEvent(EventType type, const wchar_t* pErrStr) const
{
    if (!Str::IsNullOrWhiteSpace(pErrStr))
    {
        wstring errStr;

        switch (type)
        {
            case EventType::Success:
                break;

            case EventType::Error:
                errStr += L"Error: ";
                break;

            case EventType::Warning:
                errStr += Log::WarningSymbol;
                break;

            case EventType::Info:
                errStr += Log::InfoSymbol;
                break;

            case EventType::Debug:
                type = EventType::Info;
                errStr += Log::DebugSymbol;
                break;
        }

        errStr += pErrStr;

        if (_debug)
        {
            wcout << errStr << endl;
        }
        else
        {
            HANDLE hEventLog = RegisterEventSource(NULL, getServiceName());
            if (hEventLog)
            {
                const wchar_t* strings[1];
                strings[0] = pErrStr;
                ReportEventW(hEventLog, static_cast<WORD>(type), 0, 1, nullptr, 1, 0, strings, nullptr);
                DeregisterEventSource(hEventLog);
            }
        }

        Log::String(Convert::ToUtf8(errStr));
    }
}

void WinService::reportEvent(EventType type, const char* pErrStr) const
{
    wstring errStr{ Convert::ToWide(pErrStr) };
    reportEvent(type, errStr.c_str());
}

///////////////////////////////////////////////////////////////////

typedef BOOL (CALLBACK * PCHANGESERVICECONFIG2)(SC_HANDLE, DWORD, LPVOID);

void WinService::installService(vector<wstring>& params)
{
    DWORD startType    = getStartType();
    DWORD errorControl = getErrorControl();

    LPCTSTR pLoadOrderGroup = getLoadOrderGroup();
    LPCTSTR pAccount        = getAccount();
    LPCTSTR pPassword       = getPassword();
    LPCTSTR pDependencies   = getDependencies();
    wstring dependencies;
    wstring exePathname{ AppInfo::GetExePathname() };

    for (auto& param : params)
    {
        if (param[0] != '/' && param[0] != '-')
        {
            size_t offset = param.find('=');
            if (offset && offset != wstring::npos)
            {
                ++offset;

                if (Str::StartsWith(param, L"start="))
                {
                    if (Str::EndsWith(param, L"=auto"))
                    {
                        startType = SERVICE_AUTO_START;
                    }
                    else if (Str::EndsWith(param, L"=manual"))
                    {
                        startType = SERVICE_DEMAND_START;
                    }
                    else if (Str::EndsWith(param, L"=disabled"))
                    {
                        startType = SERVICE_DISABLED;
                    }
                }
                else if (Str::StartsWith(param, L"error="))
                {
                    if (Str::EndsWith(param, L"=ignore"))
                    {
                        errorControl = SERVICE_ERROR_IGNORE;
                    }
                    else if (Str::EndsWith(param, L"=normal"))
                    {
                        errorControl = SERVICE_ERROR_NORMAL;
                    }
                    else if (Str::EndsWith(param, L"=severe"))
                    {
                        errorControl = SERVICE_ERROR_SEVERE;
                    }
                    else if (Str::EndsWith(param, L"=critical"))
                    {
                        errorControl = SERVICE_ERROR_CRITICAL;
                    }
                }
                else if (Str::StartsWith(param, L"group="))
                {
                    pLoadOrderGroup = param.c_str() + offset;
                }
                else if (Str::StartsWith(param, L"depend="))
                {
                    dependencies.reserve(param.size());

                    wchar_t ch;
                    do
                    {
                        ch = param[offset++];
                        dependencies.push_back(ch == L';' ? 0 : ch);

                    } while (ch);

                    dependencies.push_back(0);
                    pDependencies = dependencies.c_str();
                }
                else if (Str::StartsWith(param, L"account="))
                {
                    pAccount = param.c_str() + offset;
                }
                else if (Str::StartsWith(param, L"password="))
                {
                    pPassword = param.c_str() + offset;
                }
                else if (Str::StartsWith(param, L"param="))
                {
                    exePathname += L' ';
                    exePathname += param.c_str() + offset;
                }
                else
                {
                    parseInstallParam(param);
                }
            }
            else
            {
                parseInstallParam(param);
            }
        }
        else
        {
            parseInstallParam(param);
        }
    }

    SC_HANDLE hSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (hSCManager)
    {
        SC_HANDLE hService = CreateService(
            hSCManager,
            getServiceName(),
            getDisplayName(),
            SERVICE_ALL_ACCESS,
            getServiceType(),
            startType,
            errorControl,
            exePathname.c_str(),
            pLoadOrderGroup,
            NULL,
            pDependencies,
            pAccount,
            pPassword);

        if (hService)
        {
            PCHANGESERVICECONFIG2 pChangeServiceConfig2 = (PCHANGESERVICECONFIG2) GetProcAddress(GetModuleHandle(_T("advapi32.dll")), "ChangeServiceConfig2W");
            if (pChangeServiceConfig2)
            {
                SERVICE_DESCRIPTION sd;
                sd.lpDescription = const_cast<LPWSTR>(getDescription());
                pChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd);
            }

            cout << "Service installed." << endl;
            CloseServiceHandle(hService);
        }
        else
        {
            _exitCode = reportError("CreateService()");
        }

        CloseServiceHandle(hSCManager);
    }
    else
    {
        DWORD lastErr = GetLastError();

        if (lastErr == ERROR_ACCESS_DENIED)
        {
            cout << "\nError: " << pMustBeAdmin << endl;
        }
        else
        {
            cout << "\nError: " << "OpenSCManager() failed: " << GetErrStrOS(static_cast<intptr_t>(lastErr)).value_or("Unknown Error")
                 << " (" << lastErr << ")" << endl;
        }

        _exitCode = lastErr;
    }
}

DWORD WinService::getServiceType() const
{
    return SERVICE_WIN32_OWN_PROCESS;
}

DWORD WinService::getStartType() const
{
    return SERVICE_AUTO_START;
}

DWORD WinService::getErrorControl() const
{
    return SERVICE_ERROR_NORMAL;
}

LPCTSTR WinService::getLoadOrderGroup() const
{
    return nullptr;
}

LPCTSTR WinService::getDependencies() const
{
    return nullptr;
}

LPCTSTR WinService::getAccount() const
{
    return nullptr;
}

LPCTSTR WinService::getPassword() const
{
    return nullptr;
}

void WinService::parseInstallParam(const wstring& /*param*/)
{
}

void WinService::parseParams(vector<wstring>& /*params*/)
{
}

///////////////////////////////////////////////////////////////////

void WinService::stopService(bool deleteService) const
{
    try
    {
        WinServiceHandle serviceHandle(getServiceName());
        serviceHandle.stopServiceWait(&cout);

        if (deleteService)
        {
            serviceHandle.deleteService();
        }
    }
    catch (const Xception& e)
    {
        if (e.code() == ERROR_ACCESS_DENIED)
        {
            cout << "\nError: " << pMustBeAdmin << endl;
        }
        else
        {
            cout << "\nError: " << e.errStr() << endl;
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void WinService::startService() const
{
    try
    {
        WinServiceHandle serviceHandle(getServiceName());
        serviceHandle.startServiceWait(&cout);
    }
    catch (const Xception& e)
    {
        if (e.code() == ERROR_ACCESS_DENIED)
        {
            cout << "\nError: " << pMustBeAdmin << endl;
        }
        else
        {
            cout << "\nError: " << e.errStr() << endl;
        }
    }
}

void WinService::pauseService() const
{
}

///////////////////////////////////////////////////////////////////////////

BOOL WINAPI WinService::HandlerRoutine(DWORD /*ctrlType*/)
{
    _pService->setShutdownEvent();
    return TRUE;
}

void WinService::DebugKeyInputThread()
{
    SetConsoleCtrlHandler(HandlerRoutine, TRUE);

    for (;;)
    {
        wcout << "Press 'q' to quit: ";

        auto key = WinConsoleGetChar(_pService->getShutdownEvent());
        if (key < 0 || key == 'Q' || key == 'q')
        {
            wcout << "Quit" << endl;
            break;
        }
        wcout << "Unrecognized character: " << key << endl;
    }

    wcout << "Setting shutdown event." << endl;
    _pService->setShutdownEvent();
    SetConsoleCtrlHandler(NULL, FALSE);
}

///////////////////////////////////////////////////////////////////////////

void WINAPI WinService::StaticServiceMain(DWORD /*argc*/, LPTSTR* /*pArgv*/)
{
    _pService->serviceMain();
}

void WINAPI WinService::StaticServiceCtrl(DWORD ctrlCode)
{
    _pService->serviceCtrl(ctrlCode);
}

bool WinService::reportStatusToSCMgr(DWORD curState, DWORD exitCode, DWORD waitHint)
{
    static DWORD checkPoint = 1;

    if (curState == SERVICE_START_PENDING ||
        curState == SERVICE_STOP_PENDING  ||
        curState == SERVICE_PAUSE_PENDING ||
        curState == SERVICE_CONTINUE_PENDING)
    {
        _status.dwControlsAccepted = 0;
    }
    else
    {
        _status.dwControlsAccepted = controlsAccepted();
    }

    _status.dwCurrentState  = curState;
    _status.dwWin32ExitCode = exitCode;
    _status.dwWaitHint      = waitHint;

    if (curState == SERVICE_RUNNING ||
        curState == SERVICE_STOPPED ||
        curState == SERVICE_PAUSED)
    {
        _status.dwCheckPoint = 0;
    }
    else
    {
        _status.dwCheckPoint = checkPoint++;
    }

    bool result = true;
    if (!_debug) // when debugging we don't report to the SCM
    {
        if (!SetServiceStatus(_hStatusHandle, &_status))
        {
            stringstream ss;
            ss << "SetServiceStatus failed with " << GetLastError() << ".";
            reportEvent(EventType::Error, ss.str().c_str());
            result = false;
        }
    }
    return result;
}

void WinService::serviceCtrl(DWORD ctrlCode)
{
    DWORD waitHint = 0;

    switch (ctrlCode)
    {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN:
        {
            DWORD rval = shutdownService(waitHint);
            if (!rval)
            {
                return;
            }
            _status.dwCurrentState = rval;
            break;
        }

        case SERVICE_CONTROL_PAUSE:
        {
            DWORD rval = pauseService(waitHint);
            if (!rval)
            {
                return;
            }
            _status.dwCurrentState = rval;
            break;
        }

        case SERVICE_CONTROL_CONTINUE:
        {
            DWORD rval = continueService(waitHint);
            if (!rval)
            {
                return;
            }
            _status.dwCurrentState = rval;
            break;
        }
    }

    reportStatusToSCMgr(_status.dwCurrentState, NO_ERROR, waitHint);
}

bool WinService::isRunning()
{
    wstring mutexName(getServiceName());
    mutexName += L"/RunningMutex";

    return _runningMutex.CreateAsOwner(mutexName.c_str(), &winNullSecurityAttribute) != WinMutex::MUTEX_CREATE_OWNED;
}

DWORD WinService::customMain(std::vector<std::wstring>& params)
{
    DWORD exitCode = NO_ERROR;

    if (!params.empty())
    {
        exitCode = 1;
        wcout << "Error: Unknown command: \"" << params[1] << "\"" << endl;
    }

    return exitCode;
}

DWORD WinService::initService()
{
    return NO_ERROR;
}

void WinService::debugMain()
{
    if (isRunning())
    {
        wcout << getDisplayName() << " is already running!" << endl;
    }
    else
    {
        _exitCode = initService();
        if (_exitCode == NO_ERROR)
        {
            resetShutdownEvent();

            wcout << "Debugging \"" << getDisplayName() << "\"\n" << endl;

            thread debugKeyInputThread(DebugKeyInputThread);

            _exitCode = runService();

            setShutdownEvent();

            debugKeyInputThread.join();

            wcout << "Stopped \"" << getDisplayName() << "\"\n" << endl;
        }
        else
        {
            wcout << "Init failed (" << _exitCode << ") \"" << getDisplayName() << "\"\n" << endl;
        }
    }
}

void WinService::serviceMain()
{
    _hStatusHandle = RegisterServiceCtrlHandler(getServiceName(), StaticServiceCtrl);
    if (_hStatusHandle)
    {
        _status.dwServiceType = getServiceType();
        _status.dwServiceSpecificExitCode = 0;

        _exitCode = NO_ERROR;

        if (reportStatusToSCMgr(SERVICE_START_PENDING, NO_ERROR, 10000))
        {
            if (isRunning())
            {
                _exitCode = 1;
            }
            else
            {
                _exitCode = initService();
                if (_exitCode == NO_ERROR)
                {
                    resetShutdownEvent();
                    _exitCode = runService();
                }
            }
        }

        reportStatusToSCMgr(SERVICE_STOPPED, _exitCode, 0);
    }
}

bool WinService::canPause() const
{
    return true;
}

DWORD WinService::shutdownService(DWORD& waitHint)
{
    waitHint = 20 * MILLISECONDS_PER_SECOND;
    setShutdownEvent();
    return SERVICE_STOP_PENDING;
}

DWORD WinService::pauseService(DWORD& waitHint)
{
    waitHint = 0;
    return 0;  // SERVICE_PAUSE_PENDING | SERVICE_PAUSED
}

DWORD WinService::continueService(DWORD& waitHint)
{
    waitHint = 0;
    return 0; // SERVICE_CONTINUE_PENDING | SERVICE_RUNNING
}

DWORD WinService::controlsAccepted()
{
    return SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN; // SERVICE_ACCEPT_PAUSE_CONTINUE
}

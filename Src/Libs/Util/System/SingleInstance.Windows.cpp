// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SingleInstance.h"
#include <filesystem>
#define WIN_USE_GDI
#include "_lean_windows_.h"

namespace fs = std::filesystem;

using namespace std;

SingleInstance::~SingleInstance()
{
    //LibClose();
    //LogClose();

    if (_hMutex)
    {
        ReleaseMutex(_hMutex);
        CloseHandle(_hMutex);
    }
}

//static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM baseWindowTitle)
//{
//    const wchar_t* pBaseWindowTitle = static_cast<const wchar_t*>(baseWindowTitle);
static BOOL CALLBACK EnumWindowsProc(HWND hWnd, const wchar_t* pBaseWindowTitle) //-V2506
{
    int baseWindowTitleLen = static_cast<int>(wcslen(pBaseWindowTitle));

    wchar_t windowName[MAX_PATH * 2];
    int windowNameLen = ::GetWindowTextW(hWnd, windowName, _countof(windowName));
    if (windowNameLen >= baseWindowTitleLen)
    {
        if (wcscmp(pBaseWindowTitle, &windowName[windowNameLen - baseWindowTitleLen]) == 0)
        {
            DWORD foregroundProcessId = ::GetWindowThreadProcessId(::GetForegroundWindow(), NULL);
            DWORD curThreadId = ::GetCurrentThreadId();

            ::AttachThreadInput(foregroundProcessId, curThreadId, TRUE);

            ::ShowWindow(hWnd, ::IsIconic(hWnd) ? SW_RESTORE : SW_SHOW);

            ::BringWindowToTop(hWnd);
            ::SetActiveWindow(hWnd);
            ::SetForegroundWindow(hWnd);
            ::SetFocus(hWnd);

            ::AttachThreadInput(foregroundProcessId, curThreadId, FALSE);

            return FALSE;
        }
    }

    return TRUE;
}

bool SingleInstance::lock(string_view baseWindowTitle_) //-V2506
{
    // The mutex is named such since it can't be a legal filename.
    fs::path windowName(baseWindowTitle_);
    fs::path mutexName(windowName);
    mutexName += "<:Mutex:>";

    _hMutex = ::CreateMutexW(nullptr, TRUE, mutexName.wstring().c_str());
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        EnumWindows(reinterpret_cast<WNDENUMPROC>(EnumWindowsProc), reinterpret_cast<LPARAM>(windowName.wstring().c_str()));
        return false;
    }
    return true;
}

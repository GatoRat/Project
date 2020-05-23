// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "DirChange.h"
#include "../File/XceptionFile.h"
#include "_lean_windows_.h"

using namespace std;

DirChangeNotifier::DirChangeNotifier(const fs::path& pathToMonitor, bool watchSubTree, Callback dirChangeCallback)
{
    start(pathToMonitor, watchSubTree, dirChangeCallback);
}

DirChangeNotifier::~DirChangeNotifier()
{
    stop();
}

void DirChangeNotifier::start(const fs::path& pathToMonitor, bool watchSubTree, Callback dirChangeCallback)
{
    stop();

    _pathToMonitor = pathToMonitor;

    if (dirChangeCallback)
    {
        HANDLE hChange = FindFirstChangeNotificationW(pathToMonitor.wstring().c_str(), watchSubTree,
            FILE_NOTIFY_CHANGE_FILE_NAME  |
            FILE_NOTIFY_CHANGE_DIR_NAME   |
            FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_LAST_WRITE);

        if (hChange == INVALID_HANDLE_VALUE)
        {
            throw XceptionFile(pathToMonitor, GetLastError(), nullptr, GetErrStrOS);
        }

        _stopEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
        _thread = thread([&, dirChangeCallback, hChange]
        {
            HANDLE handles[2];
            handles[0] = _stopEvent;
            handles[1] = hChange;

            do
            {
                const DWORD rval = ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);
                if (rval == WAIT_OBJECT_0)
                {
                    break;
                }

                if (!dirChangeCallback(_pathToMonitor))
                {
                    break;
                }

            } while (::FindNextChangeNotification(handles[1]));

            ::FindCloseChangeNotification(hChange);
        });
    }
}

void DirChangeNotifier::stop()
{
    ::SetEvent(_stopEvent);
    if (_thread.joinable())
    {
        _thread.join();
    }
    ::CloseHandle(_stopEvent);
    _stopEvent = nullptr;
}
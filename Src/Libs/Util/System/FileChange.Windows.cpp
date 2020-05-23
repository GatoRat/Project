// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FileChange.h"
#include "../File/XceptionFile.h"
#include "_lean_windows_.h"

using namespace std;

struct FileChangeData : FileChange::Data
{
    HANDLE handles[2];
    WIN32_FILE_ATTRIBUTE_DATA lastFileInfo;

    explicit FileChangeData(const fs::path& file)
        : handles{ ::CreateEvent(nullptr, TRUE, FALSE, nullptr), INVALID_HANDLE_VALUE }
    {
        if (!::GetFileAttributesExW(file.wstring().c_str(), GetFileExInfoStandard, &lastFileInfo))
        {
            memset(&lastFileInfo, 0, sizeof(lastFileInfo));
        }
    }

    ~FileChangeData() override
    {
        ::SetEvent(handles[0]);
        ::Sleep(0);
        ::FindCloseChangeNotification(handles[1]);
        ::CloseHandle(handles[0]);
    }

    bool hasChanged(const fs::path& file)
    {
        bool changed = false;

        WIN32_FILE_ATTRIBUTE_DATA fileInfo;
        if (!::GetFileAttributesExW(file.wstring().c_str(), GetFileExInfoStandard, &fileInfo))
        {
            memset(&fileInfo, 0, sizeof(fileInfo));
        }

        if (lastFileInfo.ftCreationTime.dwLowDateTime   != fileInfo.ftCreationTime.dwLowDateTime   ||
            lastFileInfo.ftCreationTime.dwHighDateTime  != fileInfo.ftCreationTime.dwHighDateTime  ||
            lastFileInfo.ftLastWriteTime.dwLowDateTime  != fileInfo.ftLastWriteTime.dwLowDateTime  ||
            lastFileInfo.ftLastWriteTime.dwHighDateTime != fileInfo.ftLastWriteTime.dwHighDateTime ||
            lastFileInfo.nFileSizeLow                   != fileInfo.nFileSizeLow                   ||
            lastFileInfo.nFileSizeHigh                  != fileInfo.nFileSizeHigh)
        {
            changed = true;
            lastFileInfo = fileInfo;
        }

        return changed;
    }
};

FileChange::FileChange(const fs::path& fileToMonitor)
    : _file(fileToMonitor)
    , _dir(_file.parent_path())
{
}

FileChange::~FileChange()
{
    close();
    ::Sleep(0);
}

const fs::path& FileChange::getFile() const
{
    return _file;
}

const fs::path& FileChange::getDir() const
{
    return _dir;
}

bool FileChange::waitForChange() //-V2506
{
    FileChangeData* pData = nullptr;

    if (!_data)
    {
        _data = make_unique<FileChangeData>(_file);
        pData = reinterpret_cast<FileChangeData*>(_data.get());

        pData->handles[1] = ::FindFirstChangeNotificationW(_dir.wstring().c_str(), false, FILE_NOTIFY_CHANGE_LAST_WRITE);
        if (pData->handles[1] == INVALID_HANDLE_VALUE)
        {
            throw XceptionFile(_dir, GetLastError(), nullptr, GetErrStrOS);
        }
    }
    else
    {
        pData = reinterpret_cast<FileChangeData*>(_data.get());
        if (!::FindNextChangeNotification(pData->handles[1]))
        {
            return false;
        }
    }

    bool changed = false;

    while (!changed && ::WaitForMultipleObjects(2, pData->handles, FALSE, INFINITE) == (WAIT_OBJECT_0 + 1))
    {
        changed = pData->hasChanged(_file);
    }

    return changed;
}

void FileChange::close()
{
    FileChangeData* pData = reinterpret_cast<FileChangeData*>(_data.get());
    ::SetEvent(pData->handles[0]);
}

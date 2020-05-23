// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SuperBasicFile.h"
#include "../Error/OsError.h"
#include "../File/XceptionFile.h"
#include "../System/_lean_windows_.h"

SuperBasicFile::SuperBasicFile(const fs::path& pathname, bool readOnly, bool forceReadOnly, bool fileMustExist, bool exceptionOnError)
    : _pathname(pathname)
    , _hFile(INVALID_HANDLE_VALUE)
{
    if (readOnly)
    {
        _hFile = ::CreateFileW(_pathname.wstring().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if (!isOpen() && GetLastError() == ERROR_SHARING_VIOLATION && !forceReadOnly)
        {
            _hFile = ::CreateFileW(_pathname.wstring().c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        }
    }
    else
    {
        fileMustExist = true;
        _hFile = ::CreateFileW(_pathname.wstring().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    }

    if (!isOpen() && fileMustExist && exceptionOnError)
    {
        throw XceptionFile(_pathname, OsError::getCode(), "CreateFileW()", GetErrStrOS); //-V106
    }
}

SuperBasicFile::~SuperBasicFile()
{
    if (isOpen())
    {
        CloseHandle(_hFile);
    }
}

bool SuperBasicFile::isOpen() const noexcept
{
    return _hFile != INVALID_HANDLE_VALUE;
}

int64_t SuperBasicFile::getFileSize() const
{
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(_hFile, &fileSize))
    {
        throw XceptionFile(_pathname, OsError::getCode(), "GetFileSizeEx()", GetErrStrOS); //-V106
    }
    return fileSize.QuadPart;
}

uint32_t SuperBasicFile::read(void* pBuffer, uint32_t lenToRead) const
{
    DWORD bytesRead = 0;
    if (pBuffer && lenToRead)
    {
        if (!ReadFile(_hFile, pBuffer, static_cast<DWORD>(lenToRead), &bytesRead, NULL))
        {
            throw XceptionFile(_pathname, OsError::getCode(), "ReadFile()", GetErrStrOS); //-V106
        }
    }
    return static_cast<uint32_t>(bytesRead);
}

uint32_t SuperBasicFile::write(const void* pBuffer, uint32_t lenToWrite) const
{
    DWORD bytesWritten = 0;
    if (pBuffer && lenToWrite)
    {
        if (!WriteFile(_hFile, pBuffer, static_cast<DWORD>(lenToWrite), &bytesWritten, NULL))
        {
            throw XceptionFile(_pathname, OsError::getCode(), "WriteFile()", GetErrStrOS); //-V106
        }
    }
    return static_cast<uint32_t>(bytesWritten);
}

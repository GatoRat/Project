// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TempFolder.h"
#include <algorithm>
#include <exception>
#include "Util/System/_lean_windows_.h"
#include "Util/File/FastFindFile.h"
#include "Util/String/Str.h"
#include "Util/System/SysGetPath.h"
#include "Util/Time/Defines.Time.h"

using namespace std;

void TempFolder::setPath(fs::path path, uint64_t hoursToKeep)
{
    scoped_lock lock(_mutex);

    _hoursToKeep = hoursToKeep ? hoursToKeep : 1;

    try
    {
        fs::create_directory(path);
        _path = move(path);
    }
    catch (exception&)
    {
        _path = SysGetPath::Desktop();
    }

    cleanUp(_path, _hoursToKeep);
}

fs::path TempFolder::getPathname(const char* pPrefix, const char* pExt)
{
    fs::path pathname;

    scoped_lock lock(_mutex);

    if (!_path.empty())
    {
        cleanUp();

        do
        {
            TimeStamp timeStamp;
            do
            {
                timeStamp.setCurTime();

            } while (timeStamp <= _lastTime);

            _lastTime = timeStamp;

            if (Str::IsNullOrEmpty(pPrefix))
            {
                pathname = _path / _lastTime.toStrPacked(TimeStamp::UseMilliseconds);
            }
            else
            {
                pathname = _path / pPrefix;
                if (string_view(pPrefix).back() != '.')
                {
                    pathname += L'.';
                }
                pathname += _lastTime.toStrPacked(TimeStamp::UseMilliseconds);
            }

            if (!Str::IsNullOrEmpty(pExt))
            {
                if (*pExt != '.')
                {
                    pathname += L'.';
                }
                pathname += pExt;
            }

        } while (fs::exists(pathname));
    }

    return pathname;
}

void TempFolder::cleanUp()
{
    cleanUp(_path, _hoursToKeep);
}

void TempFolder::cleanUp(const fs::path& path, uint64_t hoursToKeep)
{
    scoped_lock lock(_mutex);

    FILETIME deleteBefore;
    GetSystemTimeAsFileTime(&deleteBefore);

    {
        ULARGE_INTEGER tmpTime;

        tmpTime.LowPart = deleteBefore.dwLowDateTime;
        tmpTime.HighPart = deleteBefore.dwHighDateTime;

        tmpTime.QuadPart -= hoursToKeep * _100_NANOSECONDS_PER_HOUR;

        deleteBefore.dwLowDateTime = tmpTime.LowPart;
        deleteBefore.dwHighDateTime = tmpTime.HighPart;
    }

    FastFindFile::Find(path, "*", FastFindFile::Types::OnlyFiles, [&](const fs::path& basePath, const FastFindData& findData)->bool
    {
        if (CompareFileTime(&findData.ftLastWriteTime, &deleteBefore) < 0)
        {
            fs::remove(basePath / findData.cFileName);
        }

        return true;
    });
}
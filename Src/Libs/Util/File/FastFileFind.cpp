// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FastFindFile.h"

#ifdef _WIN32

bool FastFindData::IsDots(const wchar_t* pStr)
{
    if (!pStr)
    {
        return false;
    }

    while (*pStr == L'.')
    {
        pStr++;
    }
    return *pStr == 0;
}

bool FastFindData::isDots() const
{
    return IsDots(cFileName);
}

bool FastFindData::isFile() const
{
    return cFileName[0] && isDir() ? false : !isDots();
}

bool FastFindData::isDir() const
{
    return cFileName[0] && (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool FastFindData::isHidden() const
{
    return cFileName[0] && (dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
}

bool FastFindData::isReadOnly() const
{
    return cFileName[0] && (dwFileAttributes & FILE_ATTRIBUTE_READONLY);
}

bool FastFindData::isSystem() const
{
    return cFileName[0] && (dwFileAttributes & FILE_ATTRIBUTE_SYSTEM);
}

bool FastFindData::anyExtendedAttr() const
{
    constexpr DWORD ExtendedAttr = FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY | FILE_ATTRIBUTE_OFFLINE;
    return dwFileAttributes & ExtendedAttr;
}

uint64_t FastFindData::getCreationTime() const
{
    ULARGE_INTEGER v;
    v.LowPart = ftCreationTime.dwLowDateTime;
    v.HighPart = ftCreationTime.dwHighDateTime;
    return v.QuadPart;
}

uint64_t FastFindData::getLastWriteTime() const
{
    ULARGE_INTEGER v;
    v.LowPart = ftLastWriteTime.dwLowDateTime;
    v.HighPart = ftLastWriteTime.dwHighDateTime;
    return v.QuadPart;
}

uint64_t FastFindData::getLength() const
{
    ULARGE_INTEGER v;
    v.LowPart = nFileSizeLow;
    v.HighPart = nFileSizeHigh;
    return v.QuadPart;
}

fs::path FastFindData::getFilename() const
{
    return cFileName;
}

/////////////////////////////////////////////////////////////////////////////

FastFindFile::~FastFindFile()
{
    close();
}

void FastFindFile::close() noexcept
{
    if (_hFind != INVALID_HANDLE_VALUE)
    {
        ::FindClose(_hFind);
        _hFind = INVALID_HANDLE_VALUE;
    }
}

bool FastFindFile::findFirst(const wchar_t* pMask, bool limitToDirs, bool useLargeFetch)
{
    close();

    if (pMask && *pMask)
    {
        _hFind = ::FindFirstFileEx(
            pMask,
            FindExInfoBasic,
            this,
            limitToDirs ? FindExSearchLimitToDirectories : FindExSearchNameMatch,
            nullptr,
            useLargeFetch ? FIND_FIRST_EX_LARGE_FETCH : 0);
    }

    return _hFind != INVALID_HANDLE_VALUE;
}

bool FastFindFile::findNext()
{
    return _hFind == INVALID_HANDLE_VALUE ? false : (::FindNextFile(_hFind, this) != 0);
}

bool FastFindFile::isValid() const
{
    return _hFind != INVALID_HANDLE_VALUE;
}

bool FastFindFile::Find(const fs::path& basePath, const fs::path& mask, Types types, const FindCallback& findCallback)
{
    bool keepFinding = true;

    fs::path searchPath(basePath);
    searchPath /= mask;

    FastFindFile findFile;

    if (findFile.findFirst(searchPath.c_str(), types == OnlyDirs))
    {
        do
        {
            if (!findFile.isDots())
            {
                if (findCallback &&
                    ((types == All) ||
                     (types == OnlyFiles && findFile.isFile()) ||
                     (types == OnlyDirs && findFile.isDir())))
                {
                    keepFinding = findCallback(basePath, findFile);
                }
            }

        } while (keepFinding && findFile.findNext());
    }

    return keepFinding;
}

//bool FastFindFile::FindRecursive(const fs::path& basePath, const wregex* pRegex, const FindCallback& findCallback)
//{
//    bool keepFinding = true;
//
//    fs::path searchPath(basePath);
//    searchPath /= L"*";
//
//    FastFindFile findFile;
//
//    if (findFile.findFirst(searchPath.c_str(), true))
//    {
//        do
//        {
//            if (!findFile.isDots())
//            {
//                if (findFile.isDir())
//                {
//                    fs::path nextPath(basePath);
//                    nextPath /= findFile.cFileName;
//                    keepFinding = FindRecursive(nextPath, pRegex, findCallback);
//                }
//                else if (findCallback)
//                {
//                    if (!pRegex || regex_match(findFile.cFileName, *pRegex))
//                    {
//                        keepFinding = findCallback(basePath, findFile);
//                    }
//                }
//            }
//
//        } while (keepFinding && findFile.findNext());
//    }
//
//    return keepFinding;
//}

bool FastFindFile::FindRecursive(const fs::path& basePath, const FindCallback& findCallback)
{
    bool keepFinding = true;

    fs::path searchPath(basePath);
    searchPath /= L"*";

    FastFindFile findFile;

    if (findFile.findFirst(searchPath.c_str(), true))
    {
        do
        {
            if (!findFile.isDots())
            {
                if (findFile.isDir())
                {
                    if (findCallback(basePath, findFile))
                    {
                        fs::path nextPath(basePath);
                        nextPath /= findFile.cFileName;
                        keepFinding = FindRecursive(nextPath, findCallback);
                    }
                }
                else if (findCallback)
                {
                    keepFinding = findCallback(basePath, findFile);
                }
            }

        } while (keepFinding && findFile.findNext());
    }

    return keepFinding;
}

#endif
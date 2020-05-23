// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FileHelper.h"
#include <atomic>
#include "../String/Str.h"

using namespace std;

bool EndsWith(const fs::path& pathToCheck, wstring_view strToCompare, bool ignoreCase)
{
    return Str::EndsWith(pathToCheck.c_str(), strToCompare, ignoreCase);
}

bool EndsWithPath(const fs::path& pathToCheck, const fs::path& strToCompare, bool ignoreCase)
{
    return Str::EndsWith(pathToCheck.c_str(), strToCompare.c_str(), ignoreCase);
}

int Compare(const fs::path& path1, wstring_view strToCompare, bool ignoreCase)
{
    return Str::CompareV(path1.c_str(), strToCompare, ignoreCase);
}

int ComparePath(const fs::path& path1, const fs::path& path2, bool ignoreCase)
{
    return Str::Compare(path1.c_str(), path2.c_str(), ignoreCase);
}

///////////////////////////////////////////////////////////////////////////

bool FixAndVerify(fs::path& pathname, const fs::path& extension1, const fs::path* pExtension2)
{
    bool success = false;

    if (!pathname.empty())
    {
        error_code ec;

        if (pExtension2)
        {
            if (EndsWithPath(pathname, extension1) || EndsWithPath(pathname, *pExtension2))
            {
                pathname.replace_extension();
            }

            fs::path tmpPathname(pathname);
            tmpPathname += extension1;
            success = fs::is_regular_file(fs::status(tmpPathname, ec));
            if (success)
            {
                tmpPathname.replace_extension(*pExtension2);
                success = fs::is_regular_file(fs::status(tmpPathname, ec));
            }
        }
        else
        {
            if (!EndsWithPath(pathname, extension1))
            {
                pathname += extension1;
            }
            success = fs::is_regular_file(fs::status(pathname, ec));
        }
    }

    return success;
}

void FixExtension(fs::path& pathname, const fs::path& extension1, const fs::path* pExtension2)
{
    if (pExtension2)
    {
        if (EndsWithPath(pathname, extension1) || EndsWithPath(pathname, *pExtension2))
        {
            pathname.replace_extension();
        }
    }
    else if (!EndsWithPath(pathname, extension1))
    {
        pathname += extension1;
    }
}

///////////////////////////////////////////////////////////////////////////

static atomic<std::uint64_t> tmpPathnameUnique{ 0 };

fs::path GetTempPathname(const char* pPrefix, const char* pSuffix)
{
    fs::path pathname = fs::temp_directory_path();

    if (pPrefix)
    {
        pathname /= pPrefix;
    }
    else
    {
        pathname /= "~";
    }

    pathname += to_string(++tmpPathnameUnique);

    if (pSuffix)
    {
        pathname += pSuffix;
    }

    return pathname;
}

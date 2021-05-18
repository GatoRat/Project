// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "GetPath.h"
#include "Util/String/Str.h"
#include "Util/System/SysGetPath.h"

using namespace std;

static fs::path appPath;
static fs::path appBasePath;
static fs::path docPath;
static fs::path downloadPath;
static fs::path tmpPath;

static fs::path appDataPath;
static fs::path roamingDataPath;
static fs::path programDataPath;

static fs::path desktopPath;


static bool CheckArg(const string& arg, string_view startsWith, fs::path& path)
{
    bool success = false;

    if (Str::StartsWith(arg, startsWith))
    {
        path = arg.substr(startsWith.length());
        if (!path.empty())
        {
            if (fs::is_directory(path))
            {
                success = true;
            }
            else
            {
                path.clear();
            }
        }
    }

    return success;
}

static bool CheckArg(const wstring& arg, wstring_view startsWith, fs::path& path)
{
    bool success = false;

    if (Str::StartsWith(arg, startsWith))
    {
        path = arg.substr(startsWith.length());
        if (!path.empty())
        {
            if (fs::is_directory(path))
            {
                success = true;
            }
            else
            {
                path.clear();
            }
        }
    }

    return success;
}

static void SetPaths(size_t basePathRemoveCount)
{
    if (appPath.empty())
    {
        appPath = SysGetPath::App();
    }

    if (appBasePath.empty())
    {
        appBasePath = appPath;

        while (basePathRemoveCount-- > 0)
        {
            appBasePath = appBasePath.parent_path();
        }
    }

    downloadPath = SysGetPath::Download();
    tmpPath = SysGetPath::Tmp();

    appDataPath = SysGetPath::AppData();
    roamingDataPath = SysGetPath::RoamingData();
    programDataPath = SysGetPath::ProgramData();
}

void InitDirectories(vector<wstring>& args, const wchar_t* pDocSubdir, size_t basePathRemoveCount) //-V826
{
    for (auto it = args.begin(); it != args.end();)
    {
        if (CheckArg(*it, L"AppPath=", appPath))
        {
            it = args.erase(it);
        }
        else if (CheckArg(*it, L"AppBasePath=", appBasePath))
        {
            it = args.erase(it);
        }
        else
        {
            ++it;
        }
    }

    docPath = SysGetPath::Doc();
    if (pDocSubdir && *pDocSubdir)
    {
        docPath /= pDocSubdir;
        fs::create_directories(docPath);
    }

    SetPaths(basePathRemoveCount);
}

void InitDirectories(vector<string>& args, const char* pDocSubdir, size_t basePathRemoveCount) //-V826
{
    for (auto it = args.begin(); it != args.end();)
    {
        if (CheckArg(*it, "AppPath=", appPath))
        {
            it = args.erase(it);
        }
        else if (CheckArg(*it, "AppBasePath=", appBasePath))
        {
            it = args.erase(it);
        }
        else
        {
            ++it;
        }
    }

    docPath = SysGetPath::Doc();
    if (pDocSubdir && *pDocSubdir)
    {
        docPath /= pDocSubdir;
        fs::create_directories(docPath);
    }

    SetPaths(basePathRemoveCount);
}

const fs::path& GetAppPath()
{
    return appPath;
}

const fs::path& GetAppBasePath()
{
    return appBasePath;
}

const fs::path& GetDocPath()
{
    return docPath;
}

const fs::path& GetDownloadPath()
{
    return downloadPath;
}

const fs::path& GetTmpPath()
{
    return tmpPath;
}

const fs::path& GetAppDataPath()
{
    return appDataPath;
}

const fs::path& GetRoamingDataPath()
{
    return roamingDataPath;
}

const fs::path& GetProgramDataPath()
{
    return programDataPath;
}

const fs::path& GetDesktopPath()
{
    return desktopPath;
}
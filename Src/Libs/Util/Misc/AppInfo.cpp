// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "AppInfo.h"
#include <sstream>
#include "../System/_lean_windows_.h"
#include "../String/Str.h"

using namespace std;

namespace AppInfo
{
    string   name;
    string   product;
    string   company;
    string   versionStr;
    fs::path exePath;
    fs::path exeName;
    fs::path exePathname;
    Version  version;

    struct InitAppInfo
    {
        InitAppInfo()
        {
#ifdef _WIN32
            // Note: _get_wpgmptr throws an exception if main() was used.
            //       _get_pgmptr throws an exception if wmain() was used.
            //       This uses Win32 to avoid "knowing" which main was used.

            vector<wchar_t> moduleFileName;
            moduleFileName.resize(MAX_PATH);
            for (;;)
            {
                const DWORD len = GetModuleFileNameW(NULL, &moduleFileName[0], static_cast<DWORD>(moduleFileName.size()));
                if (static_cast<size_t>(len) < moduleFileName.size())
                {
                    break;
                }

                const DWORD lastError = GetLastError();
                if (lastError != ERROR_INSUFFICIENT_BUFFER)
                {
                    exeName = L"UnknownProgram";
                    moduleFileName.clear();
                    break;
                }

                moduleFileName.resize(moduleFileName.size() + MAX_PATH);
            }

            if (!moduleFileName.empty())
            {
                exePathname = &moduleFileName[0];
                exePath = exePathname.parent_path();
                exeName = exePathname.stem();
            }

            name = exeName.string();
#else
            static const char* pSelfSymLink = "/proc/self/exe";

            char pathname[PATH_MAX + 1];
            ssize_t pathnameLen = readlink(pSelfSymLink, pathname, PATH_MAX);
            if (pathnameLen)
            {
                ssize_t dirLen(pathnameLen);

                while (dirLen && pathname[dirLen - 1] != '/')
                {
                    dirLen--;
                }

                if (dirLen)
                {
                    exePath.assign(pathname, dirLen);
                }

                exeName.assign(&pathname[dirLen], pathnameLen - dirLen);
            }
            else
            {
                exeName = "UnknownTestProgram";
            }
#endif
        }
    };

    static InitAppInfo initAppInfo;

    const string& GetName()
    {
        return name;
    }

    const string& GetProduct()
    {
        return product;
    }

    const string& GetCompany()
    {
        return company;
    }

    const string& GetVersionStr()
    {
        return versionStr;
    }

    const fs::path& GetExeName()
    {
        return exeName;
    }

    const fs::path& GetExePath()
    {
        return exePath;
    }

    const fs::path& GetExePathname()
    {
        return exePathname;
    }

    Version GetVersion()
    {
        return version;
    }

    void SetVersion(int major, int minor, int build, int revision)
    {
        stringstream ss;
        ss << major << '.' << minor << '.' << build << '.' << revision;
        versionStr = ss.str();
        version.major = major;
        version.minor = minor;
        version.build = build;
        version.revision = revision;
    }

    void SetProduct(string_view company_, string_view product_, const char* pName)
    {
        company = company_;
        product = product_;
        if (!Str::IsEmptyOrWhiteSpace(pName))
        {
            name = pName;
        }
    }
}

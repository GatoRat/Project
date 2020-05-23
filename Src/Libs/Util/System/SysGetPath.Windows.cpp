// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SysGetPath.h"
#define INCLUDE_SHLOBJ
#include "_lean_windows_.h"
#include <initguid.h>
#include <KnownFolders.h>
#include <wchar.h>
#include "../String/Str.h"
#include "../Misc/AppInfo.h"

using namespace std;

namespace SysGetPath
{
    fs::path App()
    {
        vector<wchar_t> buffer;

        do
        {
            buffer.resize(buffer.size() + MAX_PATH);
            GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));

        } while (GetLastError() == ERROR_INSUFFICIENT_BUFFER && buffer.size() < USHRT_MAX);

        return fs::canonical(fs::path(buffer.data()).parent_path());
    }

    static fs::path GetKnownFolderPath(REFKNOWNFOLDERID rfid, bool create = false)
    {
        fs::path path;

        LPWSTR tmpPath;
        HRESULT result = SHGetKnownFolderPath(rfid, create ? KF_FLAG_CREATE : KF_FLAG_DONT_VERIFY, 0, &tmpPath);
        if (result == S_OK)
        {
            path = fs::canonical(fs::path(tmpPath));
            CoTaskMemFree(tmpPath);
        }

        return path;
    }

    fs::path Doc()
    {
        return GetKnownFolderPath(FOLDERID_Documents);
    }

    fs::path Download()
    {
        return GetKnownFolderPath(FOLDERID_Downloads);
    }

    fs::path Tmp()
    {
        return fs::temp_directory_path();
    }

    fs::path BaseAppData()
    {
        return GetKnownFolderPath(FOLDERID_LocalAppData);
    }

    fs::path BaseRoamingData()
    {
        return GetKnownFolderPath(FOLDERID_RoamingAppData);
    }

    fs::path BaseProgramData()
    {
        return GetKnownFolderPath(FOLDERID_ProgramData);
    }

    /*
    static bool LoadVersionInfo()
    {
        wchar_t filename[MAX_PATH];
        if (!::GetModuleFileNameW(nullptr, filename, MAX_PATH))
        {
            return false;
        }

        DWORD dummyHandle;
        DWORD len = ::GetFileVersionInfoSizeW(filename, &dummyHandle);
        if (!len)
        {
            return false;
        }

        vector<char> verInfo;
        verInfo.resize(len);

        if (!::GetFileVersionInfoW(filename, 0, len, verInfo.data()))
        {
            return false;
        }

        void* pCompanyName = nullptr;
        UINT  companyNameLen = 0;

        void* pProductName = nullptr;
        UINT  productNameLen = 0;

        if (!VerQueryValueA(verInfo.data(), "\\StringFileInfo\\040904e4\\CompanyName", &pProductName, &productNameLen) ||
            !VerQueryValueA(verInfo.data(), "\\StringFileInfo\\040904e4\\ProductName", &pProductName, &productNameLen))
        {

        }
    }
    */

    struct Path final
    {
        fs::path path;

        Path(REFKNOWNFOLDERID rfid)
            : path(GetKnownFolderPath(rfid, true) / AppInfo::GetCompany() / AppInfo::GetProduct())
        {
            if (Str::CompareV(AppInfo::GetProduct(), AppInfo::GetName(), true) != 0)
            {
                path /= AppInfo::GetName();
            }

            error_code ec;
            if (!fs::create_directories(path, ec))
            {
                // TODO: What to do here
                // Clean up paths in case illegal chars are on it and try again?
            }
        }
    };

    fs::path AppData()
    {
        Path path(FOLDERID_LocalAppData);
        return path.path;
    }

    fs::path ProgramData()
    {
        Path path(FOLDERID_ProgramData);
        return path.path;
    }

    fs::path RoamingData()
    {
        Path path(FOLDERID_RoamingAppData);
        return path.path;
    }
    // FOLDERID_Music
    // FOLDERID_Pictures

    fs::path Desktop()
    {
        Path path(FOLDERID_Desktop);
        return path.path;
    }
}
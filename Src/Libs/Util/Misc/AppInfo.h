// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string>
#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;

namespace AppInfo
{
    struct Version
    {
        int major;
        int minor;
        int build;
        int revision;
    };

    const std::string& GetName();
    const std::string& GetProduct();
    const std::string& GetCompany();
    const std::string& GetVersionStr();

    const fs::path& GetExeName();
    const fs::path& GetExePath();
    const fs::path& GetExePathname();

    Version GetVersion();

    /** Sets version info.
       @remarks Should be called early in main using the data from
            the project's Version.h file.
    */
    void SetVersion(int major, int minor, int build, int revision);

    /** Sets company and product names.
        @param company The company name.
        @param product The product name.
        @param pName If not NULL, use this is name (else the name of the executable,
            minus extension, is used.)
        @remarks Should be called early in main using the data from
            the project's Version.h file.
    */
    void SetProduct(std::string_view company, std::string_view product, const char* pName = nullptr);

}; // namespace AppInfo

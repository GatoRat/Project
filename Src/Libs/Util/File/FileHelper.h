// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

bool EndsWith(const fs::path& pathToCheck, std::wstring_view strToCompare, bool ignoreCase = true);
bool EndsWithPath(const fs::path& pathToCheck, const fs::path& strToCompare, bool ignoreCase = true);

int Compare(const fs::path& path1, std::wstring_view strToCompare, bool ignoreCase = true);
int ComparePath(const fs::path& path1, const fs::path& path2, bool ignoreCase = true);

///////////////////////////////////////////////////////////////////////////

/**
    @remarks On success, if \p pExtension2 is nullptr, extension1 will be appended
        to pathname, else pExtension2 will be appended.
*/
bool FixAndVerify(fs::path& pathname, const fs::path& extension1, const fs::path* pExtension2 = nullptr);

void FixExtension(fs::path& pathname, const fs::path& extension1, const fs::path* pExtension2);

///////////////////////////////////////////////////////////////////////////

fs::path GetTempPathname(const char* pPrefix = nullptr, const char* pSuffix = nullptr);

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>

namespace fs = std::filesystem;

// Predicates and Hashes which can be used by std::map and std::set classes for paths.

/** A binary predicate to compare two paths using the native case requirements.
*/
struct path_native_case
{
    /** The predicate operator.
        @param path1 The first path to compare.
        @param path2 The second path to compare.
        @returns \b true if \a path1 is less than \a path2, else \b false.
        @remarks This is case insensitive.
    */
    bool operator()(const fs::path& path1, const fs::path& path2) const
    {
#ifdef _WIN32
        return _wcsicmp(path1.c_str(), path2.c_str()) < 0;
#else
        return path1 < path2;
#endif
    }
};

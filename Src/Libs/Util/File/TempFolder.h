#pragma once

#include <filesystem>
#include <mutex>
#include "../Time/TimeStamp.h"

namespace fs = std::filesystem;

class TempFolder final
{
public:
    TempFolder() = default;
    TempFolder(const TempFolder&) = delete;
    void operator=(const TempFolder&) = delete;
    ~TempFolder() = default;

    static constexpr uint64_t DefaultHoursToKeep = 72;

    void setPath(fs::path path, uint64_t hoursToKeep = DefaultHoursToKeep);

    fs::path getPathname(const char* pPrefix, const char* pExt);

    void cleanUp(const fs::path& path, uint64_t hoursToKeep = DefaultHoursToKeep);
    void cleanUp();

private:
    fs::path  _path;
    uint64_t  _hoursToKeep;     // This must be at least 1. This folder is for files which can't be immediately deleted.
    TimeStamp _lastTime;
    std::recursive_mutex _mutex;
};

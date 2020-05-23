// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <functional>
#include <thread>

namespace fs = std::filesystem;

class DirChangeNotifier final
{
public:
    typedef std::function<bool(const fs::path& monitoredPath)> Callback;

public:
    DirChangeNotifier() = default;
    DirChangeNotifier(const fs::path& pathToMonitor, bool watchSubTree, Callback dirChangeCallback);
    DirChangeNotifier(const DirChangeNotifier&) = delete;
    void operator=(const DirChangeNotifier&) = delete;
    ~DirChangeNotifier();

    void start(const fs::path& pathToMonitor, bool watchSubTree, Callback dirChangeCallback);
    void stop();

private:
    std::thread _thread;
    fs::path    _pathToMonitor;
    void*       _stopEvent = nullptr;
};

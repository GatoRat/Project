// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

class FileChange final
{
public:
    explicit FileChange(const fs::path& fileToMonitor);
    FileChange(const FileChange&) = delete;
    void operator=(const FileChange&) = delete;
    ~FileChange();

    const fs::path& getFile() const;
    const fs::path& getDir() const;

    bool waitForChange();

    void close();

public:
    struct Data
    {
        Data() = default;
        Data(const Data&) = delete;
        void operator=(const Data&) = delete;
        virtual ~Data() = default;
    };

private:
    std::unique_ptr<Data> _data;
    fs::path _file;
    fs::path _dir;
};

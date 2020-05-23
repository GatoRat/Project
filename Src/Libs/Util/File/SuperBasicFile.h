// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class SuperBasicFile final
{
public:
    /**
        @param fileMustExist If \b true, \p readOnly is \b true and file is not found,
            this will throw an exception.
    */
    SuperBasicFile(const fs::path& pathname_, bool readOnly, bool forceReadOnly = false, bool fileMustExist = true, bool exceptionOnError = true);
    SuperBasicFile(const SuperBasicFile&) = delete;
    void operator=(const SuperBasicFile&) = delete;
    ~SuperBasicFile();

    bool isOpen() const noexcept;

    int64_t getFileSize() const;

    uint32_t read(void* pBuffer, uint32_t lenToRead) const;
    uint32_t write(const void* pBuffer, uint32_t lenToWrite) const;

private:
    const fs::path& _pathname;
    void* _hFile;
};

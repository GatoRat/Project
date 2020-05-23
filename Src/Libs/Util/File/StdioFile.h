// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdio>
#ifdef _WIN32
    #include <share.h>
#endif
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class StdioFile
{
public:
    enum Origin
    {
        Cur = SEEK_CUR,
        Set = SEEK_SET,
        End = SEEK_END
    };

    static constexpr size_t StringReadLen = 256;

public:
    StdioFile() = default;
    // Calls fopen
    StdioFile(const fs::path& filename, const wchar_t* pMode, int shareMode = _SH_DENYWR);
    StdioFile(const StdioFile&) = delete;

    virtual ~StdioFile();

    operator FILE*() const noexcept;
    void operator=(const StdioFile&) = delete;

    // Calls fopen
    bool open(const fs::path& filename, const wchar_t* pMode, int shareMode = _SH_DENYWR);
    // Calls fclose
    void close() noexcept;
    bool isOpen() const noexcept;

    // Calls feof
    bool isEof() const noexcept;
    // Calls ferror
    int  error() const noexcept;
    // Calls clearerr
    void clearError() const;

    // Calls fflush
    void flush() const;

    // Calls ftell and fseek
    int64_t getLength(int64_t* pCurPos = nullptr) const;
    // Calls ftell and fseek
    int64_t getRemainingLength() const;

    // Calls fseek
    bool seek(int64_t offset, Origin origin = Set) const;
    // Calls fseek
    bool seekToBegin() const;
    // Calls fseek
    bool seekToEnd() const;

    // Calls ftell
    int64_t tell() const;

    // Calls fgetpos
    bool getPos(fpos_t& pos) const;
    // Calls fgetpos
    bool setPos(fpos_t pos) const;

    // Calls fread
    size_t read(void* pBuffer, size_t readLen) const;
    // Calls fread
    bool read(std::vector<uint8_t>& buffer) const;
    // Calls fread
    bool read(std::string& buffer) const;
    // Calls fread
    bool read(std::vector<uint8_t>& buffer, size_t maxReadLen) const;
    // Calls fread
    bool read(std::string& buffer, size_t maxReadLen) const;

    // Calls fread
    std::vector<uint8_t> read() const;

    // Calls fgetc
    int readChar() const;

    // Calls fgets
    int readString(char* pBuffer, int readLen) const;

    // Calls a series of fgets
    const char* readString(std::string& str, size_t maxReadLen = 0, size_t stringReadLen = StringReadLen) const;
    // Calls a series of fgets
    std::string readString(size_t maxReadLen = 0, size_t stringReadLen = StringReadLen) const;

    // Calls fwrite
    bool write(const void* pBuffer, size_t lenToWrite) const;
    // Calls fwrite
    bool write(const std::vector<uint8_t>& data) const;
    // Calls fwrite
    bool write(std::string_view str) const;

    // Calls fputs
    bool writeString(const char* pStr, const char* pStr2 = nullptr) const;
    // Calls fputs
    bool writeString(const std::string& str, const char* pStr2 = nullptr) const;

    // Calls fputc
    bool write(char ch) const;

private:
    int64_t calcLenToRead(size_t maxReadLen) const;

protected:
    FILE* _pFile = nullptr;
    fs::path _filename;
};

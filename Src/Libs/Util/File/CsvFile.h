// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <ostream>
#include <string>
#include <vector>
#include "Util/File/CsvFile.h"

namespace fs = std::filesystem;

class CsvFile
{
public:
    std::vector<std::vector<const char*>> rows;

public:
    CsvFile();
    explicit CsvFile(const fs::path& pathame);
    explicit CsvFile(std::string& data, bool moveStr = true, const char* pPathname = nullptr);
    explicit CsvFile(const std::string& data, const char* pPathname = nullptr);
    explicit CsvFile(const char* pStr, const char* pPathname = nullptr);

    void parse(const fs::path& pathame);
    void parse(std::string& data, bool moveStr = true, const char* pPathname = nullptr);
    void parse(const std::string& data, const char* pPathname = nullptr);
    void parse(const char* pStr, const char* pPathname = nullptr);

    void write(const fs::path& pathame) const;
    std::ostream& write(std::ostream& stream) const;

    size_t getMaxRowLength() const;
    bool checkRowLengths() const;
    void normalizeRowLengths();

    // Calling this may cause roundtripping to fail
    void trimCells();

    size_t numRows() const;
    size_t numColumns() const;

    const std::vector<const char*>& operator[](size_t row) const;

    const fs::path& getPathame() const;

private:
    void parse();

    std::string _data;
    fs::path _pathname;
};

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FastFile.h"
#include <fstream>
#include <sstream>
#include "../System/_lean_windows_.h"
#include "SuperBasicFile.h"
#include "StdioFile.h"
#include "XceptionFile.h"

using namespace std;

string FastReadTextFileCpp(const ifstream& inStream)
{
    return string(static_cast<stringstream const&>(stringstream() << inStream.rdbuf()).str());
}

string FastReadTextFileCpp(const fs::path& pathname)
{
    ifstream inStream(pathname);
    return inStream.bad() ? string() : FastReadTextFileCpp(inStream);
}

bool FastWriteTextFileCpp(const fs::path& pathname, string_view str)
{
    bool success = false;

    ofstream outStream(pathname);
    if (!outStream.bad())
    {
        outStream << str;
        outStream.flush();
        success = true;
    }

    return success;
}

string FastReadTextFile(const fs::path& pathname, int64_t maxToRead, int64_t* pFileSize)
{
    string str;
    FastReadTextFile(str, pathname, maxToRead, pFileSize);
    return str;
}

size_t FastReadTextFile(string& data, const fs::path& pathname, int64_t maxToRead, int64_t* pFileSize)
{
    StdioFile file(pathname, L"rt");

    if (maxToRead < 0 || pFileSize)
    {
        const int64_t fileSize = file.getLength();

        if (pFileSize)
        {
            *pFileSize = fileSize;
        }

        if (maxToRead < 0)
        {
            maxToRead = fileSize;
        }
    }

    data.resize(static_cast<size_t>(maxToRead));
    return file.read(data) ? data.size() : 0;
}

bool FastWriteTextFile(const fs::path& pathname, const char* pStr)
{
    StdioFile file(pathname, L"wt");
    return file.writeString(pStr ? pStr : "");
}

template<typename T>
uint32_t FastReadFileT(bool fileMustExist, T& data, const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool exclusive, bool exceptionOnOpenError = true)
{
    SuperBasicFile file(pathname, true, exclusive, fileMustExist, exceptionOnOpenError);

    uint32_t lenRead = 0;

    if (file.isOpen())
    {
        const int64_t fileSize = file.getFileSize();

        if (pFileSize)
        {
            *pFileSize = fileSize;
        }

        if (fileSize < static_cast<int64_t>(maxToRead))
        {
            maxToRead = static_cast<uint32_t>(fileSize);
        }

        data.resize(maxToRead + extraSize); //-V106
        lenRead = file.read(data.data(), maxToRead);
    }
    else if (pFileSize)
    {
        *pFileSize = -1;
    }

    data.resize(lenRead); //-V106
    return lenRead;
}

uint32_t FastReadFile(vector<uint8_t>& data, const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool forceReadOnly)
{
    return FastReadFileT(true, data, pathname, maxToRead, pFileSize, extraSize, forceReadOnly);
}

uint32_t FastReadFile(vector<char>& data, const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool forceReadOnly)
{
    return FastReadFileT(true, data, pathname, maxToRead, pFileSize, extraSize, forceReadOnly);
}

vector<uint8_t> FastReadFile(const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool forceReadOnly)
{
    vector<uint8_t> data;
    FastReadFileT(true, data, pathname, maxToRead, pFileSize, extraSize, forceReadOnly);
    return data;
}

vector<char> FastReadFile2(const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool forceReadOnly)
{
    vector<char> data;
    FastReadFileT(true, data, pathname, maxToRead, pFileSize, extraSize, forceReadOnly);
    return data;
}

/*
uint32_t FastReadFileStr(bool fileMustExist, string& str, const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool forceReadOnly)
{
    SuperBasicFile file(pathname, true, forceReadOnly, fileMustExist);

    uint32_t lenRead = 0;

    if (file.isOpen())
    {
        int64_t fileSize = file.getFileSize();

        if (pFileSize)
        {
            *pFileSize = fileSize;
        }

        if (fileSize < static_cast<int64_t>(maxToRead))
        {
            maxToRead = static_cast<uint32_t>(fileSize);
        }

        str.resize(maxToRead + extraSize); //-V106
        lenRead = file.read(&str[0], maxToRead);
    }
    else
    {
        if (pFileSize)
        {
            *pFileSize = -1;
        }
    }

    str.resize(lenRead); //-V106
    return lenRead;
}
*/

uint32_t FastReadFileStr(string& str, const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool forceReadOnly)
{
    return FastReadFileT(true, str, pathname, maxToRead, pFileSize, extraSize, forceReadOnly);
}

uint32_t FastReadFileStr_noExOnOpen(string& str, const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool forceReadOnly)
{
    return FastReadFileT(true, str, pathname, maxToRead, pFileSize, extraSize, forceReadOnly, false);
}

string FastReadFileStr(bool fileMustExist, const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool forceReadOnly)
{
    string str;
    FastReadFileT(fileMustExist, str, pathname, maxToRead, pFileSize, extraSize, forceReadOnly);
    return str;
}

string FastReadFileStr(const fs::path& pathname, uint32_t maxToRead, int64_t* pFileSize, uint32_t extraSize, bool forceReadOnly)
{
    return FastReadFileStr(true, pathname, maxToRead, pFileSize, extraSize, forceReadOnly);
}

uint32_t FastReadFile(bool fileMustExist, const fs::path& pathname, void* pBuffer, uint32_t maxToRead, int64_t* pFileSize, bool forceReadOnly)
{
    SuperBasicFile file(pathname, true, forceReadOnly, fileMustExist);

    uint32_t lenRead = 0;

    if (file.isOpen())
    {
        if (pFileSize)
        {
            *pFileSize = file.getFileSize();
        }

        lenRead = pBuffer && maxToRead ? file.read(pBuffer, maxToRead) : 0;
    }
    else
    {
        if (pFileSize)
        {
            *pFileSize = -1;
        }
    }

    return lenRead;
}

uint32_t FastReadFile(const fs::path& pathname, void* pBuffer, uint32_t maxToRead, int64_t* pFileSize, bool forceReadOnly)
{
    return FastReadFile(true, pathname, pBuffer, maxToRead, pFileSize, forceReadOnly);
}

uint32_t FastWriteFile(const fs::path& pathname, const void* pBuffer, uint32_t bytesToWrite)
{
    SuperBasicFile file(pathname, false);
    return file.write(pBuffer, bytesToWrite);
}

uint32_t FastWriteFile(const fs::path& pathname, const vector<uint8_t>& data)
{
    return FastWriteFile(pathname, data.data(), static_cast<uint32_t>(data.size()));
}

uint32_t FastWriteFile(const fs::path& pathname, string_view str)
{
    return FastWriteFile(pathname, str.data(), static_cast<uint32_t>(str.size()));
}

uint32_t FastWriteFileStr(const fs::path& pathname, const char* pStr, uint32_t charsToWrite)
{
    const uint32_t lenToWrite = static_cast<uint32_t>(min((pStr ? strlen(pStr) : 0), static_cast<size_t>(charsToWrite)));
    return FastWriteFile(pathname, pStr, lenToWrite);
}

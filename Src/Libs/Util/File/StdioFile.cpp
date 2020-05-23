// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "StdioFile.h"
#include "XceptionFile.h"
#include "../String/Str.h"

using namespace std;

StdioFile::StdioFile(const fs::path& filename, const wchar_t* pMode, int shareMode)
{
    if (!open(filename, pMode, shareMode))
    {
        throw XceptionFile(filename, static_cast<intptr_t>(errno));
    }
}

StdioFile::~StdioFile()
{
    close();
}

StdioFile::operator FILE*() const noexcept
{
    return _pFile;
}

bool StdioFile::isOpen() const noexcept
{
    return _pFile != nullptr;
}

bool StdioFile::open(const fs::path& filename, const wchar_t* pMode, int shareMode)
{
    bool success = false;

    close();
    _pFile = _wfsopen(filename.wstring().c_str(), pMode ? pMode : L"", shareMode);
    if (_pFile)
    {
        success = true;
        _filename = filename;
    }

    return success;
}

void StdioFile::close() noexcept
{
    if (isOpen())
    {
        fclose(_pFile);
        _pFile = nullptr;
        _filename.clear();
    }
}

bool StdioFile::isEof() const noexcept
{
    return isOpen() ? feof(_pFile) != 0 : false;
}

int StdioFile::error() const noexcept
{
    return isOpen() ? ferror(_pFile) : 0;
}

void StdioFile::clearError() const
{
    if (isOpen())
    {
        clearerr(_pFile);
    }
}

void StdioFile::flush() const
{
    if (isOpen())
    {
        fflush(_pFile);
    }
}

int64_t StdioFile::getLength(int64_t* pCurPos) const
{
    int64_t len = -1;

    if (isOpen())
    {
        const int64_t curPos = tell();
        if (curPos >= 0)
        {
            seekToEnd();
            len = tell();
            seek(curPos);
        }

        if (pCurPos)
        {
            *pCurPos = curPos;
        }
    }

    return len;
}

int64_t StdioFile::getRemainingLength() const
{
    int64_t curPos;
    const int64_t len = getLength(&curPos);
    return len >= 0 && curPos >= 0 ? len - curPos : -1;
}

bool StdioFile::seek(int64_t offset, Origin origin) const
{
    return isOpen() && _fseeki64(_pFile, offset, origin) == 0;
}

bool StdioFile::seekToBegin() const
{
    return seek(0, Set);
}

bool StdioFile::seekToEnd() const
{
    return seek(0, End);
}

int64_t StdioFile::tell() const
{
    return isOpen() ? _ftelli64(_pFile) : -1;
}

bool StdioFile::getPos(fpos_t& pos) const
{
    return isOpen() && fgetpos(_pFile, &pos) == 0;
}

bool StdioFile::setPos(fpos_t pos) const
{
    return isOpen() && fsetpos(_pFile, &pos) == 0;
}

size_t StdioFile::read(void* pBuffer, size_t readLen) const
{
    size_t lenRead = 0;
    if (isOpen() && pBuffer && readLen)
    {
        lenRead = fread_s(pBuffer, readLen, 1, readLen, _pFile);
    }
    return lenRead;
}

int64_t StdioFile::calcLenToRead(size_t maxReadLen) const
{
    int64_t remainingLen = getRemainingLength();
    if (remainingLen > 0)
    {
        if (static_cast<size_t>(remainingLen) > maxReadLen)
        {
            remainingLen = static_cast<int64_t>(maxReadLen);
        }
    }
    return remainingLen;
}

bool StdioFile::read(vector<uint8_t>& buffer) const
{
    const size_t lenRead = buffer.empty() ? 0 : fread_s(&buffer[0], buffer.size(), 1, buffer.size(), _pFile);
    buffer.resize(lenRead);
    return !(lenRead == 0 && error());
}

vector<uint8_t> StdioFile::read() const
{
    vector<uint8_t> data;
    data.resize(getLength());
    read(data);
    return data;
}

bool StdioFile::read(string& buffer) const
{
    const size_t lenRead = buffer.empty() ? 0 : fread_s(&buffer[0], buffer.size(), 1, buffer.size(), _pFile);
    buffer.resize(lenRead);
    return !(lenRead == 0 && error());
}

bool StdioFile::read(vector<uint8_t>& buffer, size_t maxReadLen) const
{
    buffer.resize(calcLenToRead(maxReadLen));
    return read(buffer);
}

bool StdioFile::read(string& buffer, size_t maxReadLen) const
{
    buffer.resize(calcLenToRead(maxReadLen));
    return read(buffer);
}

int StdioFile::readChar() const
{
    int ch = 0;
    if (isOpen())
    {
        ch = fgetc(_pFile);
    }
    return ch;
}

int StdioFile::readString(char* pBuffer, int readLen) const
{
    int len = -1;
    if (isOpen() && pBuffer && readLen)
    {
        const char* pStr = fgets(pBuffer, readLen, _pFile);
        if (pStr)
        {
            len = static_cast<int>(strlen(pStr));
        }
    }
    return len;
}

const char* StdioFile::readString(string& str, size_t maxReadLen, size_t stringReadLen) const
{
    const char* pReturnStr = nullptr;
    str.resize(0);

    if (isOpen())
    {
        if (stringReadLen == 0)
        {
            stringReadLen = StringReadLen;
        }
        else if (stringReadLen > USHRT_MAX)
        {
            stringReadLen = USHRT_MAX;
        }

        if (maxReadLen == 0)
        {
            maxReadLen = SIZE_MAX;
        }

        size_t curLen = 0;

        do
        {
            size_t readLen = stringReadLen;
            if (maxReadLen <= stringReadLen)
            {
                readLen = maxReadLen;
                maxReadLen = 0;
            }
            else
            {
                maxReadLen -= stringReadLen;
            }

            str.resize(curLen + readLen + 1); // fgets reads until num - 1 characters and automatically appends a terminating null

            const char* pStr = fgets(&str[curLen], static_cast<int>(readLen + 1), _pFile);
            if (!pStr)
            {
                break;
            }

            const size_t len = strlen(pStr);
            curLen += len;

            if (len < static_cast<size_t>(readLen))
            {
                break;
            }

        } while (maxReadLen);

        str.resize(curLen);

        if (ferror(_pFile) == 0)
        {
            pReturnStr = str.data();
        }
    }

    return pReturnStr;
}

string StdioFile::readString(size_t maxReadLen, size_t stringReadLen) const
{
    string str;
    readString(str, maxReadLen, stringReadLen);
    return str;
}

bool StdioFile::write(const void* pBuffer, size_t lenToWrite) const
{
    bool result = false;

    if (isOpen())
    {
        const size_t lenWritten = fwrite(pBuffer, 1, lenToWrite, _pFile);
        result = lenWritten == lenToWrite;
    }

    return result;
}

bool StdioFile::write(const vector<uint8_t>& data) const
{
    return write(data.data(), data.size());
}

bool StdioFile::write(string_view str) const
{
    return write(str.data(), str.size());
}

bool StdioFile::writeString(const char* pStr, const char* pStr2) const
{
    bool result = false;

    if (isOpen() && pStr)
    {
        result = fputs(pStr, _pFile) != EOF;
        if (result && !Str::IsNullOrEmpty(pStr2))
        {
            result = fputs(pStr2, _pFile) != EOF;
        }
    }

    return result;
}

bool StdioFile::writeString(const string& str, const char* pStr2) const
{
    return writeString(str.c_str(), pStr2);
}

bool StdioFile::write(char ch) const
{
    bool result = false;

    if (isOpen())
    {
        result = fputc(ch, _pFile) != EOF;
    }

    return result;
}

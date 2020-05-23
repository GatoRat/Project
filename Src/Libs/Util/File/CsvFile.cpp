// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CsvFile.h"
#include "Util/File/CheckUtf8Marker.h"
#include "Util/File/FastFile.h"
#include "Util/File/XceptionFile.h"
#include "Util/String/ASCII.h"
#include "Util/String/Str.h"
#include <fstream>
#include <sstream>

using namespace std;

CsvFile::CsvFile()
{
}

CsvFile::CsvFile(const fs::path& pathname)
    : _pathname(pathname)
{
    parse(pathname);
}

CsvFile::CsvFile(string& str, bool moveStr, const char* pPathname)
    : _pathname(pPathname ? pPathname : "")
{
    parse(str, moveStr);
}

CsvFile::CsvFile(const string& str, const char* pPathname)
    : _data(str)
    , _pathname(pPathname ? pPathname : "")
{
    parse();
}

CsvFile::CsvFile(const char* pStr, const char* pPathname)
    : _data(pStr ? pStr : "")
    , _pathname(pPathname ? pPathname : "")
{
    if (!pStr)
    {
        throw Xception(0, "Invalid parameter: pStr == nullptr");
    }
    parse();
}

void CsvFile::parse(const fs::path& pathname)
{
    _pathname = pathname;

    if (!FastReadFileStr(_data, pathname, FastReadAll))
    {
        throw XceptionFile(pathname, 0, "File is empty");
    }

    parse();
}

void CsvFile::parse(string& str, bool moveStr, const char* pPathname)
{
    _pathname = pPathname ? pPathname : "";

    if (moveStr)
    {
        _data = move(str);
    }
    else
    {
        _data = str;
    }
    parse();
}

void CsvFile::parse(const string& str, const char* pPathname)
{
    _pathname = pPathname ? pPathname : "";

    _data = str;
    parse();
}

void CsvFile::parse(const char* pStr, const char* pPathname)
{
    _pathname = pPathname ? pPathname : "";

    if (pStr && *pStr)
    {
        _data = pStr;
        parse();
    }
    else
    {
        _data.clear();
    }
}

/// <summary>
/// Ensures the character <paramref name="pSrc"/> is pointing at is zero. If it was not
/// originally zero, it increments <paramref name="pSrc"/> past the comma or EOL characters,
/// returning a <b>true</b> if there was an EOL or terminating zero.
/// <summery>
/// <param name="pSrc">The pSrc pointer.</param>
/// <returns><b>true</b> if pSrc was pointing at EOL or terminating zero, else <b>false</b></returns>
static inline //-V2506
bool EndCell(char*& pSrc)
{
    if (!*pSrc)
    {
        return true;
    }

    const char ch = *pSrc;
    *pSrc++ = 0;

    if (ch == ASCII::CR)
    {
        if (*pSrc == ASCII::LF)
        {
            ++pSrc;
        }
        return true;
    }

    return ch == ASCII::LF;
}

void CsvFile::parse()
{
    rows.clear();

    if (!_data.empty())
    {
        vector<const char*> line;
        char* pSrc = _data.data();
        CheckUtf8Marker(pSrc);

        const auto error = [&](const char* pWhat)
        {
            stringstream ss;
            if (pWhat)
            {
                ss << pWhat;
                ss << ": ";
            }
            ss << "Line " << rows.size() + 1 << ", Column " << line.size();

            throw XceptionFile(_pathname, 0, ss.str());
        };

        for (;;)
        {
            if (ASCII::IsEOL(*pSrc))
            {
                if (line.empty())
                {
                    break;
                }

                line.emplace_back(pSrc);
                EndCell(pSrc);
                const size_t lineSize = line.size();
                rows.emplace_back(move(line));

                if (!*pSrc)
                {
                    break;
                }

                line.reserve(lineSize); //-V1030
            }
            else if (*pSrc == ',')
            {
                *pSrc = 0;
                line.emplace_back(pSrc++);
            }
            else if (*pSrc == '"')
            {
                char* pDst = ++pSrc;
                line.emplace_back(pDst);

                for (;;)
                {
                    if (*pSrc == '"')
                    {
                        if (pSrc[1] == ',' || ASCII::IsEOL(pSrc[1]))
                        {
                            ++pSrc;
                            *pDst = 0;
                            break;
                        }

                        if (*++pSrc != '"')
                        {
                            error("Double-quote not followed by double-quote or comma");
                        }
                    }
                    else if (!*pSrc) //-V2516
                    {
                        error("Unterminated double-quote");
                    }

                    *pDst++ = *pSrc++;
                }

                if (EndCell(pSrc))
                {
                    const size_t lineSize = line.size();
                    rows.emplace_back(move(line));
                    line.reserve(lineSize); //-V1030
                }
            }
            else
            {
                line.push_back(pSrc);

                while (*pSrc != ',' && !ASCII::IsEOL(*pSrc))
                {
                    ++pSrc;
                }

                if (EndCell(pSrc))
                {
                    const size_t lineSize = line.size();
                    rows.emplace_back(move(line));
                    line.reserve(lineSize); //-V1030
                }
            }
        }
    }
}

void CsvFile::write(const fs::path& pathname) const
{
    ofstream file;

    file.exceptions(ofstream::badbit);
    file.open(pathname, ios::out | ios::trunc);
    if (!file.is_open())
    {
        throw XceptionFile(pathname, nullptr);
    }

    try
    {
        write(file);
    }
    catch (Xception& e)
    {
        try
        {
            file.close();
            fs::remove(pathname);
        }
        catch (exception&)
        { //-V565
        }

        throw XceptionFile(pathname, e.what());
    }
}

ostream& CsvFile::write(ostream& stream) const
{
    for (const auto& row : rows)
    {
        for (auto it = row.cbegin();;)
        {
            if (*it && (*it)[0])
            {
                const size_t offset = strcspn(*it, ",\"\r\n"); //-V2513
                if (!(*it)[offset])
                {
                    stream << *it;
                }
                else
                {
                    stream << '"';

                    for (const char* pStr = *it; *pStr; pStr++)
                    {
                        stream << *pStr;
                        if (*pStr == '"')
                        {
                            stream << '"';
                        }
                    }

                    stream << '"';
                }
            }

            if (++it == row.cend())
            {
                break;
            }

            stream << ',';
        }
        stream << '\n';
    }
    return stream;
}

size_t CsvFile::getMaxRowLength() const
{
    size_t maxRowLen = 0;

    for (auto& row : rows)
    {
        if (row.size() > maxRowLen)
        {
            maxRowLen = row.size();
        }
    }

    return maxRowLen;
}

bool CsvFile::checkRowLengths() const
{
    bool success = true;

    if (!rows.empty())
    {
        auto row = rows.cbegin();
        auto prevRow = row++;

        while (row != rows.cend())
        {
            if (prevRow->size() != row->size())
            {
                success = false;
                break;
            }

            prevRow = row++;
        }
    }

    return success;
}

void CsvFile::normalizeRowLengths()
{
    if (!rows.empty())
    {
        bool needsNormalization = false;

        auto row = rows.begin();
        size_t maxRowSize = row->size();
        for (++row; row != rows.end(); ++row)
        {
            if (row->size() != maxRowSize)
            {
                needsNormalization = true;

                if (row->size() > maxRowSize)
                {
                    maxRowSize = row->size();
                }
            }
        }

        if (needsNormalization)
        {
            const char* pStr = _data.data() + _data.size();
            for (row = rows.begin(); row != rows.end(); ++row)
            {
                row->resize(maxRowSize, pStr);
            }
        }
    }
}

void CsvFile::trimCells()
{
    for (auto& row : rows)
    {
        for (auto& cell : row)
        {
            if (*cell)
            {
                // const is cast away so trimming can be done in-place.
                auto pStr = const_cast<char*>(cell);
                while (ASCII::IsWhitespace(*pStr) != 0)
                {
                    pStr++;
                }

                size_t len = strlen(pStr); //-V2513
                if (len)
                {
                    while (ASCII::IsWhitespace(pStr[len - 1]) != 0)
                    {
                        len--;
                    }

                    pStr[len] = 0;
                }

                cell = pStr;
            }
        }
    }
}

size_t CsvFile::numRows() const
{
    return rows.size();
}

size_t CsvFile::numColumns() const
{
    return numRows() ? rows[0].size() : 0;
}

const vector<const char*>& CsvFile::operator[](size_t row) const
{
    return rows[row];
}

const fs::path& CsvFile::getPathame() const
{
    return _pathname;
}

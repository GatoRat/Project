// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TestHelper.TmpFile.h"
#include <fstream>

using namespace std;

namespace TestHelper
{
    void TmpFile::create(const TmpDir& parent, const char* pFileName, fstream* pFile, ios_base::openmode mode)
    {
        if (!pFileName)
        {
            throw invalid_argument("pFileName is NULL.");
        }
        else if (!*pFileName)
        {
            throw invalid_argument("pFileName is empty.");
        }
        else
        {
            fs::path pathname = parent.fullpath() / pFileName;

            if (pFile)
            {
                pFile->open(pathname, mode);
            }

            _pathname = move(pathname);
        }
    }

    TmpFile::TmpFile(const TmpDir& parent, const char* pFileName, fstream* pFile, ios_base::openmode mode)
    {
        create(parent, pFileName, pFile, mode);
    }

    TmpFile::TmpFile(const TmpDir& parent, const char* pFileName, const char* pData, streamsize dataLen)
    {
        fstream file;
        file.exceptions(fstream::failbit | fstream::badbit);

        create(parent, pFileName, &file, ios_base::out | ios_base::trunc | ios_base::binary);

        if (pData && dataLen)
        {
            file.write(pData, dataLen);
        }
    }

    TmpFile::~TmpFile()
    {
        remove();
    }

    void TmpFile::remove()
    {
        if (!_pathname.empty())
        {
            error_code ec;
            fs::remove(_pathname, ec);
        }
    }
}

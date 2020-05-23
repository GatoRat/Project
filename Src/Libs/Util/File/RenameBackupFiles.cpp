// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "RenameBackupFiles.h"
#include "../String/ConvertW.h"
#include "../String/GetNumDigits.h"

using namespace std;

#ifdef _WIN32
    constexpr wchar_t* pExtDot = L".";
#else
    constexpr char* pExtDot = ".";
#endif

constexpr size_t CountBufferLen = 7;

void RenameBackupFiles(const fs::path& pathname, uint16_t maxBackups)
{
    if (!pathname.empty() && maxBackups)
    {
        const fs::file_status fileStatus = fs::status(pathname);
        if (fs::exists(fileStatus) && fs::is_regular_file(fileStatus))
        {
            const fs::path ext{ pathname.extension() };

#ifdef _WIN32
            wchar_t buffer[CountBufferLen];
#else
            char buffer[CountBufferLen];
#endif
            const size_t maxDigits = GetNumDigits(maxBackups);
            auto pStr = ConvertW::ToStrFixedFast(buffer, maxBackups, maxDigits);
            *pStr = 0;

            fs::path basePathname{ pathname };
            basePathname.replace_extension(pExtDot);
/*
            basePathname.replace_extension();
            basePathname.replace_extension(pPreExtension);
*/

            fs::path dstPathname{ basePathname };
            dstPathname += buffer;
            dstPathname += ext;

            error_code ec; // ignored
            fs::remove(dstPathname, ec);

            for (size_t srcBackup = maxBackups - 1; srcBackup; srcBackup--)
            {
                ConvertW::ToStrFixedFast(buffer, srcBackup, maxDigits);

                fs::path srcPathname{ basePathname };
                srcPathname += buffer;
                srcPathname += ext;

                fs::rename(srcPathname, dstPathname, ec);

                dstPathname = move(srcPathname);
            }

            fs::rename(pathname, dstPathname, ec);
        }
    }
}

/*
void RenameBackupFiles(const fs::path& pathname, uint16_t maxBackups)
{
    if (!pathname.empty() && maxBackups)
    {
        fs::file_status fileStatus = fs::status(pathname);
        if (fs::exists(fileStatus) && fs::is_regular_file(fileStatus))
        {
            fs::path basePathname{ pathname.parent_path() };
            basePathname /= StemPrefix;
            basePathname += pathname.stem();

            fs::path ext{ pathname.extension() };

            if (ext.empty() || ext == pExtDot)
            {
                ext = pDefaultExt;
            }

#ifdef _WIN32
            wchar_t buffer[CountBufferLen];
#else
            char buffer[CountBufferLen];
#endif
            buffer[0] = *pExtDot;
            size_t maxDigits = GetNumDigits(maxBackups);
            auto pStr = Convert::ToStrFixedFast(&buffer[1], maxBackups, maxDigits);
            *pStr = 0;

            fs::path dstPathname{ basePathname };
            dstPathname += buffer;
            dstPathname += ext;

            error_code ec; // ignored
            fs::remove(dstPathname, ec);

            for (size_t srcBackup = maxBackups - 1; srcBackup; srcBackup--)
            {
                Convert::ToStrFixedFast(&buffer[1], srcBackup, maxDigits);

                fs::path srcPathname{ basePathname };
                srcPathname += buffer;
                srcPathname += ext;

                fs::rename(srcPathname, dstPathname, ec);

                dstPathname = move(srcPathname);
            }

            fs::rename(pathname, dstPathname, ec);
        }
    }
}

*/

/*
void RenameBackupFiles2(const fs::path& pathname, uint16_t maxBackups)
{
    if (!pathname.empty() && maxBackups)
    {
        fs::file_status fileStatus = fs::status(pathname);
        if (fs::exists(fileStatus) && fs::is_regular_file(fileStatus))
        {
            int maxDigits = GetNumDigits(maxBackups);

#ifdef _WIN32
            wostringstream ss;
            ss <<  L'.' << setw(maxDigits) << setfill(L'0') << maxBackups << L".bak";
#else
            ostringstream ss;
            ss << '.' << setw(maxDigits) << setfill('0') << maxBackups << ".bak";
#endif
            fs::path dstPathname(pathname);
            dstPathname += ss.str();

            error_code ec; // ignored
            fs::remove(dstPathname, ec);

            for (size_t srcBackup = maxBackups - 1; srcBackup; srcBackup--)
            {
                ss.clear();
#ifdef _WIN32
                ss.str(L"");
                ss <<  L'.' << setw(maxDigits) << setfill(L'0') << srcBackup << L".bak";
#else
                ss.str("");
                ss << '.' << setw(maxDigits) << setfill('0') << srcBackup << ".bak";
#endif
                fs::path srcPathname(pathname);
                srcPathname += ss.str();
                fs::rename(srcPathname, dstPathname, ec);

                dstPathname = move(srcPathname);
            }

            fs::rename(pathname, dstPathname, ec);
        }
    }
}
*/
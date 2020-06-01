// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TestHelper.TmpDir.h"
#include "Util/Misc/AppInfo.h"
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

using namespace std;

namespace TestHelper
{
    static fs::path CreateUniqueDir(const fs::path& base)
    {
        fs::path fullpath;

        auto seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(static_cast<unsigned>(seed));

        error_code ec;
        do
        {
            stringstream ss;
            ss << hex << setfill('0') << setw(8) << generator();

            fullpath = base;
            fullpath += ss.str();

        } while (!fs::create_directory(fullpath, ec));

        return fullpath;
    }

    void TmpDir::create(const TmpDir& parentDir, const char* pDirName)
    {
        fs::path fullpath(parentDir.fullpath());

        if (pDirName && *pDirName)
        {
            if (const char* pEnd = strchr(pDirName, '*'); pEnd)
            {
                fullpath.append(pDirName, pEnd);
                _fullpath = CreateUniqueDir(fullpath);
            }
            else
            {

                fullpath /= pDirName;

                try
                {
                    fs::create_directory(fullpath);
                }
                catch (const fs::filesystem_error& e)
                {
                    cerr << "ERROR: " << e.code() << ": TmpDir::create: Could not create temp directory: '" << _fullpath << "'" << endl;
                    throw exception();
                }

                _fullpath = fullpath;
            }
        }
        else
        {
            fullpath /= "~dir.";
            _fullpath = CreateUniqueDir(fullpath);
        }
    }

    TmpDir::TmpDir(const char* pDirName)
    {
        create(GetTmpDir(), pDirName);
    }

    TmpDir::TmpDir(const TmpDir& parentDir, const char* pDirName)
    {
        create(parentDir, pDirName);
    }

    TmpDir::~TmpDir()
    {
        remove();
    }

    void TmpDir::remove()
    {
        if (!_fullpath.empty())
        {
            error_code ec; // ignored
            fs::remove_all(_fullpath, ec);
            _fullpath.clear();
        }
    }

    class BaseTmpDir : public TmpDir
    {
    public:
        BaseTmpDir()
            : TmpDir(CreateBaseDir())
        {
        }

    private:
        static fs::path CreateBaseDir()
        {
            fs::path tmpPath = fs::temp_directory_path();
            tmpPath /= "~";
            tmpPath += AppInfo::GetExeName();
            tmpPath += '.';
            return CreateUniqueDir(tmpPath);
        }
    };

    const TmpDir& GetTmpDir()
    {
        static BaseTmpDir baseTmpDir;
        return baseTmpDir;
    }
}

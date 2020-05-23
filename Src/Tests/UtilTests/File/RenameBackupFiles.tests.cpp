// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/File/FastFile.h"
#include "Util/File/RenameBackupFiles.h"
#include "TestHelper/TestHelper.TmpDir.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("File/RenameBackupFiles")
{
    SECTION("path empty")
    {
        fs::path pathname;
        RenameBackupFiles(pathname, 2);
        // No checks, just making sure there are no system errors
    }
    SECTION("file doesn't exist")
    {
        TestHelper::TmpDir tmpDir(NULL);
        fs::path pathname(tmpDir.fullpath());
        pathname /= "InvalidFile";

        RenameBackupFiles(pathname, 2);
    }
    SECTION("maxBackups=0")
    {
        TestHelper::TmpDir tmpDir("Ren");
        fs::path pathname(tmpDir.fullpath());
        pathname /= "File.txt";

        FastWriteFile(pathname, "1", 1);

        RenameBackupFiles(pathname, 0);

        size_t count = 0;
        for (auto& file : fs::directory_iterator(tmpDir.fullpath()))
        {
            REQUIRE(file.path().filename().string() == "File.txt"); //-V521
            ++count;
        }

        REQUIRE(count == 1); //-V521

        string contents = FastReadTextFile(pathname);
        REQUIRE(contents == "1"); //-V521
    }
    SECTION("maxBackups=2")
    {
        TestHelper::TmpDir tmpDir("Ren");
        fs::path pathname(tmpDir.fullpath());
        pathname /= "File.txt";

        FastWriteFile(pathname, "1", 1);
        RenameBackupFiles(pathname, 2);

        {
            vector<fs::path> paths;
            for (auto& file : fs::directory_iterator(tmpDir.fullpath()))
            {
                paths.push_back(file);
            }
            REQUIRE(paths.size() == 1); //-V521
        }
        {
            fs::path tmpPathname(tmpDir.fullpath());
            tmpPathname /= "File.1.txt";
            string contents = FastReadTextFile(tmpPathname);
            REQUIRE(contents == "1"); //-V521
        }

        FastWriteFile(pathname, "2", 1);
        RenameBackupFiles(pathname, 2);

        {
            vector<fs::path> paths;
            for (auto& file : fs::directory_iterator(tmpDir.fullpath()))
            {
                paths.push_back(file);
            }
            REQUIRE(paths.size() == 2); //-V521
        }
        {
            fs::path tmpPathname(tmpDir.fullpath());
            tmpPathname /= "File.2.txt";
            string contents = FastReadTextFile(tmpPathname);
            REQUIRE(contents == "1"); //-V521
        }
        {
            fs::path tmpPathname(tmpDir.fullpath());
            tmpPathname /= "File.1.txt";
            string contents = FastReadTextFile(tmpPathname);
            REQUIRE(contents == "2"); //-V521
        }

        FastWriteFile(pathname, "3", 1);
        RenameBackupFiles(pathname, 2);

        {
            vector<fs::path> paths;
            for (auto& file : fs::directory_iterator(tmpDir.fullpath()))
            {
                paths.push_back(file);
            }
            REQUIRE(paths.size() == 2); //-V521
        }
        {
            fs::path tmpPathname(tmpDir.fullpath());
            tmpPathname /= "File.2.txt";
            string contents = FastReadTextFile(tmpPathname);
            REQUIRE(contents == "2"); //-V521
        }
        {
            fs::path tmpPathname(tmpDir.fullpath());
            tmpPathname /= "File.1.txt";
            string contents = FastReadTextFile(tmpPathname);
            REQUIRE(contents == "3"); //-V521
        }
    }
    SECTION("maxBackups=99, no extension")
    {
        TestHelper::TmpDir tmpDir("Ren");
        fs::path pathname(tmpDir.fullpath());
        pathname /= "File";

        FastWriteFile(pathname, "1", 1);
        RenameBackupFiles(pathname, 99);

        {
            vector<fs::path> paths;
            for (auto& file : fs::directory_iterator(tmpDir.fullpath()))
            {
                paths.push_back(file);
            }
            REQUIRE(paths.size() == 1); //-V521
        }
        {
            fs::path tmpPathname(tmpDir.fullpath());
            tmpPathname /= "File.01";
            string contents = FastReadTextFile(tmpPathname);
            REQUIRE(contents == "1"); //-V521
        }

        FastWriteFile(pathname, "2", 1);
        RenameBackupFiles(pathname, 99);

        {
            vector<fs::path> paths;
            for (auto& file : fs::directory_iterator(tmpDir.fullpath()))
            {
                paths.push_back(file);
            }
            REQUIRE(paths.size() == 2); //-V521
        }
        {
            fs::path tmpPathname(tmpDir.fullpath());
            tmpPathname /= "File.02";
            string contents = FastReadTextFile(tmpPathname);
            REQUIRE(contents == "1"); //-V521
        }
        {
            fs::path tmpPathname(tmpDir.fullpath());
            tmpPathname /= "File.01";
            string contents = FastReadTextFile(tmpPathname);
            REQUIRE(contents == "2"); //-V521
        }
    }
}

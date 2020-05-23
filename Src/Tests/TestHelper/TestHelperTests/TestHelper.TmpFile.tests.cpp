// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <fstream>
#include "Util/System/_lean_windows_.h"
#include "TestHelper/TestHelper.TmpFile.h"
#include "TestHelper/catch.hpp"

// This test is only run when debugging TestHelper::TmpFile
// to reduce file i/o and speed up other tests.

using namespace std;

TEST_CASE("TestHelper.TmpFile")
{
    const TestHelper::TmpDir& baseTmpDir = TestHelper::GetTmpDir();

    SECTION("::TmpFile(..., NULL, 0)")
    {
        fs::path pathname;

        {
            TestHelper::TmpFile tmpFile(baseTmpDir, "1", NULL, 0);

            pathname = tmpFile.pathname();
            REQUIRE(!pathname.empty()); //-V521

            fstream file(pathname, ios::in | ios::out | ios::trunc);
            REQUIRE(file.good()); //-V521

            streampos filesize = file.tellg();
            REQUIRE(filesize == static_cast<streampos>(0)); //-V521

            file.seekg(0, ios::end);
            filesize = file.tellg() - filesize;
            REQUIRE(filesize == static_cast<streampos>(0)); //-V521
            REQUIRE(file.good()); //-V521

            file << "text";
            REQUIRE(file.good()); //-V521

            file.flush();
            REQUIRE(file.good()); //-V521

            filesize = file.tellg();
            REQUIRE(filesize == static_cast<streampos>(4)); //-V112 //-V521
        }
        {
            fstream file(pathname);
            REQUIRE(!file.good()); //-V521
        }
    }
    SECTION("::TmpFile(..., !NULL, 0)")
    {
        fs::path pathname;

        {
            TestHelper::TmpFile tmpFile(baseTmpDir, "1", "abc", 0);

            pathname = tmpFile.pathname();
            REQUIRE(!pathname.empty()); //-V521

            fstream file(pathname, ios::in | ios::out | ios::trunc);
            REQUIRE(file.good()); //-V521

            streampos filesize = file.tellg();
            REQUIRE(filesize == static_cast<streampos>(0)); //-V521

            file.seekg(0, ios::end);
            filesize = file.tellg() - filesize;
            REQUIRE(filesize == static_cast<streampos>(0)); //-V521
            REQUIRE(file.good()); //-V521

            file << "text";
            REQUIRE(file.good()); //-V521

            file.flush();
            REQUIRE(file.good()); //-V521

            filesize = file.tellg();
            REQUIRE(filesize == static_cast<streampos>(4)); //-V112 //-V521
        }
        {
            fstream file(pathname);
            REQUIRE(!file.good()); //-V521
        }
    }
    SECTION("::TmpFile(..., NULL, !0)")
    {
        fs::path pathname;

        {
            TestHelper::TmpFile tmpFile(baseTmpDir, "1", NULL, 3);

            pathname = tmpFile.pathname();
            REQUIRE(!pathname.empty()); //-V521

            fstream file(pathname, ios::in | ios::out | ios::trunc);
            REQUIRE(file.good()); //-V521

            streampos filesize = file.tellg();
            REQUIRE(filesize == static_cast<streampos>(0)); //-V521

            file.seekg(0, ios::end);
            filesize = file.tellg() - filesize;
            REQUIRE(filesize == static_cast<streampos>(0)); //-V521
            REQUIRE(file.good()); //-V521

            file << "text";
            REQUIRE(file.good()); //-V521

            file.flush();
            REQUIRE(file.good()); //-V521

            filesize = file.tellg();
            REQUIRE(filesize == static_cast<streampos>(4)); //-V112 //-V521
        }
        {
            fstream file(pathname);
            REQUIRE(!file.good()); //-V521
        }
    }
    SECTION("::TmpFile(..., !NULL, !0)")
    {
        fs::path pathname;

        {
            TestHelper::TmpFile tmpFile(baseTmpDir, "1", "abc", 3);

            pathname = tmpFile.pathname();
            REQUIRE(!pathname.empty()); //-V521

            fstream file(pathname);
            REQUIRE(file.good()); //-V521

            streampos filesize = file.tellg();
            REQUIRE(filesize == static_cast<streampos>(0)); //-V521

            file.seekg(0, ios::end);
            filesize = file.tellg() - filesize;
            REQUIRE(filesize == static_cast<streampos>(3)); //-V521
            REQUIRE(file.good()); //-V521

            file.seekg(0, ios::beg);

            string data;
            file >> data;
            REQUIRE(file.eof()); //-V521
            REQUIRE(data == string("abc")); //-V521
        }
        {
            fstream file(pathname);
            REQUIRE(!file.good()); //-V521
        }
    }
    SECTION("::TmpFile(..., fstream*)")
    {
        fs::path pathname;

        {
            fstream file;
            TestHelper::TmpFile tmpFile(baseTmpDir, "1", &file);
            REQUIRE(file.good()); //-V521

            pathname = tmpFile.pathname();
            REQUIRE(!pathname.empty()); //-V521

            streampos filesize = file.tellg();
            REQUIRE(filesize == static_cast<streampos>(0)); //-V521

            file.seekg(0, ios::end);
            filesize = file.tellg() - filesize;
            REQUIRE(filesize == static_cast<streampos>(0)); //-V521
            REQUIRE(file.good()); //-V521

            file << "text";
            REQUIRE(file.good()); //-V521

            file.flush();
            REQUIRE(file.good()); //-V521

            filesize = file.tellg();
            REQUIRE(filesize == static_cast<streampos>(4)); //-V112 //-V521

            file.close();
        }
        {
            fstream file(pathname);
            REQUIRE(!file.good()); //-V521
        }
    }
}

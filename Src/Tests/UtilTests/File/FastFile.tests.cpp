// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/File/FastFile.h"
#include "Util/File/XceptionFile.h"
#include "TestHelper/TestHelper.TmpDir.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("File/FastFile")
{
    SECTION("FastWriteFileStr/FastReadFileStr")
    {
        TestHelper::TmpDir tmpDir(nullptr);
        fs::path pathname(tmpDir.fullpath());
        pathname /= "file.txt";

        uint32_t lenWritten = FastWriteFileStr(pathname, "1bC");
        REQUIRE(lenWritten == 3); //-V521

        string str("string");
        str = FastReadFileStr(pathname, 0, nullptr, 0, true);
        REQUIRE(str.empty()); //-V521

        str = FastReadFileStr(pathname, 1, nullptr, 0, true);
        REQUIRE(str == "1"); //-V521

        str = FastReadFileStr(pathname, 1, nullptr, 128, true);
        REQUIRE(str == "1"); //-V521
        REQUIRE(str.capacity() >= 128); //-V521

        int64_t fileSize = 0;
        str = FastReadFileStr(pathname, 2, &fileSize, 0, true);
        REQUIRE(str == "1b"); //-V521
        REQUIRE(fileSize == 3); //-V521

        str = FastReadFileStr(pathname, FastReadAll, nullptr, 0, true);
        REQUIRE(str == "1bC"); //-V521

        str.reserve(1024);
        uint32_t lenRead = FastReadFileStr(str, pathname, FastReadAll, nullptr, 1);
        REQUIRE(lenRead == 3); //-V521
        REQUIRE(str == "1bC"); //-V521
        REQUIRE(str.capacity() >= 1024); //-V521
    }

    //uint64_t fileSize = 0;
    //string str = FastReadFileStr(pathname, 0, &fileSize, 0, true);
}

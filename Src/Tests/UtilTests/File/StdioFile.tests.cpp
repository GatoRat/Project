// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/File/StdioFile.h"
#include "Util/File/XceptionFile.h"
#include "TestHelper/TestHelper.TmpDir.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("File/StdioFile")
{
    SECTION("ctor (file doesn't exist)")
    {
        TestHelper::TmpDir tmpDir(NULL);
        fs::path pathname(tmpDir.fullpath());
        pathname /= "InvalidFile";

        try
        {
            StdioFile file(pathname, L"rb");
            FAIL("XceptionFile should have been thrown");
        }
        catch (XceptionFile&)
        { //-V565
        }
    }
    SECTION("ctor (create file)")
    {
        TestHelper::TmpDir tmpDir(NULL);
        fs::path pathname(tmpDir.fullpath());
        pathname /= "ValidFile";

        try
        {
            StdioFile file(pathname, L"wb");
            REQUIRE(file.isOpen()); //-V521
            FILE* pFile = file;
            REQUIRE(pFile != nullptr); //-V521
        }
        catch (XceptionFile&)
        {
            FAIL("XceptionFile should NOT have been thrown");
        }

        REQUIRE(fs::is_regular_file(pathname)); //-V521
    }
    SECTION("open (file doesn't exist)")
    {
        TestHelper::TmpDir tmpDir(NULL);
        fs::path pathname(tmpDir.fullpath());
        pathname /= "InvalidFile";

        StdioFile file;
        REQUIRE_FALSE(file.open(pathname, L"rb")); //-V521
    }
    SECTION("open (create file)")
    {
        TestHelper::TmpDir tmpDir(NULL);
        fs::path pathname(tmpDir.fullpath());
        pathname /= "ValidFile";

        StdioFile file;
        REQUIRE(file.open(pathname, L"wb")); //-V521
        REQUIRE(file.isOpen()); //-V521
        FILE* pFile = file;
        REQUIRE(pFile != nullptr); //-V521

        file.close();
        REQUIRE_FALSE(file.isOpen()); //-V521
        pFile = file;
        REQUIRE(pFile == nullptr); //-V521

        REQUIRE(fs::is_regular_file(pathname)); //-V521
    }
    SECTION("read/write")
    {
        TestHelper::TmpDir tmpDir(NULL);
        fs::path pathname(tmpDir.fullpath());
        pathname /= "File";

        const char charData{ 'A' };
        const vector<uint8_t> vectorData{ 1, 2, 3 };
        const string stringData{ "xyz" };

        {
            StdioFile file(pathname, L"wb");
            file.write(charData);
            file.write(vectorData);
            file.write(stringData);

            int64_t pos = file.tell();
            REQUIRE(pos == 7); //-V521
        }
        {
            StdioFile file(pathname, L"rb");
            int ch = file.readChar();
            REQUIRE(ch == charData); //-V521

            vector<uint8_t> buffer;
            file.read(buffer, 3);

            REQUIRE(buffer.size() == vectorData.size()); //-V521
            for (size_t i = 0; i < vectorData.size(); ++i)
            {
                REQUIRE(buffer[i] == vectorData[i]); //-V521
            }

            string str;
            file.read(str, 3);

            REQUIRE(str.size() == stringData.size()); //-V521
            for (size_t i = 0; i < stringData.size(); ++i)
            {
                REQUIRE(str[i] == stringData[i]); //-V521
            }

            REQUIRE_FALSE(file.isEof()); //-V521

            ch = file.readChar();
            REQUIRE(ch == -1); //-V521
            REQUIRE(file.isEof()); //-V521
        }
    }
    SECTION("read/write string")
    {
        TestHelper::TmpDir tmpDir(NULL);
        fs::path pathname(tmpDir.fullpath());
        pathname /= "File";

        string baseStr("012");

        StdioFile file(pathname, L"w+b");
        file.write(baseStr);
        file.write(0);
        file.write('A');

        ////

        REQUIRE(file.seekToBegin()); //-V521

        string readString;
        const char* pStr = file.readString(readString, 0, 2);

        REQUIRE(pStr != nullptr); //-V521
        REQUIRE(readString.size() == baseStr.size()); //-V521
        REQUIRE(readString == baseStr); //-V521

        ////

        REQUIRE(file.seekToBegin()); //-V521
        pStr = file.readString(readString, 0, 3);

        REQUIRE(pStr != nullptr); //-V521
        REQUIRE(readString.size() == baseStr.size()); //-V521
        REQUIRE(readString == baseStr); //-V521

        ////

        REQUIRE(file.seekToBegin()); //-V521
        pStr = file.readString(readString, 0, 4); //-V112

        REQUIRE(pStr != nullptr); //-V521
        REQUIRE(readString.size() == baseStr.size()); //-V521
        REQUIRE(readString == baseStr); //-V521
    }
    SECTION("read/write string (eof)")
    {
        TestHelper::TmpDir tmpDir(NULL);
        fs::path pathname(tmpDir.fullpath());
        pathname /= "File";

        string baseStr("012");

        StdioFile file(pathname, L"w+b");
        file.write(baseStr);

        ////

        REQUIRE(file.seekToBegin()); //-V521

        string readString;
        const char* pStr = file.readString(readString, 0, 2);

        REQUIRE(pStr != nullptr); //-V521
        REQUIRE(readString.size() == baseStr.size()); //-V521
        REQUIRE(readString == baseStr); //-V521

        ////

        REQUIRE(file.seekToBegin()); //-V521
        pStr = file.readString(readString, 0, 3);

        REQUIRE(pStr != nullptr); //-V521
        REQUIRE(readString.size() == baseStr.size()); //-V521
        REQUIRE(readString == baseStr); //-V521

        ////

        REQUIRE(file.seekToBegin()); //-V521
        pStr = file.readString(readString, 0, 4); //-V112

        REQUIRE(pStr != nullptr); //-V521
        REQUIRE(readString.size() == baseStr.size()); //-V521
        REQUIRE(readString == baseStr); //-V521
    }
}

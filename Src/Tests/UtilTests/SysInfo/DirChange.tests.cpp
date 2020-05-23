// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/System/DirChange.h"
#include "Util/File/XceptionFile.h"
#include "Util/File/FastFile.h"
#include "Util/Misc/WaitEvent.h"
#include "TestHelper/TestHelper.TmpDir.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("SysInfo/DirChangeNotifier")
{
    TestHelper::TmpDir tmpDir("Change");
    fs::path pathname(tmpDir.fullpath());
    pathname /= "File.txt";

    FastWriteFile(pathname, "1", 1);
    this_thread::sleep_for(2ms);
    WaitEvent waitEvent;
    {
        // NOTE: This may get called twice. Once for the actual file creation and once when the
        //       the file is written.

        DirChangeNotifier dirChange(tmpDir.fullpath(), true, [&](const fs::path& pathToMonitor)->bool
        {
            try
            {
                fs::path fileToRead(pathToMonitor / "File.txt");
                auto contents = FastReadFile(fileToRead);
                REQUIRE(contents.size() == 1); //-V521
                REQUIRE(contents[0] == '2'); //-V521
                waitEvent.set();
                return false;
            }
            catch (XceptionFile& e)
            {
                [[maybe_unused]] intptr_t code = e.code();
                [[maybe_unused]] string errStr = e.errStr();
                return true;
            }
        });

        FastWriteFile(pathname, "2", 1);
        REQUIRE(waitEvent.waitFor(2000)); //-V521

        //fs::path pathnameDir(tmpDir.fullpath());
        //pathnameDir /= "Dir";

        //fs::create_directory(pathnameDir);
        //REQUIRE(waitEvent.waitFor(2000));
    }
}

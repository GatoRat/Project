// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TestHelper/TestHelper.TmpDir.h"
#include "TestHelper/catch.hpp"

// This test is only run when debugging testHelper::TmpDir since
// automating it requires system calls that aren't portable.
// Plus, once it works, it works.

using namespace std;

TEST_CASE("TmpDir")
{
    const TestHelper::TmpDir& baseTmpDir = TestHelper::GetTmpDir();

    {
        TestHelper::TmpDir tmpDir1("1");
        TestHelper::TmpDir tmpDir2(baseTmpDir, "2");
        TestHelper::TmpDir tmpDir3(NULL);
        TestHelper::TmpDir tmpDir4(baseTmpDir, NULL);
        {
            TestHelper::TmpDir tmpDir5(tmpDir2, "5");
            TestHelper::TmpDir tmpDir5a(tmpDir2, "5a");
        }
        TestHelper::TmpDir tmpDir6(tmpDir4, "6");
    }
}

#include "SummitApp/Report/ReportFolder.h"
#include "TestHelper/TestHelper.TmpDir.h"
#include "TestHelper/TestHelper.TmpFile.h"
#include "TestHelper/catch.hpp"
#include "Util/File/FastFile.h"
#include "Util/File/FastFindFile.h"
#include "Util/String/Str.h"

using namespace std;

TEST_CASE("Report/ReportFolder")
{
    TestHelper::TmpDir tmpDir(nullptr);

    reportFolder.setPath(tmpDir.fullpath(), 2);

    auto createFile = [](const fs::path& pathname, uint8_t val)
    {
        FastWriteFile(pathname, &val, 1);
    };

    createFile(reportFolder.getPathname(nullptr, nullptr), 1);
    createFile(reportFolder.getPathname(nullptr, nullptr), 2);
    createFile(reportFolder.getPathname(nullptr, nullptr), 3);

    createFile(reportFolder.getPathname(nullptr, nullptr), 101);
    createFile(reportFolder.getPathname(nullptr, nullptr), 102);
    createFile(reportFolder.getPathname(nullptr, nullptr), 103);
    createFile(reportFolder.getPathname(nullptr, nullptr), 104);

    reportFolder.cleanUp();

    vector<fs::path> endOfDay;
    vector<fs::path> single;
    vector<fs::path> other;

    FastFindFile::Find(tmpDir.fullpath(), "*", FastFindFile::Types::OnlyFiles, [&](const fs::path& /*basePath_*/, const FastFindData& findData)->bool
    {
        if (Str::StartsWith(findData.cFileName, L"DrawSingleReport."))
        {
            single.push_back(findData.cFileName);
        }
        else if (Str::StartsWith(findData.cFileName, L"DrawReport."))
        {
            endOfDay.push_back(findData.cFileName);
        }
        else
        {
            other.push_back(findData.cFileName);
        }

        return true;
    });

    REQUIRE(other.size()    == 0);
    REQUIRE(endOfDay.size() == 2);
    REQUIRE(single.size()   == 3);
}